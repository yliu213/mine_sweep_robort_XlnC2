/*                  Copyright Eleven Engineering 2014                         */
/* Author: Sam Cristall
 * License: GPLv3
 */
/** \file DASI.h
 *  \brief Interface for accessing digital audio serial interface-related
 *  registers.
 */

#ifndef _SXC_LIB_ACCUMULATOR_H_
#define _SXC_LIB_ACCUMULATOR_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup timer_names
 * List of available timers
 * @{
 */
enum accumulator_name {
  ACCUMULATOR_0 = 0,
  ACCUMULATOR_1 = 1,
  kNumAccumulators
};
/** @} */

/**
 * \defgroup accumulator_config_flags
 * List of bitwise-ORable flags to set up an accumulator
 * @{
 */
enum accumulator_config_flag {
  ACCUMULATOR_SATURATED_MODE_BIT = 0x1 << 0,
  ACCUMULATOR_SIGNED_16_MODE_BIT = 0x1 << 1,
  ACCUMULATOR_CLEAR_BIT = 0x1 << 2,
  ACCUMULATOR_SIGNED_OVERFLOW_BIT = 0x1 << 13,
  ACCUMULATOR_OVERFLOW_BIT = 0x1 << 14,
  ACCUMULATOR_STICKY_OVERFLOW_BIT = 0x1 << 15,
};
/** @} */

/**
 * \brief Configures the selected accumulator to behave according to the
 * provided configuration. See XInC2 manual for details.
 * \param[in] cfg Bitwise-ORed flags to configure the accumulator.
 * \param[in] accum The accumulator to configure.
 */
SXC_INLINE void accumulator_set_config(uint16_t cfg,
                                       enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    _outp(cfg, Accum0cfg);
  } else if (accum == ACCUMULATOR_1) {
    _outp(cfg, Accum1cfg);
  }
}

/**
 * \brief Returns the selected accumulator's current configuration.
 * \param[in] accumulator The accumulator to get the configuration of.
 * \return The accumulator's configuration at time of function call.
 */
SXC_INLINE uint16_t accumulator_get_config(enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    return _inp(Accum0cfg);
  } else if (accum == ACCUMULATOR_1) {
    return _inp(Accum1cfg);
  }
  return 0;
}

/**
 * \brief Writes to the most significant word of the accumulator
 * \param[in] value The value to write to the most significant word
 * \param[in] accum The accumulator to write to
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE void accumulator_set_high(uint16_t value,
                                     enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    _outp(value, Accum0high);
  } else if (accum == ACCUMULATOR_1) {
    _outp(value, Accum1high);
  }
}

/**
 * \brief Writes to the least significant word of the accumulator
 * \param[in] value The value to write to the least significant word
 * \param[in] accum The accumulator to write to
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE void accumulator_set_low(uint16_t value,
                                    enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    _outp(value, Accum0low);
  } else if (accum == ACCUMULATOR_1) {
    _outp(value, Accum1low);
  }
}

/**
 * \brief Gets the most significant word of the accumulator
 * \param[in] accum The accumulator to read from
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE uint16_t accumulator_get_high(enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    return _inp(Accum0high);
  } else if (accum == ACCUMULATOR_1) {
    return _inp(Accum1high);
  }
  return 0;
}

/**
 * \brief Gets the least significant word of the accumulator
 * \param[in] accum The accumulator to read from
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE uint16_t accumulator_get_low(enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    return _inp(Accum0low);
  } else if (accum == ACCUMULATOR_1) {
    return _inp(Accum1low);
  }
  return 0;
}

/**
 * \brief Adds a 32-bit number to the current value in the accumulator
 * \param[in] msw The msw of the 32-bit number to add to the accumulator
 * \param[in] lsw The lsw of the 32-bit number to add to the accumulator
 * \param[in] accum The accumulator to write to
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE void accumulator_add(uint16_t msw, uint16_t lsw,
                                enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    _outp(msw, Accum0addsubhigh);
    _outp(lsw, Accum0addlow);
  } else if (accum == ACCUMULATOR_1) {
    _outp(msw, Accum1addsubhigh);
    _outp(lsw, Accum1addlow);
  }
}

/**
 * \brief Subtracts a 32-bit number to the current value in the accumulator
 * \param[in] msw The msw of the 32-bit number to subtract from the accumulator
 * \param[in] lsw The lsw of the 32-bit number to subtract from the accumulator
 * \param[in] accum The accumulator to write to
 * \note Will block until any prior accumulation is complete.
 */
SXC_INLINE void accumulator_sub(uint16_t msw, uint16_t lsw,
                                enum accumulator_name accum) {
  safe_mode_assert(accum < kNumAccumulators);
  if (accum == ACCUMULATOR_0) {
    _outp(msw, Accum0addsubhigh);
    _outp(lsw, Accum0sublow);
  } else if (accum == ACCUMULATOR_1) {
    _outp(msw, Accum1addsubhigh);
    _outp(lsw, Accum1sublow);
  }
}

#endif

