/*                  Copyright Eleven Engineering 2014                         */
/* Author: Sam Cristall & Thia Wyrod
 * License: GPLv3
 */
/** \file SXC.h
 *  \brief Basis for defining XInC2 functionality, such as "under-the-hood"
 *  interface and std library functions.
 *  \details
 *  Optimization macros for SXC library and XInC2 processor.
 *  asm definitions for XInC2 instructions that don't model well in LLVM.
 *  Definitions of debugging/assertion switches.
 */
#ifndef _SXC_LIB_SXC_H_
#define _SXC_LIB_SXC_H_

#include <stdint.h>
#include <stddef.h>
// include __Macros for commonly used macros
#include "__Macros.h"

#ifndef __XINC2__
#include <assert.h>
#endif

/******************************************************************************/
/*                         XInC2 Instruction Handlers                         */
/******************************************************************************/
//
// Some XInC2 instructions that aren't easily modelled in LLVM
// These are functions that compile down to single instructions
uint16_t _inp(const uint16_t port)
  __asm__("llvm.XInC2.inp");

void _outp(uint16_t val, const uint16_t port)
  __asm__("llvm.XInC2.outp");

SXC_INLINE uint16_t _rol(uint16_t a, uint16_t b)
{
  register uint16_t retval;
  __asm__("rol\t%0,%1,%2" : "=r"(retval) : "r"(a), "rn"(b) : "EX" );
  return retval;
}

SXC_INLINE uint16_t _ror(uint16_t a, uint16_t b)
{
  return _rol(a, -b);
}

SXC_INLINE void _nop(void)
{
  __asm__("");
}

SXC_INLINE uint16_t _thrd(void)
{
  register uint16_t retval;
  __asm__("thrd\t%0" : "=r"(retval) : :);
  return retval;
}

SXC_INLINE void _softreset(void)
{
  __asm__("bra\tSoftReset" : : :);
}

SXC_INLINE void _terminate_and_show_code(uint16_t code)
{
  register uint16_t r0 __asm__("R0") = code;
  __asm__("bra\tShowTerminationCode" : : "r"(r0) :);
}

#ifndef __DISABLE_SOFTWARE_DIVIDE__
SXC_INLINE __attribute__((used)) uint16_t __xinc_udiv(uint16_t lhs, uint16_t rhs)
{
  register uint16_t r1 __asm__("R1") = lhs;
  register uint16_t r2 __asm__("R2") = rhs;
  __asm__ ("jsr\tr6, UnsignedDivide"
      : "=r"(r1), "=r"(r2)
      : "r"(r1), "r"(r2)
      : "%R6");
  return r1;
}

SXC_INLINE __attribute__((used)) uint16_t __xinc_umod(uint16_t lhs, uint16_t rhs)
{
  register uint16_t r1 __asm__("R1") = lhs;
  register uint16_t r2 __asm__("R2") = rhs;
  __asm__ ("jsr\tr6, UnsignedDivide"
      : "=r"(r1), "=r"(r2)
      : "r"(r1), "r"(r2)
      : "%R6");
  return r2;
}
#endif //__DISABLE_SOFTWARE_DIVIDE

SXC_INLINE void internal_busy_wait(uint16_t timeout)
{
  //timeout is measured in system ticks, but thread ticks operate at 1/8th of
  //that, and each iteration of the loop is 2 instructions
  register uint16_t loop_counter __asm__("R1") = timeout >> 4;
  __asm__ volatile (
    "sub\tr1, r1, 1\n"
    "bc\tNE, @-1"
    :"=r"(loop_counter)
    :"r"(loop_counter)
    :
  );
}

#endif //SXC_LIB_SXC_H_
