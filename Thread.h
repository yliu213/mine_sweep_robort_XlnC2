/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file Thread.h
 *  \brief Provides a convenient interface for enabling multithreading on the
 *  XInC2. Thread-safe, but all threads are equal peers and may terminate
 *  each other's execution.
 *  Uses POSIX under-the-hood for other platforms: same interface as XInC2.
 */
#ifndef _SXC_LIB_THREAD_H_
#define _SXC_LIB_THREAD_H_

#include "Semaphore.h"
#include "SFU.h"

static const size_t kNumThrds = 8;

extern uint16_t thrds_vector;

static const size_t THREAD_PC_SHIFT = 3;
static const uint16_t THREAD_R2 = 2;

/**
 * \brief Starts running the selected thread.
 * No effect if the thread is already running.
 * \param[in] thrd_num 3-bit integer representing which thread to run.
 */
SXC_INLINE void thread_run(uint16_t thrd_num)
{
  safe_mode_assert(thrd_num < kNumThrds);
  sem_lock(SEM_THRD);
  thrds_vector &= ~(1 << thrd_num);
  _outp(thrds_vector, SCUstop);
  sem_unlock(SEM_THRD);
}

/**
 * \brief Stops the selected thread.
 * No effect if thread is already stopped.
 * \param[in] thrd_num 3-bit integer representing which thread to stop.
 */
SXC_INLINE void thread_stop(uint16_t thrd_num)
{
  safe_mode_assert(thrd_num < kNumThrds);
  sem_lock(SEM_THRD);
  thrds_vector |= (1 << thrd_num);
  _outp(thrds_vector, SCUstop);
  sem_unlock(SEM_THRD);
}

/**
 * \brief Stops/starts the execution of the selected threads. Multiple threads
 * may be affected simultaneously. Setting bitN stops threadN, while clearing it
 * starts/continues running the thread. It is not recommended to call this
 * function, unless one knows the run status of all threads and preserves the
 * status of threads which are not intended to be run or stopped by a call to
 * this function.
 * \param[in] thrds Lower byte: set bits correspond to threads to stop, cleared
 * bits correspond to threads to run. Upper byte is irrelevant.
 */
SXC_INLINE void thread_stoprun_multiple(uint16_t thrds)
{
  sem_lock(SEM_THRD);
  thrds_vector = thrds;
  _outp(thrds_vector, SCUstop);
  sem_unlock(SEM_THRD);
}

/**
 * \brief Returns the bit sequence indicating which I/O page every thread is
 * using at the time of function call.
 * \return The 8-bit sequence representing which I/O page each thread is using.
 * ex. if bit0 is 1, thread0 is using page1
 */
SXC_INLINE uint16_t thread_getpage(void)
{
  return _inp(SCUiopageRD);
  return 0;
}

/**
 * \brief Sets up initial arguments and initial function to execute in the
 * selected thread. Does NOT actually start the thread. Behaviour is undefined
 * if the thread is already executing.
 * \param[in] func Function pointer to the input function. Function may have no
 * return value and no inputs, or pointers for either or both.
 * \param[in] ptr Pointer to input arguments to provide to thread function.
 * \param[in] thrd_num The thread to set up.
 */
SXC_INLINE void thread_setup(void* (*func)(void*), void* ptr, uint16_t thrd_num)
{
  safe_mode_assert(thrd_num < kNumThrds);
  sem_lock(SEM_THRD);
  _outp((thrd_num << THREAD_PC_SHIFT)|THREAD_R2, SCUpntr);
  _outp((uint16_t)ptr, SCUreg);
  _outp((uint16_t)func, SCUpc);
  sem_unlock(SEM_THRD);
}

#endif // _SXC_LIB_THREAD_H_
