/**
 * @file
 *
 * @ingroup RTEMSImplClassicRateMonotonic
 *
 * @brief This source file contains the implementation of
 *   rtems_rate_monotonic_get_status().
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright (c) 2016 embedded brains GmbH.
 *  Copyright (c) 2017 Kuan-Hsun Chen.
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <rtems/rtems/ratemonimpl.h>

rtems_status_code rtems_rate_monotonic_get_status(
  rtems_id                            id,
  rtems_rate_monotonic_period_status *period_status
)
{
  Rate_monotonic_Control *the_period;
  ISR_lock_Context        lock_context;

  if ( period_status == NULL ) {
    return RTEMS_INVALID_ADDRESS;
  }

  the_period = _Rate_monotonic_Get( id, &lock_context );
  if ( the_period == NULL ) {
    return RTEMS_INVALID_ID;
  }

  _Rate_monotonic_Acquire_critical( the_period, &lock_context );

  period_status->owner = the_period->owner->Object.id;
  period_status->state = the_period->state;
  period_status->postponed_jobs_count = the_period->postponed_jobs;

  if ( the_period->state == RATE_MONOTONIC_INACTIVE ) {
    /*
     *  If the period is inactive, there is no information.
     */
    _Timespec_Set_to_zero( &period_status->since_last_period );
    _Timespec_Set_to_zero( &period_status->executed_since_last_period );
  } else {
    Timestamp_Control wall_since_last_period;
    Timestamp_Control cpu_since_last_period;

    /*
     *  Grab the current status.
     */
    _Rate_monotonic_Get_status(
      the_period,
      &wall_since_last_period,
      &cpu_since_last_period
    );
    _Timestamp_To_timespec(
      &wall_since_last_period,
      &period_status->since_last_period
    );
    _Timestamp_To_timespec(
      &cpu_since_last_period,
      &period_status->executed_since_last_period
    );
  }

  _Rate_monotonic_Release( the_period, &lock_context );
  return RTEMS_SUCCESSFUL;
}
