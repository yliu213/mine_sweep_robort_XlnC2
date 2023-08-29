/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file IOConfig.h
 *  \brief I/O configuration interface. Sets up I/O config pins on the XInC2.
 *  Functions are NOT thread-safe: manual management of semaphores is required.
 */
#ifndef _SXC_IOCONFIG_H_
#define _SXC_IOCONFIG_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup IO_port_config_flags
 * List of bitwise-ORable flags to specify an IO port configuration.
 * @{
 */
enum IO_port_config_flag {
  IO_FAST_SLEW = 0x1 << 0, //by default, we have a slow slew
  IO_SCHMITT = 0x1 << 4, //by default, no Schmitt trigger activation
  IO_PULL_UP = 0x1 << 5, //by default, pull down
  IO_PULL_ENABLE = 0x1 << 6, //by default, pull disabled
  IO_SLOW_SLEW = 0,
  IO_NO_SCHMITT = 0,
  IO_PULL_DOWN = 0,
  IO_PULL_DISABLE = 0,

//Drive strength bitfield
  IO_DRIVE_2mA = 0, //default
  IO_DRIVE_4mA = 0x1 << 1,
  IO_DRIVE_6mA = 0x2 << 1,
  IO_DRIVE_8mA = 0x3 << 1,
  IO_DRIVE_10mA = 0x4 << 1,
  IO_DRIVE_12mA = 0x5 << 1,
  IO_DRIVE_14mA = 0x6 << 1,
  IO_DRIVE_16mA = 0x7 << 1,
};
/** @} */

/**
 * \defgroup IO_port_names
 * List of available IO port names
 * @{
 */
enum io_port_name {
  io_PA = 0,
  io_PB0 = 1,
  io_PB1 = 2,
  io_PB2 = 3,
  io_PC0 = 4,
  io_PC1 = 5,
  io_PC2 = 6,
  io_PC3 = 7,
  io_PC4 = 8,
  io_PC5 = 9,
  io_PC6 = 10,
  io_PC7 = 11,
  io_PD0 = 12,
  io_PD1 = 13,
  io_PD2 = 14,
  io_PD3 = 15,
  io_PD4 = 16,
  io_PD5 = 17,
  io_PD6 = 18,
  io_PD7 = 19,
  io_PE = 20,
  io_PF = 21,
  io_PG0 = 22,
  io_PG1 = 23,
  io_PG2 = 24,
  io_PG3 = 25,
  io_PH = 26,
  io_PI = 27,
  io_PJ = 28,
  io_TMRB = 29,
  kNumIOPorts
};
/** @} */

/**
 * \brief Configures the selected I/O pin to the bitwise-ORed set of properties
 * specified.
 * \param[in] cfg Bitwise-ORed set of flags specifying the configuration.
 * \param[in] pin I/O pin to configure.
 */
SXC_INLINE void io_set_config(uint16_t cfg, enum io_port_name pin)
{
  safe_mode_assert(pin < kNumIOPorts);
  _outp(pin, SCXioCfgP);
  _outp(cfg, SCXioCfgD);
}

/**
 * \brief Retrieves the property configuration of the selected I/O pin at the
 * time of function call.
 * \param[in] pin I/O pin to get the config of.
 * \return Bitwise-ORed set of flags specifying the configuration.
 */
SXC_INLINE uint16_t io_get_config(enum io_port_name pin)
{
  safe_mode_assert(pin < kNumIOPorts);
  _outp(pin, SCXioCfgP);
  return _inp(SCXioCfgD);
}

/**
 * \brief Configures all of the XInC2 I/O pins according to the array of configs
 * provided. The array MUST be **at least** of length kNumIOPorts.
 * \param[in] cfgs Array of I/O pin configurations.
 */
SXC_INLINE void io_set_all_configs(uint16_t const * cfgs)
{
  uint16_t i;
  for(i=0; i<(uint16_t)kNumIOPorts; ++i)
    io_set_config(cfgs[i], (enum io_port_name)i);
}
#endif
