/**
 * @file
 *
 * @ingroup RTEMSScoreThreadMP
 *
 * @brief This header file provides the interfaces of the
 *   @ref RTEMSScoreThreadMP.
 */

/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_THREADMP_H
#define _RTEMS_SCORE_THREADMP_H

#ifndef _RTEMS_SCORE_THREADIMPL_H
# error "Never use <rtems/score/threadmp.h> directly; include <rtems/score/threadimpl.h> instead."
#endif

#include <rtems/score/mpciimpl.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup RTEMSScoreThreadMP Thread Handler Multiprocessing (MP) Support
 *
 * @ingroup RTEMSScoreThread
 *
 * @brief This group contains the implementation to support the Thread Handler
 *   in multiprocessing (MP) configurations.
 *
 * This handler encapsulates functionality which is related to managing
 * threads in a multiprocessor system configuration.  This handler must
 * manage proxies which represent remote threads blocking on local
 * operations.
 *
 * @{
 */

/**
 * @brief Initialize MP thread handler.
 *
 * This routine initializes the multiprocessing portion of the Thread Handler.
 *
 * @param maximum_proxies The maximum number of proxies for the MP thread handler.
 */
void _Thread_MP_Handler_initialization (
  uint32_t   maximum_proxies
);

/**
 * @brief Allocates a MP proxy control block from
 * the inactive chain of free proxy control blocks.
 *
 * This  allocates a proxy control block from
 * the inactive chain of free proxy control blocks.
 *
 * @note This function returns a thread control pointer
 *       because proxies are substitutes for remote threads.
 *
 * * @param the_state The state for the allocated MP proxy control block.
 */
Thread_Control *_Thread_MP_Allocate_proxy (
  States_Control the_state
);

/**
 * @brief Removes the MP proxy control block for the specified
 * id from the active chain of proxy control blocks.
 *
 * This function removes the proxy control block for the specified
 * id from the active red-black tree of proxy control blocks.
 *
 * @param the_id The id of the proxy control block to remove.
 *
 * @return The removed proxy control block.
 */
Thread_Control *_Thread_MP_Find_proxy (
  Objects_Id the_id
);

/**
 * This function returns true if the thread in question is the
 * multiprocessing receive thread.
 *
 * @note This is a macro to avoid needing a prototype for
 *       _MPCI_Receive_server_tcb until it is used.
 */
#define _Thread_MP_Is_receive(_the_thread) \
  ((_the_thread) == _MPCI_Receive_server_tcb)

/**
 * @brief Extracts the proxy of the thread if necessary.
 *
 * This routine ensures that if there is a proxy for this thread on another
 * node, it is also dealt with. A proxy is a data that is on the thread queue
 * on the remote node and acts as a proxy for the local thread. If the local
 * thread was waiting on a remote operation, then the remote side of the
 * operation must be cleaned up.
 *
 * @param[in, out] the_thread is the thread to determine the proxy.
 */
void _Thread_MP_Extract_proxy( Thread_Control *the_thread );

/**
 * @brief Trees a proxy control block to the inactive chain of free proxy
 *      control blocks.
 */
void _Thread_MP_Free_proxy( Thread_Control *the_thread );

/**
 * @brief Checks if the thread MP with this object id is remote.
 *
 * @param id The object id.
 *
 * @retval true The object id is valid and the thread MP with this object id
 *      is remote.
 * @retval false The object if is not valid or the thread MP with this object
 *      id is not remote.
 */
RTEMS_INLINE_ROUTINE bool _Thread_MP_Is_remote( Objects_Id id )
{
  Objects_Information *information;

  information = _Thread_Get_objects_information_by_id( id );
  if ( information == NULL ) {
    return false;
  }

  return _Objects_MP_Is_remote( id, information );
}

/** @} */

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
