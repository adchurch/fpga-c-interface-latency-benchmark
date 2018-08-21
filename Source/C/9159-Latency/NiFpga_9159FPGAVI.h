/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_9159FPGAVI_h__
#define __NiFpga_9159FPGAVI_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_9159FPGAVI_Bitfile;
 */
#define NiFpga_9159FPGAVI_Bitfile "NiFpga_9159FPGAVI.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_9159FPGAVI_Signature = "5CA60EC123F01B6B98386DB81738C23A";

typedef enum
{
   NiFpga_9159FPGAVI_IndicatorBool_ArbDataLoop_TimedOut = 0x8112,
   NiFpga_9159FPGAVI_IndicatorBool_ArbFIFO_Done = 0x811A,
   NiFpga_9159FPGAVI_IndicatorBool_SlowIOScanLoopTimedOut = 0x819A,
   NiFpga_9159FPGAVI_IndicatorBool_TargetToHostLoopTimedOut = 0x815E,
} NiFpga_9159FPGAVI_IndicatorBool;

typedef enum
{
   NiFpga_9159FPGAVI_IndicatorU8_FastDI_U8 = 0x81AA,
   NiFpga_9159FPGAVI_IndicatorU8_Mod2_U8 = 0x8166,
} NiFpga_9159FPGAVI_IndicatorU8;

typedef enum
{
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI0_I32 = 0x8194,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI10_I32 = 0x813C,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI11_I32 = 0x8138,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI12_I32 = 0x8134,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI13_I32 = 0x8130,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI14_I32 = 0x812C,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI15_I32 = 0x8128,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI1_I32 = 0x8190,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI2_I32 = 0x818C,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI3_I32 = 0x8188,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI4_I32 = 0x8154,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI5_I32 = 0x8150,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI6_I32 = 0x814C,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI7_I32 = 0x8148,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI8_I32 = 0x8144,
   NiFpga_9159FPGAVI_IndicatorI32_Mod3_AI9_I32 = 0x8140,
   NiFpga_9159FPGAVI_IndicatorI32_TargetToHostLoopIterCount = 0x8158,
} NiFpga_9159FPGAVI_IndicatorI32;

typedef enum
{
   NiFpga_9159FPGAVI_IndicatorU32_FastAILoop_Duration_ticks = 0x8184,
   NiFpga_9159FPGAVI_IndicatorU32_Mod5_U32 = 0x8160,
   NiFpga_9159FPGAVI_IndicatorU32_Mod6_U32 = 0x810C,
   NiFpga_9159FPGAVI_IndicatorU32_SlowDOLoop_Duration_ticks = 0x817C,
   NiFpga_9159FPGAVI_IndicatorU32_SlowScanLoop_Duration_ticks = 0x819C,
} NiFpga_9159FPGAVI_IndicatorU32;

typedef enum
{
   NiFpga_9159FPGAVI_ControlBool_SlowIOScanLoop_Toggle = 0x8116,
   NiFpga_9159FPGAVI_ControlBool_Trigger_ArbFIFO_Bool = 0x8126,
} NiFpga_9159FPGAVI_ControlBool;

typedef enum
{
   NiFpga_9159FPGAVI_ControlU8_FastDO_U8 = 0x81A6,
} NiFpga_9159FPGAVI_ControlU8;

typedef enum
{
   NiFpga_9159FPGAVI_ControlI16_ArbFIFO_Read_Size_Samples = 0x8122,
} NiFpga_9159FPGAVI_ControlI16;

typedef enum
{
   NiFpga_9159FPGAVI_ControlU32_FastAILoop_Rate_us = 0x8180,
   NiFpga_9159FPGAVI_ControlU32_Mod10_U32 = 0x816C,
   NiFpga_9159FPGAVI_ControlU32_Mod11_U32 = 0x8168,
   NiFpga_9159FPGAVI_ControlU32_Mod7_U32 = 0x8174,
   NiFpga_9159FPGAVI_ControlU32_Mod9_U32 = 0x8170,
   NiFpga_9159FPGAVI_ControlU32_SlowDOLoop_Rate_us = 0x8178,
   NiFpga_9159FPGAVI_ControlU32_SlowScanLoop_Rate_us = 0x81A0,
} NiFpga_9159FPGAVI_ControlU32;

typedef enum
{
   NiFpga_9159FPGAVI_ControlU64_ArbFIFO_Sample_Value = 0x811C,
} NiFpga_9159FPGAVI_ControlU64;

typedef enum
{
   NiFpga_9159FPGAVI_TargetToHostFifoI32_TargetToHostI32FIFO = 0,
} NiFpga_9159FPGAVI_TargetToHostFifoI32;

typedef enum
{
   NiFpga_9159FPGAVI_TargetToHostFifoU64_TargetToHostArbU64FIFO = 1,
} NiFpga_9159FPGAVI_TargetToHostFifoU64;

#endif
