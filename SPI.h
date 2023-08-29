/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file SPI.h
 *  \brief Interface for configuring and reading/writing data on the serial
 *  peripheral interface.
 */
#ifndef _SXC_LIB_SPI_H_
#define _SXC_LIB_SPI_H_

#include "SystemClock.h"

enum SPI_name {
  SPI0 = 0,
  SPI1 = 1,
  kNumSPIs
};

enum SPI_reg_type {
  SPI_CFG,
  SPI_TX,
  SPI_RX
};

/**
 * \defgroup SPI_config_flags
 * List of bitwise-ORable flags to set up the SPI
 * @{
 */
enum SPI_config_flag {
  SPI_ENABLE = 0x1 << 0,
  SPI_MASTER = 0x1 << 1,
//the following are only applicable if master is set
  SPI_CLK_RATE_DIV_2 = 0x0 << 2,
  SPI_CLK_RATE_DIV_4 = 0x1 << 2,
  SPI_CLK_RATE_DIV_8 = 0x2 << 2,
  SPI_CLK_RATE_DIV_16 = 0x3 << 2,
  SPI_CLK_RATE_DIV_32 = 0x4 << 2,
  SPI_CLK_RATE_DIV_64 = 0x5 << 2,
  SPI_CLK_RATE_DIV_128 = 0x6 << 2,
  SPI_CLK_RATE_DIV_256 = 0x7 << 2,
  SPI_CLK_PHASE = 0x1 << 5,
  SPI_CLK_IDLE_AT_1 = 0x1 << 6,
//end of master-only config flags
  SPI_TX_PIN_DISABLE = 0x1 << 7,
  SPI_SLAVE_SELECT_BIT = 0x1 << 15, //read-only: value at CS0 pin
};
/** @} */


/**
 * \brief Returns the standard SPI configuration value for the given system
 * frequency and SPI.
 * \param[in] sf input system frequency
 * \param[in] name SPI to target
 * \return The SPI config for valid frequencies. 0 else.
 */
SXC_INLINE SXC_CONSTEXPR uint16_t get_SPI_std_cfg(enum sys_freq sf,
  enum SPI_name name)
{
  return
    (name == SPI0) ? (
      (sf == _12_288_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_16 :
      (sf == _24_576_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_32 :
      (sf == _36_864_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_64 :
      (sf == _49_152_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_64 :
      (sf == _73_728_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_128 :
      (sf == _98_304_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_128 :
      (sf == _110_592_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_256 :
      0) :
    (name == SPI1) ? (
      (sf == _12_288_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_2 :
      (sf == _24_576_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_4 :
      (sf == _36_864_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_4 :
      (sf == _49_152_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_8 :
      (sf == _73_728_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_8 :
      (sf == _98_304_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_16 :
      (sf == _110_592_MHz) ?
        SPI_ENABLE|SPI_MASTER|SPI_CLK_RATE_DIV_16 :
      0) :
    0;

}

SXC_INLINE uint16_t get_spi_addr(enum SPI_reg_type type, enum SPI_name name)
{
  safe_mode_assert(name < kNumSPIs);
  switch(name) {
  case SPI0:
    switch(type) {
    case SPI_CFG:
      return SPI0cfg;
    case SPI_TX:
      return SPI0tx;
    case SPI_RX:
      return SPI0rx;
    }
  case SPI1:
    switch(type) {
    case SPI_CFG:
      return SPI1cfg;
    case SPI_TX:
      return SPI1tx;
    case SPI_RX:
      return SPI1rx;
    }
  default:
    return 0;
  }
}

/**
 * \brief Configures the selected SPI based on the provided configuration. It is
 * recommended to instead call SPI_set_config_optimal(), unless you know what
 * you are doing.
 * \param[in] cfg Desired SPI configuration.
 * \param[in] name The selected SPI.
 */
SXC_INLINE void SPI_set_config(uint16_t cfg, enum SPI_name name)
{
  uint16_t reg = get_spi_addr(SPI_CFG, name);
  _outp(cfg, reg);
}

/**
 * \brief Returns the selected SPI's current configuration.
 * \param[in] name The selected SPI.
 * \return Desired SPI's configuration at time of function call.
 */
SXC_INLINE uint16_t SPI_get_config(enum SPI_name name)
{
  uint16_t reg = get_spi_addr(SPI_CFG, name);
  return _inp(reg);
}


/**
 * \brief Configures the selected SPI with the appropriate "best" standard
 * configuration for the given system frequency and SPI.
 * \param[in] sf The system clock frequency.
 * \param[in] name The selected SPI.
 */
SXC_INLINE void SPI_set_config_optimal(enum sys_freq sf, enum SPI_name name)
{
  SPI_set_config(get_SPI_std_cfg(sf, name), name);
}

/**
 * \brief Writes an 8-bit byte onto the MOSI or MISO line of the selected SPI,
 * depending on whether the SPI is configured as a master or slave, respectively.
 * \param[in] val LSB: The 8-bit value to write onto the line. MSB: ignored
 * \param[in] name The selected SPI.
 */
SXC_INLINE void SPI_write(uint16_t val, enum SPI_name name)
{
  uint16_t out_reg = get_spi_addr(SPI_TX, name);
  uint16_t in_reg = get_spi_addr(SPI_RX, name);
  _outp(val, out_reg);
  _inp(in_reg);
}

/**
 * \brief Reads an 8-bit byte from the MOSI or MISO line of the selected SPI,
 * depending on whether the SPI is configured as a slave or master, respectively.
 * \param[in] name The selected SPI.
 * \return LSB: The 8-bit value on the line at the time of function call. MSB:
 * undefined
 */
SXC_INLINE uint16_t SPI_read(enum SPI_name name)
{
  uint16_t in_reg = get_spi_addr(SPI_RX, name);
  uint16_t out_reg = get_spi_addr(SPI_TX, name);
  _outp(0, out_reg); //dummy write with 0 so an accidental command isn't sent
  return _inp(in_reg);
}

#endif //_SXC_LIB_SPI_H_
