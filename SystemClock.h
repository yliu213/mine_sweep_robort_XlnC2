/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file SystemClock.h
 *  \brief Interface for configuring the system clock.
 */
#ifndef _SXC_LIB_SYSTEM_H_
#define _SXC_LIB_SYSTEM_H_

#include "Timer.h"

/**
 * \defgroup clock_config_flags
 * List of bitwise-ORable flags to set up the system clock
 * @{
 */
enum clock_config_flag {
  SYSCLOCK_OSC_DISABLE = 0x0,
  SYSCLOCK_OSC_ENABLE = 0x1,
  SYSCLOCK_OSC_DISABLE_TRISTATE = 0x1 << 1,
  SYSCLOCK_OSC_3RD_OVERTONE = 0x1 << 2,
  SYSCLOCK_OSC_FUNDAMENTAL = 0x0,
  SYSCLOCK_HIGH_FREQ = 0x1 << 3,
  SYSCLOCK_LOW_FREQ = 0x0,
  SYSCLOCK_FEEDBACK_REG_DISABLE = 0x1 << 4,
  SYSCLOCK_SELECT_HFOSC = 0x1 << 10,
  SYSCLOCK_SELECT_RC = 0x0,
  SYSCLOCK_HFOSC_PLL = 0x1 << 11,
  SYSCLOCK_HFOSC_CRYSTAL = 0x0,
};
/** @} */

/**
 * \defgroup pll_config0_flags_consts
 * List of bitwise-ORable flags and constants to configure the PLL
 * Consult XInC2 manual for details, I promise that these will make sense
 * @{
 */
enum pll_config0_flag {
  PLL_ENABLE = 0x1,
  PLL_HF_MODE = 0x1 << 13,
  PLL_LF_MODE = 0x0,
  PLL_DIVIDER_BITSHIFT = 1,
  PLL_FEEDBACK_DIVIDER_BITSHIFT = 7,
};
/** @} */

/**
 * \defgroup scx_alt_config_flags
 * List of bitwise-ORable flags to configure the SCX options
 * Consult XInC2 manual for details, I promise that these will make sense
 * @{
 */
enum scx_alt_config_flag {
  SCX_BUFFER_ENABLE = 0x1,
  SCX_DRIVE_STR_2mA = 0x0 << 1,
  SCX_DRIVE_STR_4mA = 0x1 << 1,
  SCX_DRIVE_STR_6mA = 0x2 << 1,
  SCX_DRIVE_STR_8mA = 0x3 << 1,
  SCX_DRIVE_STR_10mA = 0x4 << 1,
  SCX_DRIVE_STR_12mA = 0x5 << 1,
  SCX_DRIVE_STR_14mA = 0x6 << 1,
  SCX_DRIVE_STR_16mA = 0x7 << 1,
  SCX_CLK_SLEW_FAST = 0x1 << 4,
  SCX_DIVIDER_SRC_DISABLE = 0x1 << 5,
  SCX_BUF_SRC_PLL = 0x0,
  SCX_BUF_SRC_RC = 0x1 << 6,
  SCX_BUF_SRC_CRYS = 0x2 << 6,
  SCX_BUF_SRC_SYSCLK = 0x3 << 6,
  SCX_BUF_CLK_DIV_SHIFT = 8, //The desired 5-bit clock divider value is
                             //specified directly, then ORed in after shifting
                             //by this amount. The actual divider value will
                             //be 2x what is given.
                             //Value of 0 maps to division by 1, however.
  SCX_DASI_CLK_DISABLE = 0x1 << 13,
  SCX_I2S_SRC_SYSCLK = 0x0,
  SCX_I2S_SRC_RC = 0x1 << 14,
  SCX_I2S_SRC_CRYS = 0x2 << 14,
  SCX_I2S_SRC_PLL = 0x3 << 14,
};

/** @} */

/**
 * \defgroup system_clock_frequencies
 * List of available system clock frequencies
 * @{
 */
enum sys_freq {
  _12_288_MHz = 0,
  _24_576_MHz = 1,
  _36_864_MHz = 2,
  _49_152_MHz = 3,
  _73_728_MHz = 5,
  _98_304_MHz = 7,
  _110_592_MHz = 8
};
/** @} */

/**
 * \defgroup crystal_frequencies
 * List of available crystal oscillation frequencies
 * @{
 */
enum crystal_freq {
  crys_12_288_MHz = 0,
  crys_24_576_MHz = 1,
  kNumCFs
};
/** @} */

static const size_t kNumSlicesPerMs = 16;
//number of time slices that an ms is split into

/**
 * \brief Returns the number of system clock ticks per millisecond "slice"
 * based on the provided system frequency.
 * \param [in] sf System frequency for which to eval
 * \return The number of system clock ticks per millisecond "slice"
 */
SXC_INLINE SXC_CONSTEXPR size_t sys_clock_ticks_per_ms_slice(enum sys_freq sf)
{
  return //constexpr is rather limited in C++11, so we must use this ternary chain
    (sf == _12_288_MHz) ? 12288/kNumSlicesPerMs :
    (sf == _24_576_MHz) ? 24576/kNumSlicesPerMs :
    (sf == _36_864_MHz) ? 36864/kNumSlicesPerMs :
    (sf == _49_152_MHz) ? 49152/kNumSlicesPerMs :
    (sf == _73_728_MHz) ? 73728/kNumSlicesPerMs :
    (sf == _98_304_MHz) ? 98304/kNumSlicesPerMs :
    (sf == _110_592_MHz) ? 110592/kNumSlicesPerMs : 0;
}

SXC_INLINE uint16_t is_sysclock_src_oscn(void)
{
  return (_inp(SCXclkCfg) & (0x1 << 15)) ? 1 : 0;
}

SXC_INLINE uint16_t get_pllcfg1(enum crystal_freq cf, enum sys_freq sf)
{
  return (cf == crys_24_576_MHz && sf == _49_152_MHz) ? 0 : 1;
}

SXC_INLINE uint16_t get_pllcfg0(enum crystal_freq cf, enum sys_freq sf)
{
  switch(cf) {
  case crys_12_288_MHz:
    switch(sf) {
    case _24_576_MHz:
      return PLL_ENABLE|(0x2 << PLL_DIVIDER_BITSHIFT)|
        (0x8 << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    case _49_152_MHz:
      return PLL_ENABLE|(0x2 << PLL_DIVIDER_BITSHIFT)|
        (0x10 << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    case _73_728_MHz:
      return PLL_ENABLE|(0x2 << PLL_DIVIDER_BITSHIFT)|
        (0x18 << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    default:
      return PLL_ENABLE | ((sf + 1) << (1+PLL_FEEDBACK_DIVIDER_BITSHIFT)) |
        ((cf + 1) << PLL_DIVIDER_BITSHIFT);
    }

  case crys_24_576_MHz:
    switch(sf) {
    case _36_864_MHz:
      return PLL_ENABLE|(0x1 << PLL_DIVIDER_BITSHIFT)|
        (0x3 << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    case _49_152_MHz:
      return PLL_ENABLE|(0x1 << PLL_DIVIDER_BITSHIFT)|
        (0x2 << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    case _73_728_MHz:
      return PLL_ENABLE|(0x2 << PLL_DIVIDER_BITSHIFT)|
        (0xC << PLL_FEEDBACK_DIVIDER_BITSHIFT);
    default:
      return PLL_ENABLE | ((sf + 1) << (1+PLL_FEEDBACK_DIVIDER_BITSHIFT)) |
        ((cf + 1) << PLL_DIVIDER_BITSHIFT);
    }
  default:
    return 0;
  }
}

SXC_INLINE uint16_t get_scxaltcfg(enum crystal_freq cf)
{
  uint16_t const alt_cfg = SCX_BUFFER_ENABLE | SCX_DRIVE_STR_4mA |
    SCX_BUF_SRC_CRYS | SCX_I2S_SRC_CRYS;
  return (cf == crys_24_576_MHz) ? alt_cfg|(0x1 << SCX_BUF_CLK_DIV_SHIFT) : alt_cfg;
}

SXC_INLINE uint16_t sys_clock_get_scu_time(void)
{
  return _inp(SCUtime);
}

/**
 * \brief Initiates/configures the system clock based on the given crystal
 * resonant and desired system frequencies.
 * \param [in] cf desired crystal frequency
 * \param [in] sf desired system frequency
 */
SXC_INLINE void sys_clock_init(enum crystal_freq cf, enum sys_freq sf)
{
  _outp( _inp(SCXclkCfg)&~SYSCLOCK_SELECT_HFOSC, SCXclkCfg );
  while (is_sysclock_src_oscn())
    continue;

  _outp( get_pllcfg1(cf, sf), SCXPLLCfg1 );
  _outp( get_pllcfg0(cf, sf), SCXPLLCfg0 );

  _outp( _inp(SCXclkCfg)&~SYSCLOCK_SELECT_HFOSC, SCXclkCfg );
  while (is_sysclock_src_oscn())
    continue;

  _outp(_inp(SCXclkCfg)|SYSCLOCK_HFOSC_PLL|SYSCLOCK_SELECT_HFOSC,SCXclkCfg);

  while (!is_sysclock_src_oscn())
    continue;

  _outp(get_scxaltcfg(cf), SCXaltCfg);
}

/**
 * \brief Busy-waits the specified amount of time in processor 'ticks'.
 * \param[in] duration number of ticks to wait (ticks=seconds*sys_freq)
 */
SXC_INLINE void sys_clock_wait(uint16_t duration)
{
  uint16_t end_time = sys_clock_get_scu_time() + duration;
  int16_t delta = -1;
  while (delta < 0)
    delta = (int16_t)(sys_clock_get_scu_time() - end_time);
}

#endif //_SXC_LIB_SYSTEM_H_
