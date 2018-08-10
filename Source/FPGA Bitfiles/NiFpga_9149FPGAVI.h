/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_9149FPGAVI_h__
#define __NiFpga_9149FPGAVI_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_9149FPGAVI_Bitfile;
 */
#define NiFpga_9149FPGAVI_Bitfile "NiFpga_9149FPGAVI.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_9149FPGAVI_Signature = "D7CF3E694CD61A44318DC7DA53A5CDBA";

typedef enum
{
   NiFpga_9149FPGAVI_IndicatorBool_SlowScanLoopTimedOut = 0x18066,
   NiFpga_9149FPGAVI_IndicatorBool_TargetToHostLoopTimedOut = 0x1803A,
} NiFpga_9149FPGAVI_IndicatorBool;

typedef enum
{
   NiFpga_9149FPGAVI_IndicatorU8_FastDI_Mod8_U8 = 0x18072,
} NiFpga_9149FPGAVI_IndicatorU8;

typedef enum
{
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI0_I32 = 0x18060,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI10_I32 = 0x18018,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI11_I32 = 0x18014,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI12_I32 = 0x18010,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI13_I32 = 0x1800C,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI14_I32 = 0x18008,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI15_I32 = 0x18004,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI1_I32 = 0x1805C,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI2_I32 = 0x18058,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI3_I32 = 0x18054,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI4_I32 = 0x18030,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI5_I32 = 0x1802C,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI6_I32 = 0x18028,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI7_I32 = 0x18024,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI8_I32 = 0x18020,
   NiFpga_9149FPGAVI_IndicatorI32_Mod3_AI9_I32 = 0x1801C,
   NiFpga_9149FPGAVI_IndicatorI32_TargetToHostLoopIterCount = 0x18034,
} NiFpga_9149FPGAVI_IndicatorI32;

typedef enum
{
   NiFpga_9149FPGAVI_IndicatorU32_FastAILoop_Duration_ticks = 0x18050,
   NiFpga_9149FPGAVI_IndicatorU32_Mod6_U32 = 0x1803C,
   NiFpga_9149FPGAVI_IndicatorU32_SlowDOLoop_Duration_ticks = 0x18048,
   NiFpga_9149FPGAVI_IndicatorU32_SlowScanLoop_Duration_ticks = 0x18068,
} NiFpga_9149FPGAVI_IndicatorU32;

typedef enum
{
   NiFpga_9149FPGAVI_ControlU8_FastDO_Mod5_U8 = 0x1806E,
} NiFpga_9149FPGAVI_ControlU8;

typedef enum
{
   NiFpga_9149FPGAVI_ControlU32_FastAILoop_Rate_us = 0x1804C,
   NiFpga_9149FPGAVI_ControlU32_Mod1_U32 = 0x18040,
   NiFpga_9149FPGAVI_ControlU32_SlowDOLoop_Rate_us = 0x18044,
   NiFpga_9149FPGAVI_ControlU32_SlowScanLoop_Rate_us = 0x18000,
} NiFpga_9149FPGAVI_ControlU32;

typedef enum
{
   NiFpga_9149FPGAVI_TargetToHostFifoI32_TargetToHostU32FIFO = 0,
} NiFpga_9149FPGAVI_TargetToHostFifoI32;

#endif
