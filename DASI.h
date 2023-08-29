/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod & Sam Cristall
 * License: GPLv3
 */
/** \file DASI.h
 *  \brief Interface for accessing digital audio serial interface-related
 *  registers.
 */
#ifndef _SXC_LIB_DASI_H_
#define _SXC_LIB_DASI_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup DASIconfig0_flags
 * List of bitwise-ORable flags to set up the DASI, config register 0
 * @{
 */
enum DASIconfig0_flag {
  DASI_DISABLE = 0x0 << 0,
  DASI_ENABLE = 0x1 << 0,
  DASI_WAIT_ENABLE = 0x1 << 1,
  DASI_BIT_CLK_RISING_LATCH = 0x1 << 2,
  DASI_BIT_CLK_FALLING_LATCH = 0x0 << 2,
  DASI_MODE_MASTER = 0x1 << 3,
  DASI_MODE_SLAVE = 0x0 << 3,
  DASI_ENABLE_TRANSCEIVER_0 = 0x1 << 4,
  DASI_ENABLE_TRANSCEIVER_1 = 0x1 << 8,
  DASI_ENABLE_TRANSCEIVER_2 = 0x1 << 12,
  DASI_TRANSMIT_CHANNEL_0 = 0x1 << 5,
  DASI_TRANSMIT_CHANNEL_1 = 0x1 << 9,
  DASI_TRANSMIT_CHANNEL_2 = 0x1 << 13,
  DASI_RECEIVE_CHANNEL_0 = 0x0 << 5,
  DASI_RECEIVE_CHANNEL_1 = 0x0 << 9,
  DASI_RECEIVE_CHANNEL_2 = 0x0 << 13,
  DASI_SHIFT_DATA_R1_REL_LRCK_0 = 0x1 << 6,
  DASI_SHIFT_DATA_R1_REL_LRCK_1 = 0x1 << 10,
  DASI_SHIFT_DATA_R1_REL_LRCK_2 = 0x1 << 14,
};
/** @} */

/**
 * \defgroup DASIconfig1_flags
 * List of bitwise-ORable flags to set up the DASI, config register 1
 * @{
 */
enum DASIconfig1_flag {
  DASI_CLK_GEN_ENABLE = 0x1 << 15,
  DASI_SCK_OUT_DIV_2 = 0x1 << 0,
  DASI_SCK_OUT_DIV_4 = 0x2 << 0,
  DASI_SCK_OUT_DIV_8 = 0x3 << 0,
  DASI_SCK_OUT_DIV_16 = 0x4 << 0,
  DASI_SCK_OUT_DIV_32 = 0x5 << 0,
  DASI_SCK_OUT_DIV_64 = 0x6 << 0,
  DASI_SCK_OUT_DIV_128 = 0x7 << 0,
  DASI_SCK_OUT_DIV_256 = 0x8 << 0,
  DASI_SCK_OUT_DIV_512 = 0x9 << 0,
  DASI_MCLK_OUT_DIV_1 = 0x0 << 4,
  DASI_MCLK_OUT_DIV_2 = 0x1 << 4,
  DASI_MCLK_OUT_DIV_4 = 0x2 << 4,
  DASI_MCLK_OUT_DIV_8 = 0x3 << 4,
  DASI_MCLK_OUT_DIV_16 = 0x4 << 4,
  DASI_MCLK_OUT_DIV_32 = 0x5 << 4,
  DASI_MCLK_OUT_DIV_64 = 0x6 << 4,
  DASI_MCLK_OUT_DIV_128 = 0x7 << 4,
  DASI_MCLK_OUT_DIV_256 = 0x8 << 4,
  DASI_MCLK_OUT_DIV_512 = 0x9 << 4,
  DASI_BITS_PER_FRAME_SHIFT = 8, //4-bit integer, ORed in after shifting by
                                    //this; 16: no LRCK sync, continuous mode
  DASI_LOOPBACK_NONE = 0x0 << 12,
  DASI_LOOPBACK_SDIO_0_1 = 0x1 << 12,
  DASI_LOOPBACK_SDIO_0_2 = 0x2 << 12,
  DASI_LOOPBACK_SDIO_1_2 = 0x3 << 12,
  DASI_LRCK_SYNC = 0x1 << 14,
};
/** @} */

/**
 * \defgroup DASIconfig2_info
 * Information on the DASI, config register 2
 * @{
 */
//bits 7-0 represent an 8-bit integer n: LRCK must be low for n+1 SCK ticks before
//a low-high transition
//
//bits 15-8 represent an 8-bit integer n: LRCK must be high for n+1 SCK ticks before
//a high-low transition
/** @} */
static const size_t kNumDasiCfgRegs = 3;
static const size_t kNumSdioPorts = 3;

/**
 * \brief Returns the selected DASI register's current configuration.
 * \param[in] cfg_reg The DASI configuration register to access.
 * \return The DASI config register's configuration at time of function call.
 */
SXC_INLINE uint16_t dasi_get_config(size_t cfg_reg)
{
  safe_mode_assert(cfg_reg < kNumDasiCfgRegs);
  switch(cfg_reg)
  {
  case 0:
    return _inp(DASIconfig0);
  case 1:
    return _inp(DASIconfig1);
  case 2:
    return _inp(DASIconfig2);
  default:
    return 0;
  }
}

/**
 * \brief Configures the selected DASI register's with the provided config.
 * \param[in] cfg The DASI configuration to write.
 * \param[in] cfg_reg The DASI configuration register to access.
 */
SXC_INLINE void dasi_set_config(uint16_t cfg, size_t cfg_reg)
{
  safe_mode_assert(cfg_reg < kNumDasiCfgRegs);
  switch(cfg_reg)
  {
  case 0:
    _outp(cfg, DASIconfig0);
    return;
  case 1:
    _outp(cfg, DASIconfig1);
    return;
  case 2:
    _outp(cfg, DASIconfig2);
    return;
  default:
    return;
  }
}

/**
 * \brief Transmits 8-bits of data on the selected SDIO port
 * \param[in] value A word containing 8-bits of data to be sent (upper 8-bits are ignored)
 * \param[in] sdio_port The SDIO port number to write to
 * \note Assumes the DASI has already been configured to transmit on the given port
 */
SXC_INLINE void dasi_tx(uint16_t value, size_t sdio_port)
{
  safe_mode_assert(sdio_port < kNumSdioPorts);
  switch(sdio_port)
  {
  case 0:
    _outp(value, DASItx0);
    return;
  case 1:
    _outp(value, DASItx1);
    return;
  case 2:
    _outp(value, DASItx2);
    return;
  default:
    return;
  }
}

/**
 * \brief Receives 8-bits of data from a selected SDIO port, plus status bits
 * \param[in] sdio_port The SDIO port number to read from
 * \return Returns 8-bits of data in the LSB as well as FIFO full/empty, overflow and LRCK status bits
 * \note Assumes the DASI has already been configured to receive on the given port
 */
SXC_INLINE uint16_t dasi_rx(size_t sdio_port)
{
  safe_mode_assert(sdio_port < kNumSdioPorts);
  switch(sdio_port)
  {
  case 0:
    return _inp(DASIrx0);
  case 1:
    return _inp(DASIrx1);
  case 2:
    return _inp(DASIrx2);
  default:
    return 0;
  }
}
#endif //_SXC_LIB_DASI_H_
