/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file BBU.h
 *  \brief Interface for the XInC2 baseband unit.
 */
#ifndef _SXC_LIB_BBU_H_
#define _SXC_LIB_BBU_H_

#include "SystemClock.h"

/**
 * \defgroup bbu_config_flags
 * List of bitwise-ORable flags to set up the baseband unit
 * @{
 */
enum bbu_config_flag {
  BBU_ENABLE = 1 << 0,
  BBU_TX_ENABLE = 1 << 1,
  BBU_RAW_ENABLE = 1 << 2,
  BUU_FORCE_HUNT = 1 << 3,
  BBU_LOOPBACK_MODE = 1 << 4,
  BBU_USE_CLK_IN = 1 << 5,
  BBU_CLK_DETECT_ENABLE = 1 << 6,
  BBU_CLK_ENABLE = 1 << 7,
  BBU_CLK_OUT_RISING_EDGE = 1 << 8,
  BBU_CLK_IN_RISING_EDGE = 1 << 9,
  BBU_TX_MODE_RET_TO_ZERO = 1 << 10,
  BBU_RX_MODE_RET_TO_ZERO = 1 << 11,
  BBU_BIDIRECTION_ENABLE = 1 << 12,
  BBU_EXT_CLK_TO_INC = 1 << 13,
  BBU_USE_NEW_BAUD_GEN = 1 << 14,
  BBU_ROM_ENABLE = 1 << 15,
};
/** @} */

enum BaudRate {
  _19200,
  _57600,
  _115200,
  _2048000
};

enum BBU_name {
  BBU0 = 0,
  BBU1 = 1,
  kNumBBUs
};

enum BBU_reg_type {
  BBU_CFG0,
  BBU_CFG1,
  BBU_start,
  BBU_brg,
  BBU_time,
  BBU_TX,
  BBU_rx4,
  BBU_rx6,
  BBU_rx
};

SXC_INLINE uint16_t get_bbu_address(enum BBU_reg_type type, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  switch(name) {
  case BBU0:
    switch(type) {
    case BBU_CFG0:
      return BBU0cfg0;
    case BBU_CFG1:
      return BBU0cfg1;
    case BBU_start:
      return BBU0start;
    case BBU_brg:
      return BBU0brg;
    case BBU_time:
      return BBU0time;
    case BBU_TX:
      return BBU0tx;
    case BBU_rx4:
      return BBU0rx4;
    case BBU_rx6:
      return BBU0rx6;
    case BBU_rx:
      return BBU0rx;
    }
  case BBU1:
    switch(type) {
    case BBU_CFG0:
      return BBU1cfg0;
    case BBU_CFG1:
      return BBU1cfg1;
    case BBU_start:
      return BBU1start;
    case BBU_brg:
      return BBU1brg;
    case BBU_time:
      return BBU1time;
    case BBU_TX:
      return BBU1tx;
    case BBU_rx4:
      return BBU1rx4;
    case BBU_rx6:
      return 0; //BBU1 doesn't have this register
    case BBU_rx:
      return BBU1rx;
    }
  default:
    return 0;
  }
}

SXC_INLINE void bbu_set_cfg_0(uint16_t config, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _outp(config, get_bbu_address(BBU_CFG0, name));
}

SXC_INLINE void bbu_set_cfg_1(uint16_t config, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _outp(config, get_bbu_address(BBU_CFG1, name));
}

SXC_INLINE void bbu_set_brg(uint16_t baudrate, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _outp(baudrate, get_bbu_address(BBU_brg, name));
}

SXC_INLINE void bbu_set_start_word(uint16_t start_word, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _outp(start_word, get_bbu_address(BBU_start, name));
}

//allows the user to setup a BBU of their choosing without worrying about registers
SXC_INLINE void bbu_setup(uint16_t br, uint16_t cfg0, uint16_t cfg1,
                          uint16_t start_word, enum BBU_name name) {
  safe_mode_assert(name < kNumBBUs);
  _outp(cfg0, get_bbu_address(BBU_CFG0, name));
  _outp(cfg1, get_bbu_address(BBU_CFG1, name));
  _outp(start_word, get_bbu_address(BBU_start, name));
  _outp(br, get_bbu_address(BBU_brg, name));
}

//disables the receive raw data mode on the BBU chosen
SXC_INLINE void bbu_lock_timing(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  uint16_t addr = get_bbu_address(BBU_CFG0, name);
  _outp(_inp(addr) | BBU_RAW_ENABLE, addr);
}

//enables the receive raw data mode on the BBU chosen
SXC_INLINE void bbu_unlock_timing(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  uint16_t addr = get_bbu_address(BBU_CFG0, name);
  _outp(_inp(addr) & ~BBU_RAW_ENABLE, addr);
}

//disables the receive raw data mode on the BBU chosen
SXC_INLINE void bbu_set_transmitting(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  uint16_t addr = get_bbu_address(BBU_CFG0, name);
  _outp(_inp(addr) | BBU_TX_ENABLE, addr);
}

//enables the receive raw data mode on the BBU chosen
SXC_INLINE void bbu_set_receiving(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  uint16_t addr = get_bbu_address(BBU_CFG0, name);
  _outp(_inp(addr) & ~BBU_TX_ENABLE, addr);
}

//fetches the current word read by the BBU without side effects or blocking
SXC_INLINE uint16_t bbu_peek(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  return _inp(get_bbu_address(BBU_rx, name));
}


//read current word, but block on word alignment
SXC_INLINE uint16_t bbu_get_word(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _inp(get_bbu_address(BBU_rx4, name)); //locks word alignment
  return _inp(get_bbu_address(BBU_rx, name));
}

//is the preamble bit set?
static const size_t kRxPreambleBit = 1 << 15;
SXC_INLINE uint16_t bbu_contains_preamble(enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  return (_inp(get_bbu_address(BBU_rx4, name)) & kRxPreambleBit) != 0;
}

//writes a word to the BBU BBU_TX, but blocks on word alignment
SXC_INLINE void bbu_push_word(uint16_t input, enum BBU_name name)
{
  safe_mode_assert(name < kNumBBUs);
  _outp(input, get_bbu_address(BBU_TX, name)); //NB: blocks on word alignment
}

SXC_INLINE uint16_t bbu_get_time(enum BBU_name name) {
  safe_mode_assert(name < kNumBBUs);
  return _inp(get_bbu_address(BBU_time, name));
}

#endif //SXC_LIB_BBU_H_
