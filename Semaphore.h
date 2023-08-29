/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file Semaphore.h
 *  \brief Semaphore-abstraction library for use with XInC2 processor.
 *  Provides single semaphore interface for all targets.
 *  \details
 *  In the XInC2 and test harness cases, all function calls will be some variant
 *  of _inp or _outp. For other targets, we use POSIX.
 *  Although the XInC2 processor has 16 user-accessible semaphores, we will lock
 *  thread creation/destruction a dedicated leaving only 15 for users to use
 *  arbitrarily.
 */
#ifndef _SXC_SEMAPHORE_H_
#define _SXC_SEMAPHORE_H_

#include "SXC.h"
#include "register_consts.h"

static const size_t kNumSemaphores = 16;
static const size_t SEM_THRD = 15;

/**
 * \brief Locks the semaphore numbered "sem_num". Returns immediately if the
 * semaphore is unlocked. Performs a blocking wait on the semaphore if it is
 * already locked.
 * \param [in] sem_num semaphore to lock
 */
SXC_INLINE void sem_lock(size_t sem_num)
{
  safe_mode_assert(sem_num < kNumSemaphores);
  _outp(1 << sem_num, SCUdown);
  return;
}

/**
 * \brief Unlocks the semaphore numbered "sem_num". Always returns immediately.
 * Does nothing if the semaphore is already unlocked.
 * \param [in] sem_num semaphore to unlock
 */
SXC_INLINE void sem_unlock(size_t sem_num)
{
  safe_mode_assert(sem_num < kNumSemaphores);
  _outp(1 << sem_num, SCUup);
  return;
}

/**
 * \brief Unlocks all semaphores. It is recommended to never call this function.
 */
SXC_INLINE void sem_unlock_all(void)
{
  uint16_t i;
  for (i=0; i<kNumSemaphores; ++i)
    sem_unlock(i);
  return;
}

/**
 * \brief Performs an atomic assignment from src to dest by using a semaphore
 * lock. Current pseudo-replacement for Atomic class.
 * \param [in] dest Destination address where to write data to
 * \param [in] src Source address where to read data from
 * \param [in] num_bytes Number of 16-bit (NOT 8-bit!) "bytes" to copy
 * \param [in] sem Semaphore to lock on
 */
SXC_INLINE void atomic_copy(void* dest, void const * src, size_t num_bytes,
  size_t sem)
{
  sem_lock(sem);
  size_t i;
  for (i=0; i<num_bytes; ++i)
    *((uint16_t*)dest+i) = *((uint16_t*)src+i);
  sem_unlock(sem);
}

#endif //_SXC_SEMAPHORE_H_
