#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sched.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "NiFpga_9159FPGAVI.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

// Log max size = 5 megabytes.
#define LOG_BUFFER_SIZE 5000000

enum register_types{
	BOOL,
	U32,
	SGL
};

struct results{
	double min;
	double max;
	double avg;
};


void printWithStatus(char *str, int status){
	printf("%s... ", str);
	if(status){
		printf("Failed: %d.\n", status);
		exit(-1);
	}
	else
		printf("Success.\n");
}

double timespecDelta(struct timespec *begin, struct timespec *end){
	return (double) ((end->tv_sec - begin->tv_sec) + ((double)(end->tv_nsec - begin->tv_nsec))/1E9);
}

void computeResults(struct results *values, double *durations, int size){
	double total = 0;
	int i = 0;

	 // Initialize values structure.
	values->avg = 0;
	values->max = 0;
	values->min = 1E9;

	for(i = 0; i < size; i++){
		total += durations[i];
		values->min = (durations[i] < values->min) ? durations[i] : values->min;
		values->max = (durations[i] > values->max) ? durations[i] : values->max;
	}

	values->avg = total / size;
}

void printResults(char *title, char *subtitle, struct results *values){
	printf("%s\n", title);
	printf("   %s\n", subtitle);
	printf("      Min Latency: %03.3f ms\n", values->min * 1e3);
	printf("      Max Latency: %03.3f ms\n", values->max * 1e3);
	printf("      Avg Latency: %03.3f ms\n", values->avg * 1e3);
}

void logResults(char *log_buffer, char *result_string, double *durations, int iterations){
	double duration=0;
	uint32_t i=0;
	// Write result section header to log buffer.
	snprintf(log_buffer + strlen(log_buffer),
		strlen(log_buffer),
		result_string);
	for(i=0; i < iterations; i++){ // Write duration values to log buffer.
		duration = durations[i] * 1e3;
		snprintf(log_buffer + strlen(log_buffer),
				strlen(log_buffer),
				"%u, %g\n",
				i,
				duration);
	}
}

int main(int argc, char *argv[]){
	// NiFpga C Interface variables.
	NiFpga_Status status = NiFpga_False;
	NiFpga_Session session;

	// Test configuration variables.
	int32_t dmaReadSize;
	int32_t warmupIterCount = 0;
	int32_t normalIterCount = 0;

	// RIO FPGA resource variables..
	char resource[256];
	char ip[32];
	char bitfile[256];
	char targetType[32];
	int32_t isMXI = 0;

	// Set application thread to highest priority.
	struct sched_param params;
	pthread_t thread=pthread_self();

	// Determine maximum priority for SCHED_FIFO policy.
	params.sched_priority = sched_get_priority_max(SCHED_FIFO);

	// Assign SCHED_FIFO policy to current thread.
	int32_t schedStatus = pthread_setschedparam(thread, SCHED_FIFO, &params);
	if(schedStatus){
		printf("Failed to set schedule parameters: %d\n", schedStatus);
		exit(EXIT_FAILURE);
	}

	// Assign highest priority to current thread.
	int32_t pthread_status=pthread_setschedprio(thread,params.sched_priority);
	if(pthread_status){
		printf("Error setting pthread priority: %d\n", pthread_status);
		exit(EXIT_FAILURE);
	}
	printf("Assigning current thread to priority %u using SCHED_FIFO policy.\n", params.sched_priority);

	// Test execution variables.
	struct results values;
	size_t elementsRemaining;
	int32_t i32_reg;
	int i = 0, count;
	struct timespec tp1, tp2, tp3;
	NiFpga_Bool fifo_timeout=0;

	// Assign code to run on CPU defined in executable arguments.
	pid_t pid = getpid();
	cpu_set_t set;
	uint32_t cpuNo = atoi(argv[1]);
	CPU_SET(cpuNo, &set);
	if(sched_setaffinity(pid, sizeof(set), &set)==-1){
		exit(EXIT_FAILURE);
	}

	// Create log file descriptor and variables.
	int log_fd;
	char log_name[16];
	char log_buffer[LOG_BUFFER_SIZE];
	char log_path[] = "/home/ryan/rio_latency/test_data/";
	ssize_t nbytes;

	// Prompt user for test configuration.
	printf("\033c");
	printf("NI RIO Expansion Latency Benchmark\n");
	printf("Process ID: %i\n", pid);
	printf("Running on CPU: %i\n", cpuNo);
	printf("Press any key to continue.\n");
	getchar();
	printf("Please enter the log file name: ");
	scanf("%s", log_name);
	strcat(log_path, log_name);
	printf("Please enter the path to the bitfile: ");
	scanf("%s", bitfile);
	printf("Please enter number of warmup iterations: ");
	scanf("%u", &warmupIterCount);
	printf("Please enter number of normal iterations: ");
	scanf("%u", &normalIterCount);
	printf("Please enter the DMA Read Size: ");
	scanf("%i", &dmaReadSize);
	printf("Expansion Chassis Type? (0: Ethernet, 1: MXIe): ");
	scanf("%i", &isMXI);

	// Create log file descriptor.
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	log_fd = open(log_path, O_WRONLY | O_APPEND | O_CREAT, mode);

	// Create dynamically sized variables.
	double durationToFpga[normalIterCount], durationFromFpga[normalIterCount];
	int32_t input[dmaReadSize];
	uint32_t arbFIFOReadSize = 10;
	uint64_t input64;

	// Form RIO address for MXIe or Ethernet chassis.
	if(isMXI==1){
		strcat(targetType, "9159");
		printf("Please enter the RIO resource name for the MXIe chassis: ");
		scanf("%s", resource);
	} else {
		strcat(targetType, "9149");
		printf("Please enter the IP address or RIO resource string for the RIO Expansion Chassis: ");
		scanf("%s", ip);
		strcat(resource, "rio://");
		strcat(resource, ip);
		strcat(resource, "/RIO0");
	}

	printf("RIO Resource URL: %s\n", resource);

	// Write test configuration data to log file.
	snprintf(log_buffer,
			sizeof(log_buffer),
			"Running benchmark on CPU #%u\nTarget Type: %s\nBitfile: %s\nRIO Resource: %s\nWarm Up Iterations: %u\nNormal Iterations: %u\nU32DMA Read Size: %u\n",
			cpuNo,
			targetType,
			bitfile,
			resource,
			warmupIterCount,
			normalIterCount,
			dmaReadSize);

	// Initialize NiFpga libraries.
	NiFpga_MergeStatus(&status, NiFpga_Initialize());
	printWithStatus("Initializing NiFpga", status);

	if(NiFpga_IsNotError(status)){
		// Open session to RIO resource.
		NiFpga_MergeStatus(&status,
				NiFpga_Open(bitfile,
						NiFpga_9159FPGAVI_Signature,
						resource,
						0,
						&session));
		printf("Opening session to %s", resource);
		printWithStatus("", status);
		printf("\n");

		if(NiFpga_IsNotError(status)){
			// Reset FPGA VI.
			NiFpga_MergeStatus(&status,
					NiFpga_Reset(session));

			// Run FPGA VI.
			NiFpga_MergeStatus(&status,
					NiFpga_Run(session, 0));

			// Initialize FPGA values.
			// Set DMA Loop Toggles to False.
			NiFpga_MergeStatus(&status,
					NiFpga_WriteBool(session,
							NiFpga_9159FPGAVI_ControlBool_SlowIOScanLoop_Toggle,
							NiFpga_False));
			NiFpga_MergeStatus(&status,
					NiFpga_WriteBool(session,
							NiFpga_9159FPGAVI_ControlBool_Trigger_ArbFIFO_Bool,
							NiFpga_False));

			// Initialize Slow DO output.
			NiFpga_MergeStatus(&status,
					NiFpga_WriteU32(session,
							NiFpga_9159FPGAVI_ControlU32_Mod10_U32,
							0));
			// Set Slow DO Register loop rate in us.
			NiFpga_MergeStatus(&status,
					NiFpga_WriteU32(session,
							NiFpga_9159FPGAVI_ControlU32_SlowDOLoop_Rate_us,
							10000));

			// Set Slow Scan DMA Loop rate in us.
			NiFpga_MergeStatus(&status,
					NiFpga_WriteU32(session,
							NiFpga_9159FPGAVI_ControlU32_SlowScanLoop_Rate_us,
							10000));

			// Set Fast AI loop rate in us.
			NiFpga_MergeStatus(&status,
					NiFpga_WriteU32(session,
							NiFpga_9159FPGAVI_ControlU32_FastAILoop_Rate_us,
							1000));

			// U32 register read/write benchmark.
			if(NiFpga_IsNotError(status)){
				count = 0; // Increment count to send new data every write.
				for(i = 0; i < warmupIterCount + normalIterCount; i++){
					clock_gettime(CLOCK_MONOTONIC, &tp1);
					NiFpga_MergeStatus(&status,
							NiFpga_WriteU32(session,
									NiFpga_9159FPGAVI_ControlU32_Mod10_U32,
									count));
					clock_gettime(CLOCK_MONOTONIC, &tp2);
					NiFpga_MergeStatus(&status,
							NiFpga_ReadI32(session,
									NiFpga_9159FPGAVI_IndicatorI32_TargetToHostLoopIterCount,
									&i32_reg));
					clock_gettime(CLOCK_MONOTONIC, &tp3);

					if(i >= warmupIterCount){
						durationToFpga[count] = timespecDelta(&tp1, &tp2);
						durationFromFpga[count] = timespecDelta(&tp2, &tp3);
						count++;
					}
				}

				// Compute, print, and log U32 Register Write results.
				computeResults(&values, durationToFpga, normalIterCount);
				printResults("U32 register", "To FPGA", &values);
				logResults(log_buffer,
						"U32 Register Read\n",
						durationToFpga,
						normalIterCount);

				// Compute, print, and log U32 Register Read results.
				computeResults(&values, durationFromFpga, normalIterCount);
				printResults("U32 register", "From FPGA", &values);
				logResults(log_buffer,
						"U32 Register Write\n",
						durationToFpga,
						normalIterCount);
			}

			printf("\n");
			printWithStatus("U32 register", status);
			printf("\n");



			// U64 Arbitrary Data FIFO read.
			if(NiFpga_IsNotError(status)) {

				// Set number of samples to read.
				NiFpga_MergeStatus(&status,
					NiFpga_WriteI16(session,
							NiFpga_9159FPGAVI_ControlI16_ArbFIFO_Read_Size_Samples,
							arbFIFOReadSize));
				NiFpga_MergeStatus(&status,
						NiFpga_ReadBool(session,
							NiFpga_9159FPGAVI_IndicatorBool_ArbDataLoop_TimedOut,
							&fifo_timeout));
				printf("U64 FIFO timeout: %d\n", fifo_timeout);

				// Trigger data transmission.
				NiFpga_MergeStatus(&status,
						NiFpga_WriteBool(session,
								NiFpga_9159FPGAVI_ControlBool_Trigger_ArbFIFO_Bool,
								NiFpga_True));

				// Read samples from FIFO.
				for(i = 0; i<arbFIFOReadSize-2; i++){
					clock_gettime(CLOCK_MONOTONIC, &tp1);
					NiFpga_MergeStatus(&status,
						NiFpga_ReadFifoU64(session,
							NiFpga_9159FPGAVI_TargetToHostFifoU64_TargetToHostArbU64FIFO,
							&input64,
							1,
							500,
							&elementsRemaining));
					clock_gettime(CLOCK_MONOTONIC, &tp2);
					durationFromFpga[count] = timespecDelta(&tp1, &tp2);

					NiFpga_MergeStatus(&status,
						NiFpga_ReadBool(session,
								NiFpga_9159FPGAVI_IndicatorBool_ArbDataLoop_TimedOut,
								&fifo_timeout));

					NiFpga_MergeStatus(&status,
							NiFpga_ReadBool(session,
									NiFpga_9159FPGAVI_IndicatorBool_ArbDataLoop_TimedOut,
									&fifo_timeout));
					printf("U64 FIFO timeout: %d\n", fifo_timeout);

					if(fifo_timeout==1){
						status=-90210;
					}
				}

				// Print results
				printf("\n");
				computeResults(&values, durationFromFpga, arbFIFOReadSize);
				printResults("U64 FIFO", "From FPGA", &values);
				logResults(log_buffer,
						"U64 FIFO Read",
						durationFromFpga,
						arbFIFOReadSize);
				printf("\n");
				printWithStatus("U64 FIFO Read\n", status);
				printf("\n");
			}

			// I32 IO DMA FIFO read.
			if(NiFpga_IsNotError(status)) {
				count = 0;
				// Set IO scan rate (ms).
				NiFpga_MergeStatus(&status,
						NiFpga_WriteU32(session,
								NiFpga_9159FPGAVI_ControlU32_SlowScanLoop_Rate_us,
								100));

				// Check target FIFO timeout.
				NiFpga_MergeStatus(&status,
						NiFpga_ReadBool(session,
								NiFpga_9159FPGAVI_IndicatorBool_SlowIOScanLoopTimedOut,
								&fifo_timeout));

				// Toggle IO scan on.
				NiFpga_MergeStatus(&status,
						NiFpga_WriteU32(session,
								NiFpga_9159FPGAVI_ControlBool_SlowIOScanLoop_Toggle,
								NiFpga_True));

				// Read from IO scan FIFO.
				for(i = 0; i < warmupIterCount + normalIterCount; i++){
					clock_gettime(CLOCK_MONOTONIC, &tp1);
					NiFpga_MergeStatus(&status,
							NiFpga_ReadFifoI32(session,
								NiFpga_9159FPGAVI_TargetToHostFifoI32_TargetToHostI32FIFO,
								input,
								dmaReadSize,
								NiFpga_InfiniteTimeout,
								&elementsRemaining));
					clock_gettime(CLOCK_MONOTONIC, &tp2);
					if(i >= warmupIterCount){
						durationFromFpga[count] = timespecDelta(&tp1, &tp2);
						count++;
					}

					// Check target FIFO timeout.
					NiFpga_MergeStatus(&status,
							NiFpga_ReadBool(session,
									NiFpga_9159FPGAVI_IndicatorBool_SlowIOScanLoopTimedOut,
									&fifo_timeout));

					if(fifo_timeout==1){
						status=-90210;
					}
				}

				//Toggle IO scan off.
				NiFpga_MergeStatus(&status,
						NiFpga_WriteBool(session,
								NiFpga_9159FPGAVI_ControlBool_SlowIOScanLoop_Toggle,
								NiFpga_False));

				// Print results
				printf("\n");
				computeResults(&values, durationFromFpga, normalIterCount);
				printResults("I32 FIFO", "From FPGA", &values);
				logResults(log_buffer,
						"I32 IO SCan FIFO Write",
						durationFromFpga,
						normalIterCount);
				printf("\n");
				printWithStatus("I32 IO Scan FIFO Write\n", status);
				printf("\n");
			}
		}

		// Write log buffer to log file.
		nbytes = strlen(log_buffer);
		if(write(log_fd, log_buffer, nbytes) != nbytes){
			printf("Error in log write\n");
		}

		// Close session to RIO resource.
		NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));
		printWithStatus("Closing session", status);

		// Finalize NiFpga libraries.
		status = NiFpga_Finalize();
		printWithStatus("Finalizing NiFpga", status);
	}

	close(log_fd);
	printf("\nExiting.\n\n");
	return 0;
}
