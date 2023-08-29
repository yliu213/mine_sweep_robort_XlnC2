/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file Varactor.h
 *  \brief Interface for utilizing the varactor to adjust crystal resonant
 *  frequency. Utilizes timer B to do so, will need semaphore protection.
 */
#ifndef _SXC_LIB_VARAC_H_
#define _SXC_LIB_VARAC_H_

#include "Timer.h"
#include "SystemClock.h"

#define VARACTOR_UP_VALUE 450
#define VARACTOR_DOWN_VALUE 50

SXC_INLINE uint16_t get_tmrbcfg(enum sys_freq sf)
{
  switch(sf) {
  case _12_288_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP;
  case _24_576_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP;
  case _36_864_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP;
  case _49_152_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP|TIMER_CLOCK_RATE_4;
  case _73_728_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP|TIMER_CLOCK_RATE_4;
  case _98_304_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP|TIMER_CLOCK_RATE_8;
  case _110_592_MHz:
    return TIMER_ENABLE|TIMER_MODE_UP|TIMER_CLOCK_RATE_8;
  default:
    return 0;
  }
}

SXC_INLINE void varactor_setup(enum sys_freq sf)
{
  //The 0x1 should do nothing (read-only bit), but it was there in the original
  timer_set_config_ccm(
    0x1 | TIMER_CCM_COMP_NOT_CAP | TIMER_CCM_CMP_OUT_EQ_1 | TIMER_CCM_OUT_ENABLE
    | TIMER_CCM_OUTPUT_RESET_SET, 1, TIMER_B
  );
  timer_write_CCR(500, 0, TIMER_B); //why 500? nfi
  timer_write_CCR(190, 1, TIMER_B); //why 190? nfi
  timer_set_config(get_tmrbcfg(sf), TIMER_B);
}

SXC_INLINE void varactor_pull_up(void)
{
  timer_write_CCR(VARACTOR_UP_VALUE, 1, TIMER_B);
}

SXC_INLINE void varactor_pull_down(void)
{
  timer_write_CCR(VARACTOR_DOWN_VALUE, 1, TIMER_B);
}

#endif //SXC_LIB_VARAC_H_
