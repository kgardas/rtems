/**
 * @file
 *
 * @ingroup RTEMSImplClassicSignal
 *
 * @brief This header file provides data structures used by the implementation
 *   and the @ref RTEMSImplApplConfig to ultimately define
 *   ::Thread_Configured_control.
 */

/* COPYRIGHT (c) 1989-2013.
 * On-Line Applications Research Corporation (OAR).
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_RTEMS_ASRDATA_H
#define _RTEMS_RTEMS_ASRDATA_H

#include <rtems/rtems/asr.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup RTEMSImplClassicSignal
 *
 * @{
 */

/**
 *  The following defines the control structure used to manage
 *  signals.  Each thread has a copy of this record.
 */
typedef struct {
  /** This field indicates if are ASRs enabled currently. */
  bool              is_enabled;
  /** This field indicates if address of the signal handler function. */
  rtems_asr_entry   handler;
  /** This field indicates if the task mode the signal will run with. */
  rtems_mode        mode_set;
  /** This field indicates the signal set that is pending. */
  rtems_signal_set  signals_pending;
}   ASR_Information;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
