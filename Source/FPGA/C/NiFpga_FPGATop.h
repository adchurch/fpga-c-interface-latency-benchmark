/*
 * Generated with the FPGA Interface C API Generator 16.0.0
 * for NI-RIO 16.0.0 or later.
 */

#ifndef __NiFpga_FPGATop_h__
#define __NiFpga_FPGATop_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1600
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_FPGATop_Bitfile;
 */
#define NiFpga_FPGATop_Bitfile "NiFpga_FPGATop.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_FPGATop_Signature = "FD0CB3A8660262B863550775C3E31643";

typedef enum
{
   NiFpga_FPGATop_IndicatorBool_bool_out = 0x18012,
} NiFpga_FPGATop_IndicatorBool;

typedef enum
{
   NiFpga_FPGATop_IndicatorU32_int_out = 0x18008,
} NiFpga_FPGATop_IndicatorU32;

typedef enum
{
   NiFpga_FPGATop_IndicatorSgl_sgl_out = 0x18004,
} NiFpga_FPGATop_IndicatorSgl;

typedef enum
{
   NiFpga_FPGATop_ControlBool_bool_in = 0x18016,
} NiFpga_FPGATop_ControlBool;

typedef enum
{
   NiFpga_FPGATop_ControlU32_int_in = 0x1800C,
} NiFpga_FPGATop_ControlU32;

typedef enum
{
   NiFpga_FPGATop_ControlSgl_sgl_in = 0x18000,
} NiFpga_FPGATop_ControlSgl;

typedef enum
{
   NiFpga_FPGATop_TargetToHostFifoU64_ReturnFIFO = 1,
} NiFpga_FPGATop_TargetToHostFifoU64;

typedef enum
{
   NiFpga_FPGATop_HostToTargetFifoU64_SourceFIFO = 0,
} NiFpga_FPGATop_HostToTargetFifoU64;

#endif
