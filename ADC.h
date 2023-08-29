/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file ADC.h
 *  \brief Interface for configuring and reading data from the on-chip
 *  analog-to-digital converter.
 */
#ifndef _SXC_LIB_ADC_H_
#define _SXC_LIB_ADC_H_

#include "SystemClock.h"

/**
 * \defgroup ADCcfg0_config_flags
 * List of bitwise-ORable flags to set up the ADC
 * @{
 */
enum ADCcfg0_config_flag {
  ADC_CLK_RATE_DIV_2 = 0x0 << 12,
  ADC_CLK_RATE_DIV_4 = 0x1 << 12,
  ADC_CLK_RATE_DIV_8 = 0x2 << 12,
  ADC_CLK_RATE_DIV_16 = 0x3 << 12,
  ADC_CLK_RATE_DIV_32 = 0x4 << 12,
  ADC_CLK_RATE_DIV_64 = 0x5 << 12,
  ADC_CLK_RATE_DIV_128 = 0x6 << 12,
  ADC_CLK_RATE_DIV_256 = 0x7 << 12,
  ADC_ENABLE_FLOW_CONTROL = 0x1 << 11,
  ADC_CONT_SAMPLING = 0x1 << 10,
  ADC_SINGLE_CONV_SAMPLING = 0x0 << 10,
  ADC_THREE_CHANNEL_MODE = 0x1 << 8,
  ADC_FOUR_CHANNEL_MODE = 0x0 << 8,
/** @} */
};

static const size_t ADC_ON_BIT = 0x1 << 15;
static const size_t ADC_START_CONV_BIT = 0x1 << 9;
static const size_t ADC_CFG1_RESET_BIT = 0x1 << 6;
static const size_t ADC_BUSY_BIT = 0x1 << 15;
static const size_t ADC_CLK_RATE_BIT_OFFSET = 12;
static const size_t ADC_CLK_RATE_MASK = 0x7 << ADC_CLK_RATE_BIT_OFFSET;

static const size_t kADCMsInvalid = 2;
static const uint16_t kADCReadDataMask = 0x03FF;
static const uint16_t kADCSampleChannelMask = 0x6000;
static const uint16_t kADCSampleChannelShift = 13;

/**
 * \brief Configures the ADC based on the provided configuration, a bitwise-ORed
 * set of ADCcfg0 flags.
 * \param[in] cfg Desired ADC configuration.
 */
SXC_INLINE void ADC_set_config(uint16_t cfg)
{
  _outp(cfg, ADCcfg0);
}

/**
 * \brief Returns the ADC's current configuration.
 * \return ADC's configuration at time of function call.
 */
SXC_INLINE uint16_t ADC_get_config(void)
{
  return _inp(ADCcfg0);
}

/**
 * \brief Powers on the ADC. Does nothing if the ADC is already powered on.
 * Values read from the ADC for 2 ms after powering on are invalid.
 * It is recommended to leave the ADC on only when necessary due to high power
 * consumption.
 */
SXC_INLINE void ADC_power_on(void)
{
  uint16_t cfg0 = ADC_get_config();
  ADC_set_config(cfg0|ADC_ON_BIT);
  uint16_t cfg1 = _inp(ADCcfg1);
  _outp(cfg1|ADC_CFG1_RESET_BIT, ADCcfg1);
  //the ADC operates at a fraction of the sys clock rate
  size_t log_offset = cfg0&ADC_CLK_RATE_MASK >> ADC_CLK_RATE_BIT_OFFSET;
  size_t ADC_to_sys_ticks = 2 << log_offset;
  //ADC reset bit must be on for at least 3 ADC clock cycles
  internal_busy_wait(3*ADC_to_sys_ticks);
  _outp(cfg1&~ADC_CFG1_RESET_BIT, ADCcfg1);
}

/**
 * \brief Powers the ADC off. Does nothing if the ADC is already powered off.
 */
SXC_INLINE void ADC_power_off(void)
{
  ADC_set_config(ADC_get_config()&~ADC_ON_BIT);
}

/**
 * \brief Sets the 2-bit channel number(s) for the ADC to read from. Reads in 1
 * channel number if the ADC is in single conversion mode. If it is in
 * continuous sampling mode, then as many channel numbers are read in as the ADC
 * is configured for (3 or 4). Only the least significant 2 bits from each number
 * are read; all others are ignored. No array boundary checking is done.
 * Undefined behaviour if the ADC is not configured before calling this
 * function.
 * \param [in] nums Pointer to desired sampling channel numbers
 */
SXC_INLINE void ADC_set_channel_nums(uint16_t const * nums)
{
  uint16_t cfg = ADC_get_config() & 0xFF00; //clear the channel nums
  size_t bound = (!(cfg & ADC_CONT_SAMPLING)) ? 1 :
                 (cfg & ADC_THREE_CHANNEL_MODE) ? 3 : 4;
  for (size_t i = 0; i < bound; ++i)
    cfg |= _rol( nums[i]&0x3, _rol(i,1) );
  ADC_set_config(cfg);
}

/**
 * \brief Sets the 6-bit number of ADC clocks to wait for after a sample has been
 * converted. This is only meaningful if the ADC is set for continuous sampling
 * mode; this has no effect in single sampling mode.
 * \param[in] num_clocks Number of ADC clocks to wait before re-sampling. Only
 * the least significant 6 bits are considered; all others are ignored.
 */
SXC_INLINE void ADC_set_clock_wait(uint16_t num_clocks)
{
  uint16_t cfg = _inp(ADCcfg1);
  cfg &= 0xFFC0;
  cfg |= (num_clocks&0x003F);
  _outp(cfg, ADCcfg1);
}

/**
 * \brief Returns the ADC's current clock wait rate. This is only meaningful if
 * the ADC is set for continuous sampling mode.
 * \return Number of ADC clocks to wait on at the time of function call.
 */
SXC_INLINE uint16_t ADC_get_clock_wait(void)
{
  return _inp(ADCcfg1)&0xFFC0;
}

/**
 * \brief Starts sample conversion by the ADC. Has no effect if the ADC is
 * already converting a sample. In single conversion mode, must be called every
 * time a new sample is requested. In continuous mode, it needs to be called
 * only once before reading any samples.
 */
SXC_INLINE void ADC_start(void)
{
  ADC_set_config(ADC_get_config()|ADC_START_CONV_BIT);
}

/**
 * \brief Reads a value measured by the ADC. If the flow control bit is enabled,
 * a call to this function will block until sample conversion is complete.
 * \return Bits 9-0: the 10-bit value measured and converted by the ADC.
 * Bits 12-10: reserved?
 * Bits 14-13: the channel number of the sample
 * Bit 15: if 1, the ADC is busy doing a conversion
 */
SXC_INLINE uint16_t ADC_read(void)
{
  return _inp(ADCdata);
}

#endif //_SXC_LIB_ADC_H_
