/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file Timer.h
 *  \brief Interface for accessing TimerA and TimerB functions on the XInC2
 *  processor. NOT thread-safe; timers do not have internal semaphores in
 *  hardware and the user MUST manually manage semaphores to protect their state.
 */
#ifndef _SXC_LIB_TIMER_H_
#define _SXC_LIB_TIMER_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup timer_names
 * List of available timers
 * @{
 */
enum timer_name {
  TIMER_A = 0,
  TIMER_B = 1,
  kNumTimers
};
/** @} */

static const size_t TIMER_A_IO_PINS = 4;
static const size_t TIMER_B_IO_PINS = 1;
static const size_t TIMER_A_NUM_CCMS = 4;
static const size_t TIMER_B_NUM_CCMS = 2;

/**
 * \defgroup timer_config_flags
 * List of bitwise-ORable flags to set up a timer
 * @{
 */
enum timer_config_flag {
  TIMER_ENABLE = 0x1,
  TIMER_MODE_HALT = 0x0,
  TIMER_MODE_UP = 0x1 << 1, //count up to CR0, restart at 0
  TIMER_MODE_CONT = 0x1 << 2, //count up to 0xFFFF, restart at 0
  TIMER_MODE_UPDOWN = 0x3 << 1, //count up to 0xFFFF, count down to 0, repeat
  TIMER_CLOCK_RATE_2 = 0x0 << 3,
  TIMER_CLOCK_RATE_4 = 0x1 << 3,
  TIMER_CLOCK_RATE_8 = 0x2 << 3,
  TIMER_CLOCK_RATE_16 = 0x3 << 3,
  TIMER_CLOCK_RATE_32 = 0x4 << 3,
  TIMER_CLOCK_RATE_64 = 0x5 << 3,
  TIMER_CLOCK_RATE_128 = 0x6 << 3,
  TIMER_CLOCK_RATE_256 = 0x7 << 3,
  TIMER_CLOCK_RATE_512 = 0x8 << 3,
  TIMER_CLOCK_RATE_1024 = 0x9 << 3,
  TIMER_CLOCK_RATE_2048 = 0xA << 3,
  TIMER_CLOCK_RATE_4096 = 0xB << 3,
  TIMER_BLOCK_ON_READCFG = 0x1 << 7,
  TIMER_COUNTER_OVERFLOW_BIT = 0x1 << 15,
};
/** @} */

/**
 * \defgroup timer_ccm_config_flags
 * List of bitwise-ORable flags to set up a timer's capture/compare module
 * @{
 */
enum timer_ccm_config_flag {
  TIMER_CCM_COMP_NOT_CAP = 0x1 << 1,
  TIMER_CCM_CMP_OUT_EQ_1 = 0x1 << 2,
  TIMER_CCM_OUT_ENABLE = 0x1 << 3,
  TIMER_CCM_OUTPUT_OUT = 0x0,
  TIMER_CCM_OUTPUT_SET = 0x1 << 4,
  TIMER_CCM_OUTPUT_RESET = 0x2 << 4,
  TIMER_CCM_OUTPUT_TOGGLE = 0x3 << 4,
  TIMER_CCM_OUTPUT_TOGGLE_SET = 0x4 << 4,
  TIMER_CCM_OUTPUT_TOGGLE_RESET = 0x5 << 4,
  TIMER_CCM_OUTPUT_SET_RESET = 0x6 << 4,
  TIMER_CCM_OUTPUT_RESET_SET = 0x7 << 4,
  TIMER_CCM_INPUT_DISABLE = 0x0,
  TIMER_CCM_INPUT_INPIN = 0x1 << 7,
  TIMER_CCM_INPUT_0 = 0x2 << 7,
  TIMER_CCM_INPUT_1 = 0x3 << 7,
  TIMER_CCM_CAP_DISABLE = 0x0,
  TIMER_CCM_CAP_RISING = 0x1 << 9,
  TIMER_CCM_CAP_FALLING = 0x2 << 9,
  TIMER_CCM_CAP_EITHER = 0x3 << 9,
  TIMER_CCM_BLOCK_ON_READ = 0x1 << 11,
};
/** @} */

/**
 * \brief Configures the selected timer to behave according to the provided
 * configuration. See XInC2 manual for details.
 * \param[in] cfg Bitwise-ORed flags to configure the timer.
 * \param[in] timer The timer to configure.
 */
SXC_INLINE void timer_set_config(uint16_t cfg, enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A) {
    _outp(cfg, TMRAcfg);
  }
  else if (timer == TIMER_B) {
    _outp(cfg, TMRBcfg);
  }
}

/**
 * \brief Returns the selected timer's current configuration.
 * \param[in] timer The timer to get the configuration of.
 * \return The timer's configuration at time of function call.
 */
SXC_INLINE uint16_t timer_get_config(enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A)
    return _inp(TMRAcfg);
  else if (timer == TIMER_B)
    return _inp(TMRBcfg);
  return 0;
}

/**
 * \brief Configures the selected timer CCM to behave according to the provided
 * configuration. See XInC2 manual for details.
 * \param[in] cfg Bitwise-ORed flags to configure the timer's CCM.
 * \param[in] timer The timer to target.
 * \param[in] ccm The CCM to configure.
 */
SXC_INLINE void timer_set_config_ccm(uint16_t cfg, uint16_t ccm,
                                                          enum timer_name timer)
{
  if (timer == TIMER_A) {
    safe_mode_assert(ccm < TIMER_A_NUM_CCMS);
    switch(ccm) {
    case 0:
      _outp(cfg, TMRACCM0cfg);
      break;
    case 1:
      _outp(cfg, TMRACCM1cfg);
      break;
    case 2:
      _outp(cfg, TMRACCM2cfg);
      break;
    case 3:
      _outp(cfg, TMRACCM3cfg);
      break;
    default:
      break;
    }
  }
  else if (timer == TIMER_B) {
    safe_mode_assert(ccm < TIMER_B_NUM_CCMS);
    switch(ccm) {
    case 0:
      _outp(cfg, TMRBCCM0cfg);
      break;
    case 1:
      _outp(cfg, TMRBCCM1cfg);
      break;
    default:
      break;
    }
  }
}

/**
 * \brief Returns the selected timer CCM's current configuration.
 * \param[in] timer The timer whose CCMs to check.
 * \param[in] ccm The CCM to target.
 * \return The timer CCM's configuration at time of function call.
 */
SXC_INLINE uint16_t timer_get_config_ccm(uint16_t ccm, enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A) {
    safe_mode_assert(ccm < TIMER_A_NUM_CCMS);
    switch(ccm) {
    case 0:
      return _inp(TMRACCM0cfg);

    case 1:
      return _inp(TMRACCM1cfg);

    case 2:
      return _inp(TMRACCM2cfg);

    case 3:
      return _inp(TMRACCM3cfg);

    default:
      return 0;
    }
  }
  else if (timer == TIMER_B) {
    safe_mode_assert(ccm < TIMER_B_NUM_CCMS);
    switch(ccm) {
    case 0:
      return _inp(TMRBCCM0cfg);

    case 1:
      return _inp(TMRBCCM1cfg);

    default:
      return 0;
    }
  }
  return 0;
}
/**
 * \brief Reads the specified timer's value at the time of function call.
 * \param[in] timer The timer to target.
 * \return The timer's value when the function is called. Returns 0 if something
 * has gone awry, which unfortunately can also mean that the timer value was just
 * 0.
 */
SXC_INLINE uint16_t timer_read(enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A)
    return _inp(TMRAcount);
  else if (timer == TIMER_B)
    return _inp(TMRBcount);
  return 0;
}

/**
 * \brief Reads the specified timer capture/compare register value at the time
 * of function call.
 * \param[in] timer The timer to target.
 * \param[in] ccm The CCR to target.
 * \return The CCR's value at the time of function call. This is the timer value
 * latched on a capture event in capture mode, and the compare register value in
 * compare mode. Returns 0 if something has gone awry, which unfortunately can
 * also mean that the CCR value was just 0.
 */
SXC_INLINE uint16_t timer_read_CCR(uint16_t ccm, enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A) {
    safe_mode_assert(ccm < TIMER_A_NUM_CCMS);
    switch(ccm) {
    case 0:
      return _inp(TMRACCR0);
    case 1:
      return _inp(TMRACCR1);
    case 2:
      return _inp(TMRACCR2);
    case 3:
      return _inp(TMRACCR3);
    default:
      return 0;
    }
  }
  else if (timer == TIMER_B) {
    safe_mode_assert(ccm < TIMER_B_NUM_CCMS);
    switch(ccm) {
    case 0:
      return _inp(TMRBCCR0);
    case 1:
      return _inp(TMRBCCR1);
    default:
      return 0;
    }
  }
  return 0;
}

/**
 * \brief Writes a value to the specified timer capture/compare register. The
 * manual says these registers are read-only: this is a filthy lie. God help the
 * newbies.
 * \param[in] input Arbitrary 16-bit integer to write.
 * \param[in] ccm The CCR to target.
 * \param[in] timer The timer to target.
 */
SXC_INLINE void timer_write_CCR(uint16_t input, uint16_t ccm, enum timer_name timer)
{
  safe_mode_assert(timer < kNumTimers);
  if (timer == TIMER_A) {
    safe_mode_assert(ccm < TIMER_A_NUM_CCMS);
    switch(ccm) {
    case 0:
      _outp(input, TMRACCR0);
      return;
    case 1:
      _outp(input, TMRACCR1);
      return;
    case 2:
      _outp(input, TMRACCR2);
      return;
    case 3:
      _outp(input, TMRACCR3);
      return;
    default:
      return;
    }
  }
  else if (timer == TIMER_B) {
    safe_mode_assert(ccm < TIMER_B_NUM_CCMS);
    switch(ccm) {
    case 0:
      _outp(input, TMRBCCR0);
      return;
    case 1:
      _outp(input, TMRBCCR1);
      return;
    default:
      return;
    }
  }
}

#endif //SXC_LIB_TIMER_H_
