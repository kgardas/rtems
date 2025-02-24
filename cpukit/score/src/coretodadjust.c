/**
 * @file
 *
 * @ingroup RTEMSScoreTOD
 *
 * @brief This source file contains the implementation of
 *   _TOD_Adjust().
 */

/*
 *  COPYRIGHT (c) 1989-2014.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/score/todimpl.h>

Status_Control _TOD_Adjust(
  const struct timespec *delta
)
{
  ISR_lock_Context lock_context;
  struct timespec  tod;
  Status_Control   status;

  /*
   * Currently, RTEMS does the adjustment in one movement.
   * Given interest, requirements, and sponsorship, a future
   * enhancement would be to adjust the time in smaller increments
   * at each clock tick. Until then, there is no outstanding
   * adjustment.
   */

  _TOD_Lock();
  _TOD_Acquire( &lock_context );
  _TOD_Get( &tod );
  _Timespec_Add_to( &tod, delta );
  status = _TOD_Is_valid_new_time_of_day( &tod );

  if ( status == STATUS_SUCCESSFUL ) {
    status = _TOD_Set( &tod, &lock_context );
  }

  _TOD_Unlock();

  return status;
}
