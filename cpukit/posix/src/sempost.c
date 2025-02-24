/**
 *  @file
 *
 *  @brief Unlock a Semaphore
 *  @ingroup POSIX_SEMAPHORE
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

#include <rtems/posix/semaphoreimpl.h>

#include <limits.h>

int sem_post( sem_t *_sem )
{
  Sem_Control          *sem;
  ISR_Level             level;
  Thread_queue_Context  queue_context;
  Thread_queue_Heads   *heads;
  unsigned int          count;

  POSIX_SEMAPHORE_VALIDATE_OBJECT( _sem );

  sem = _Sem_Get( &_sem->_Semaphore );
  _Thread_queue_Context_initialize( &queue_context );
  _Thread_queue_Context_ISR_disable( &queue_context, level );
  _Sem_Queue_acquire_critical( sem, &queue_context );

  heads = sem->Queue.Queue.heads;
  count = sem->count;

  if ( RTEMS_PREDICT_TRUE( heads == NULL && count < SEM_VALUE_MAX ) ) {
    sem->count = count + 1;
    _Sem_Queue_release( sem, level, &queue_context );
    return 0;
  }

  if ( RTEMS_PREDICT_TRUE( heads != NULL ) ) {
    _Thread_queue_Context_set_ISR_level( &queue_context, level );
    _Thread_queue_Surrender_no_priority(
      &sem->Queue.Queue,
      heads,
      &queue_context,
      SEMAPHORE_TQ_OPERATIONS
    );
    return 0;
  }

  _Sem_Queue_release( sem, level, &queue_context );
  rtems_set_errno_and_return_minus_one( EOVERFLOW );
}
