/*                  Copyright Eleven Engineering 2014                         */
/* Author: Sam Cristall
 * License: GPLv3
 */
/** \file __Macros.h
 *  \brief Internal macros file used for various utility macros
 */

#ifndef _SXC_LIB___MACROS_H_
#define _SXC_LIB___MACROS_H_

/******************************************************************************/
/*                            COMMON MACROS                                   */
/******************************************************************************/


/** \def STRINGIFY(x)
 * \brief C Standard Assert Formatted to XInC2; returns the literal argument x
 */
#define STRINGIFY(x) #x

/** \def TOSTRING(x)
 * \brief C Standard Assert Formatted to XInC2; evaluates x and returns that
 * result
 */
#define TOSTRING(x) STRINGIFY(x)

/** \def MFENCE(x)
 * \brief XInC2 Optimization: use in place of volatile when declaring a variable
 * \details
 * Reduces the number of times that the variable is loaded.
 * Basically means "sync here". Forces a cache flush, even if we don't have one.
 */
#define MFENCE(x) __asm__("//MFENCE: " #x : : "m"(x) :)

//ASM debugging helper
#define MARK_ASM(mark) __asm__("//" #mark : : :)

//Not yet implemented assemble time assertion
#define NOT_YET_IMPL(name) \
__asm__("!error \"The function " #name " is not yet implemented\"" \
                                   : : :)

// The syntax on volatile asm can be tricky and is not cross-compiler compatible
#define ASM_VOLATILE __asm__ volatile

/** \def SXC_NARG(x)
 * \brief Used for a variadic function to declare args
 */
#define SXC_NARG(...) SXC_NARG_(__VA_ARGS__, SXC_RSEQ_N())
#define SXC_NARG_(...) SXC_ARG_N(__VA_ARGS__)
#define SXC_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, \
                  _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,  \
                  _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,  \
                  _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,  \
                  _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,  \
                  _63, N, ...)                                                 \
  N
#define SXC_RSEQ_N()                                                          \
  63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, \
      44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, \
      26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,  \
      8, 7, 6, 5, 4, 3, 2, 1, 0

#ifdef __SAFE_MODE__
#define safe_mode_assert(x) assert(x)
#else
#define safe_mode_assert(x)
#endif

#ifdef __cplusplus

// in C++, inline keyword suffices for inline
#define SXC_INLINE inline
// C Linkage is not the default in C++, SXC_C_LINK provides that linkage
#define SXC_C_LINK extern "C"
// Allows C++ to take advantage of certain functions at compile time.
#define SXC_CONSTEXPR constexpr

#else // __cplusplus not defined!

// in C, the inline keyword on its own won't emit a function body
// define static so we can get a body if inlining fails
#define SXC_INLINE static inline
// C Linkage is default in C, no need for a definition
#define SXC_C_LINK
// constexpr has no meaning in C!
#define SXC_CONSTEXPR

#endif // __cplusplus

#endif //_SXC_LIB___MACROS_H_
