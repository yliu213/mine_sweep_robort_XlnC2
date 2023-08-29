/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file LFSR.h
 *  \brief Interface for accessing the linear feedback shift registers on the
 *  XInC2 processor. These functions are NOT thread-safe: manual semaphore
 *  management is required. See the XInC2 manual for hardware layout for a much
 *  better understanding.
 */
#ifndef _SXC_LIB_LFSR_H_
#define _SXC_LIB_LFSR_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup LFSR_names
 * List of available linear feedback shift registers
 * @{
 */
enum LFSR_name {
  LFSR_NAME_0 = 0,
  LFSR_NAME_1 = 1,
  kNumLFSRs
};
/** @} */

/**
 * \defgroup LFSR_access_mode
 * List of modes on how to read/write from/to the LFSR
 * @{
 */
enum LFSR_access_mode {
  LFSR_ACCESS_ALL = 0,
  LFSR_ACCESS_SEQUENTIAL = 1,
  kNumLFSRAccessModes
};
/** @} */

/**
 * \defgroup LFSR_feedback_modes
 * List of LFSR feedback modes
 */
enum LFSR_fb_mode {
  LFSR_FBMODE_ENCODE = 0x0,
  LFSR_FBMODE_DECODE = 0x1,
  kNumLFSRFeedbackModes
};
/** @} */

/**
 * \brief Configures the selected LFSR to behave according to the provided
 * configuration. See XInC2 manual for details.
 * \param[in] lfsr_msb 4-bit integer representing the MSB of the LFSR. Also
 * equal to desired LFSR length-1. Bits beyond bit3 are ignored.
 * \param[in] data_msb 4-bit integer representing the MSB of the data to shift
 * in. Also equal to desired data length-1. Bits beyond bit3 are ignored.
 * \param[in] fb_mode The LFSR feedback mode to use.
 * \param[in] tap The LFSR tap configuration to use (see hardware config)
 * \param[in] lfsr The LFSR to configure.
 */
SXC_INLINE void lfsr_set_config(uint16_t lfsr_msb, uint16_t data_msb,
                        enum LFSR_fb_mode fb, uint16_t tap, enum LFSR_name lfsr)
{
  safe_mode_assert(lfsr < kNumLFSRs);
  uint16_t cfg = (lfsr_msb & 0xF) | ((data_msb & 0xF) << 4) | (fb << 8);
  if (lfsr == LFSR_NAME_0) {
    _outp(cfg, LFSR0cfg);
    _outp(tap, LFSR0tapcfg);
  }
  else if (lfsr == LFSR_NAME_1) {
    _outp(cfg, LFSR1cfg);
    _outp(tap, LFSR1tapcfg);
  }
}

/**
 * \brief Returns either the selected LFSR's configuration, or its tap config.
 * User selects which to return.
 * \param[in] tap_flag if NOT 0, function returns the LFSR's tap config, rather
 * than the LFSR's config.
 * \param[in] lfsr The LFSR to target.
 * \return The selected LFSR's configuration if tap_flag is 0, its tap config
 * otherwise. Returns 0 if an invalid LFSR is selected.
 */
SXC_INLINE uint16_t lfsr_get_config(uint16_t tap_flag, enum LFSR_name lfsr)
{
  safe_mode_assert(lfsr < kNumLFSRs);
  if (lfsr == LFSR_NAME_0) {
    if (tap_flag)
      return _inp(LFSR0tapcfg);
    return _inp(LFSR0cfg);
  }
  else if (lfsr == LFSR_NAME_1) {
    if (tap_flag)
      return _inp(LFSR1tapcfg);
    return _inp(LFSR1cfg);
  }
  return 0;
}

/**
 * \brief Reads the value stored/being shifted out in the selected LFSR,
 * depending on the desired access mode.
 * \param[in] mode Flag specifying whether to read all of the bits in the
 * chosen LFSR, or just the ones being sequentially shifted out.
 * \param[in] lfsr The LFSR to target.
 * \return The selected LFSR stored value in the selected access mode. Returns
 * 0 for an invalid LFSR or an invalid access mode.
 */
SXC_INLINE uint16_t lfsr_read(enum LFSR_access_mode mode, enum LFSR_name lfsr)
{
  safe_mode_assert(lfsr < kNumLFSRs);
  safe_mode_assert(mode < kNumLFSRAccessModes);
  if (lfsr == LFSR_NAME_0) {
    if (mode == LFSR_ACCESS_ALL)
      return _inp(LFSR0reg);
    else if (mode == LFSR_ACCESS_SEQUENTIAL)
      return _inp(LFSR0data);
  }
  else if (lfsr == LFSR_NAME_1) {
    if (mode == LFSR_ACCESS_ALL)
      return _inp(LFSR1reg);
    else if (mode == LFSR_ACCESS_SEQUENTIAL)
      return _inp(LFSR1data);
  }
  return 0;
}

/**
 * \brief Writes the supplied value to the selected LFSR, in the desired mode.
 * See the XInC2 manual for further details.
 * \details
 * Nota bene: if writing data sequentially, it is written MSB first and must be
 * left-aligned.
 * \param[in] input arbitrary 16-bit integer
 * \param[in] mode Flag specifying whether to write all of the input bits
 * to the chosen LFSR, or to shift in.
 * \param[in] lfsr The LFSR to target.
 */
SXC_INLINE void lfsr_write(uint16_t input, enum LFSR_access_mode mode, enum LFSR_name lfsr)
{
  safe_mode_assert(lfsr < kNumLFSRs);
  safe_mode_assert(mode < kNumLFSRAccessModes);
  if (lfsr == LFSR_NAME_0) {
    if (mode == LFSR_ACCESS_ALL)
      _outp(input, LFSR0reg);
    else if (mode == LFSR_ACCESS_SEQUENTIAL)
      _outp(input, LFSR0data);
  }
  else if (lfsr == LFSR_NAME_1) {
    if (mode == LFSR_ACCESS_ALL)
      _outp(input, LFSR1reg);
    else if (mode == LFSR_ACCESS_SEQUENTIAL)
      _outp(input, LFSR1data);
  }
}

#endif //SXC_LIB_LFSR_H_
