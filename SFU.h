/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file SFU.h
 *  \brief SFU instruction interface. SFU instructions are fast, hardware-
 *  implemented mathematical operations of some potential use. Functions are
 *  thread-safe.
 */
#ifndef _SXC_LIB_SFU_H_
#define _SXC_LIB_SFU_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \brief Packs the bits of the lower byte of the input as described by the mask
 * in the upper byte of the input. See details, or XInC2 manual for more.
 * \param[in] input an arbitrary 16-bit number
 * \details
 * The 8-bit mask in the upper byte determines which bits to select from the
 * 8-bit value in the lower byte for the output. Only value bits whose positions
 * align with bits set to 1 in the mask are selected. These bits are then
 * packed, right-aligned, into as many bits as there were '1's in the mask.
 * \return The mask-selected bits of the lower byte, packed right-aligned. All
 * other bits are set to 0.
 *
 */
SXC_INLINE uint16_t sfu_pack_bits(uint16_t input)
{
  _outp(input, SFUpack);
  return _inp(SFUpack);
}

/**
 * \brief The bit position of the least significant bit set to 1 is returned as
 * a 4-bit integer in the rightmost bits of the return value. If no bit of
 * input is set to 1, the leftmost bit (15) of the return value is set to 1.
 * (zero flag).
 * \param[in] input an arbitrary 16-bit number
 * \return The 4-bit integer representing the first bit set to 1 in input.
 * If input is 0, returns 0x8000.
 */
SXC_INLINE uint16_t sfu_least_significant_one(uint16_t input)
{
  _outp(input, SFUls1);
  return _inp(SFUls1);
}

/**
 * \brief Returns the number of bits set to 1 in input. Value is returned as a
 * 5-bit integer in the rightmost bits of the SFUpop register.
 * \param[in] input an arbitrary 16-bit number
 * \return The 5-bit integer representing the number of bits set to 1 in input.
 */
SXC_INLINE uint16_t sfu_pop_count(uint16_t input)
{
  _outp(input, SFUpop);
  return _inp(SFUpop);
}

/**
 * \brief Returns the bitwise-reversed value of input, and writes it to the
 * SFUrev register.
 * \param[in] input an arbitrary 16-bit number
 * \return The input number, bitwise-reversed.
 */
SXC_INLINE uint16_t sfu_reverse_bits(uint16_t input)
{
  _outp(input, SFUrev);
  return _inp(SFUrev);
}

#endif //_SXC_LIB_SFU_H_
