/**
 * @file
 *
 * @brief CPU Port Implementation API
 */

/*
 * Copyright (c) 2013 embedded brains GmbH
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_CPUIMPL_H
#define _RTEMS_SCORE_CPUIMPL_H

#include <rtems/score/cpu.h>

/**
 * @defgroup RTEMSScoreCPUm68k Motorola 68000 and NXP ColdFire (m68k)
 *
 * @ingroup RTEMSScoreCPU
 *
 * @brief Motorola 68000 and NXP ColdFire (m68k) Architecture Support
 *
 * @{
 */

#define CPU_PER_CPU_CONTROL_SIZE 0

#ifndef ASM

#ifdef __cplusplus
extern "C" {
#endif

RTEMS_NO_RETURN void _CPU_Fatal_halt( uint32_t source, CPU_Uint32ptr error );

RTEMS_INLINE_ROUTINE void _CPU_Context_volatile_clobber( uintptr_t pattern )
{
  /* TODO */
}

RTEMS_INLINE_ROUTINE void _CPU_Context_validate( uintptr_t pattern )
{
  while (1) {
    /* TODO */
  }
}

RTEMS_INLINE_ROUTINE void _CPU_Instruction_illegal( void )
{
  __asm__ volatile ( "illegal" );
}

RTEMS_INLINE_ROUTINE void _CPU_Instruction_no_operation( void )
{
  __asm__ volatile ( "nop" );
}

#ifdef __cplusplus
}
#endif

#endif /* ASM */

/** @} */

#endif /* _RTEMS_SCORE_CPUIMPL_H */
