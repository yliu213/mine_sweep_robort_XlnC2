/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file Structs.h
 *  \brief Logical combinations of hardware constructs for ease of implementing
 *  more complex protocols and logic.
 */
#ifndef _SXC_LIB_STRUCTS_H_
#define _SXC_LIB_STRUCTS_H_

#include "IOConfig.h"
#include "GPIO.h"

enum PinLogicState {
  ON = 1,
  OFF = 0
};

enum GlobalPin_dir {
  PinDir_Input,
  PinDir_Output
};

enum Pin_Polarity {
  Polar_ActiveHigh = 1,
  Polar_ActiveLow = 0
};

typedef struct GlobalPin {
  enum GPIO_port_name const port;
  enum io_port_name const io_port;
  uint16_t const pin_bitmask;
  enum Pin_Polarity const polar;
} GlobalPin;

//globalPin_write_raw takes 3 instructions, which takes 8 times as many systicks
static const size_t kRawWriteSysTicks = 3 << 3;
static const size_t kRawReadSysTicks = 2 << 3;

/**
 * \brief Sets the directionality of the pin encapsulated by the GlobalPin to be
 * either output or input.
 * \param [in] glo_pin Pointer to a GlobalPin struct
 * \param [in] dir Desired pin directionality
 */
SXC_INLINE void globalPin_set_dir(enum GlobalPin_dir dir,
  GlobalPin const * glo_pin)
{
  uint16_t curr_conf = gpio_get_config(glo_pin->port);
  switch(dir) {
  case PinDir_Input:
    gpio_set_config(curr_conf & ~(glo_pin->pin_bitmask << 8), glo_pin->port);
    break;
  case PinDir_Output:
    gpio_set_config(curr_conf | (glo_pin->pin_bitmask << 8), glo_pin->port);
    break;
  }
}

/**
 * \brief Returns ON (1) if the pin encapsulated by glo_pin is logically high,
 * else returns OFF (0). Logically high refers to electrically high if the pin
 * is active-high, else refers to electrically low if the pin is active-low.
 * Behaviour is UNDEFINED if the pin is configured to be an output.
 * \param [in] glo_pin Pointer to GlobalPin struct
 * \return ON (1) if input pin is logic-high, 0 else; garbage value if pin is an
 * output
 */
SXC_INLINE enum PinLogicState globalPin_read(GlobalPin const * glo_pin)
{
  if (glo_pin->polar == Polar_ActiveHigh)
    return (enum PinLogicState)((gpio_read(glo_pin->port) & glo_pin->pin_bitmask) != 0);
  else
    return (enum PinLogicState)((gpio_read(glo_pin->port) & glo_pin->pin_bitmask) == 0);
}

/**
 * \brief Returns 1 if the pin encapsulated by glo_pin is electrically high,
 * else returns 0.
 * Behaviour is UNDEFINED if the pin is configured to be an output.
 * \param [in] glo_pin Pointer to GlobalPin struct
 * \return 1 if input pin is electrically-high, 0 else; garbage value if pin is
 * an output
 */
SXC_INLINE uint16_t globalPin_read_raw(GlobalPin const * glo_pin)
{
  return (uint16_t)((gpio_read(glo_pin->port) & glo_pin->pin_bitmask) != 0);
}

/**
 * \brief Sets or clears the pin encapsulated by glo_pin. Pin is set logically
 * high if input is non-zero (ON); if input is zero (OFF), pin is set logically
 * low. Logically low/high correlate to electrically low/high only if the pin
 * is ActiveHigh; else, they correlate to the opposite.
 * Undefined behaviour occurs if the pin is configured to be an input.
 * \param [in] input If non-zero, pin is set high; else, pin is set low.
 * \param [in] glo_pin Pointer to GlobalPin struct
 */
SXC_INLINE void globalPin_write(enum PinLogicState input,
  GlobalPin const * glo_pin)
{
  uint16_t or_pins = (input && glo_pin->polar == Polar_ActiveHigh) ||
    (!input && glo_pin->polar == Polar_ActiveLow);
  if (or_pins)
    gpio_write(gpio_read(glo_pin->port) | glo_pin->pin_bitmask, glo_pin->port);
  else
    gpio_write(gpio_read(glo_pin->port) & ~glo_pin->pin_bitmask, glo_pin->port);
}

/**
 * \brief Sets or clears the pin encapsulated by glo_pin. Pin is set electrically
 * high if input is non-zero; if input is zero, pin is set electrically
 * low.
 * Undefined behaviour occurs if the pin is configured to be an input.
 * \param [in] input If non-zero, pin is set high; else, pin is set low.
 * \param [in] glo_pin Pointer to GlobalPin struct
 */
SXC_INLINE void globalPin_write_raw(uint16_t input, GlobalPin const * glo_pin)
{
  if (input)
    gpio_write(gpio_read(glo_pin->port) | glo_pin->pin_bitmask, glo_pin->port);
  else
    gpio_write(gpio_read(glo_pin->port) & ~glo_pin->pin_bitmask, glo_pin->port);
}

#endif //_SXC_LIB_STRUCTS_H_
