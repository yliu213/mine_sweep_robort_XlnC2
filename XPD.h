/*                  Copyright Eleven Engineering 2014                         */
/* Author: Sam Cristall and Thia Wyrod
 * License: GPLv3
 */
/** \file XPD.h
 *  \brief Interface for XPD debugging unit.
 */
#ifndef _SXC_LIB_XPD_H_
#define _SXC_LIB_XPD_H_

#include "SXC.h"
#include "register_consts.h"

#ifndef __XINC2__
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

/**
 * \defgroup echo_flags
 * List of available flags to affect echo formatting.
 * @{
 */
enum echo_flag {
  XPD_Flag_Hex,
  XPD_Flag_HexNoPrefix,
  XPD_Flag_UnsignedDecimal,
  XPD_Flag_SignedDecimal
};

// Helper functions to make the MaskROM declarations a little cleaner
#define _MaskCallIn(type, reg, val, call)                    \
  register type zz __asm__(#reg) = val;                      \
  ASM_VOLATILE("\tjsr\tr6, " #call                           \
               :                                             \
               : "r"(zz)                                     \
               : #reg, "%R6");
#define _MaskCallOut(type, reg, val, call)                   \
  register type val __asm__(#reg);                           \
  ASM_VOLATILE("\tjsr\tr6, " #call                           \
               : "=r"(val)                                   \
               :                                             \
               : "%R6");
#define _MaskCallInOut(type, reg, val, out, call)            \
  register type out __asm__(#reg) = val;                     \
  ASM_VOLATILE("\tjsr\tr6, " #call                           \
               : "=r"(out)                                   \
               : "r"(out)                                    \
               : "%R6");

/**
 * \brief Checks the status of the XPD's connectivity.
 * \return XPD connection status. 1 implies connected, 0 implies not.
 */
SXC_INLINE uint16_t xpd_conn_status(void)
{
  register int reg1Out __asm__("R1");
  register int reg1In __asm__("R1") = 0x4000;
  register int reg2In __asm__("R2") = -1;
  ASM_VOLATILE("jsr\tr6, XPD_ShiftInOut"
               : "=r"(reg1Out)
               : "r"(reg1In), "r"(reg2In)
               : "%R6");
  if ((reg1Out & 0x3FFF) != 1) return 0;
  return 1;
}

/**
 * \brief Prints a character out on the XPD.
 * \param [in] input Char to print
 */
SXC_INLINE void xpd_putc(char input)
{
  _MaskCallIn(char, R1, input, XPD_WriteByte);
}

/**
 * \brief Prints a char array out on the XPD. Char array must be null-terminated;
 * otherwise, undefined behaviour occurs.
 * \param [in] input Char array to print
 */
SXC_INLINE void xpd_puts(const char *input)
{
  _MaskCallIn(const char *, R1, input, XPD_EchoString);
}

/**
 * \brief Prints an integer out on the XPD as a formatted string. Formatting
 * is dependent on the echo_flag passed in.
 * \param [in] number integer to print
 * \param [in] type echo_flag describing the formatting to use
 */
SXC_INLINE void xpd_echo_int(int number, enum echo_flag type)
{
    switch (type) {
      case XPD_Flag_SignedDecimal: {
        _MaskCallIn(unsigned int, R1, number, XPD_EchoSignedDec);
        break;
      }
      case XPD_Flag_UnsignedDecimal: {
        _MaskCallIn(unsigned int, R1, number, XPD_EchoUnsignedDec);
        break;
      }
      case XPD_Flag_Hex: {
        _MaskCallIn(unsigned int, R1, number, XPD_EchoHex);
        break;
      }
      case XPD_Flag_HexNoPrefix: {
        _MaskCallIn(unsigned int, R1, number, XPD_EchoHexNP);
        break;
      }
      default:
        break;
    }
}

/**
 * \brief Prints an array of integers out on the XPD. Prints values in
 * hexadecimal, 8 words per line, one space between words. Undefined behaviour
 * occurs if "len" exceeds the actual length of the array.
 * \param [in] array address of first int in array
 * \param [in] len length of array
 */
SXC_INLINE void xpd_echo_arr(int const *array, int len)
{
  register int r4Length __asm__("R4") = len;
  register int const *r5arrayPtr __asm__("R5") = array;
  ASM_VOLATILE(
      "//Assert Input to R4/R5 for XPD_EchoBlock"
      "\n\tjsr\tr6, XPD_EchoBlock"
      :
      : "r"(r5arrayPtr), "r"(r4Length)
      : "%R4", "%R5", "%R6");
}

/**
 * \brief Reads a single 8-bit byte from the input line of the XPD. Does not
 * return until a byte is received.
 * \return The raw 8-bit byte in the LSB of a signed integer.
 */
SXC_INLINE int xpd_getchar(void)
{
  _MaskCallOut(int, R1, out, XPD_ReadByte);
  return out & 0xFF;
}

/**
 * \brief Reads a single 8-bit byte from the input line of the XPD. Returns when
 * a byte is received or when "timeout" attempts
 * \return The raw 8-bit byte in the LSB of a signed integer. If the read timed
 * out, then 0xFFFF is returned instead.
 */
SXC_INLINE int xpd_getchar_timeout(int timeout)
{
  _MaskCallInOut(int, R1, timeout, out, XPD_ReadByteWithTimeout);
  if (out < 0)
    return out;  // This has been replaced with the return value
  else
    return out & 0xFF;
}

#endif //SXC_LIB_XPD_H_
