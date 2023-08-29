/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file GPIO.h
 *  \brief Interface for accessing the GPIO ports on the XInC2 processor.
 *  These functions are NOT thread-safe: manual semaphore management is required.
 *  See the XInC2 manual for hardware layout for a much better understanding.
 */
#ifndef _SXC_LIB_GPIO_H_
#define _SXC_LIB_GPIO_H_

#include "Semaphore.h"

/**
 * \defgroup GPIO_port_names
 * List of available GPIO ports
 * @{
 */
enum GPIO_port_name {
  GPIO_A = 0,
  GPIO_B = 1,
  GPIO_C = 2,
  GPIO_D = 3,
  GPIO_E = 4,
  GPIO_F = 5,
  GPIO_G = 6,
  GPIO_H = 7,
  GPIO_I = 8,
  GPIO_J = 9,
  kNumGPIOPorts
};
/** @} */

/**
 * \brief Configures the selected GPIO port pins' directionality based on the
 * provided configuration. CLOBBERS THE OUTPUT BUFFER REGISTER (LSB). Call
 * gpio_set_config((gpio_get_config() & 0xFF) | cfg) to avoid this, if you care.
 * \param[in] cfg Desired pin configuration. Lower byte clobbers output buffer
 * register. Set bits in the upper byte cause that pin to be an output;
 * cleared bits correlate to input pins. Because this is the MSB,
 * bit8 of the entire word corresponds to pin0, bit9 to pin1, and so on.
 * \param[in] port The selected GPIO port.
 */
SXC_INLINE void gpio_set_config(uint16_t cfg, enum GPIO_port_name port)
{
  safe_mode_assert(port < kNumGPIOPorts);
  switch(port){
  case GPIO_A:
    _outp(cfg, GPAcfg);
    break;
  case GPIO_B:
    _outp(cfg, GPBcfg);
    break;
  case GPIO_C:
    _outp(cfg, GPCcfg);
    break;
  case GPIO_D:
    _outp(cfg, GPDcfg);
    break;
  case GPIO_E:
    _outp(cfg, GPEcfg);
    break;
  case GPIO_F:
    _outp(cfg, GPFcfg);
    break;
  case GPIO_G:
    _outp(cfg, GPGcfg);
    break;
  case GPIO_H:
    _outp(cfg, GPHcfg);
    break;
  case GPIO_I:
    _outp(cfg, GPIcfg);
    break;
  case GPIO_J:
    _outp(cfg, GPJcfg);
    break;
  default:
    break;
  }
}

/**
 * \brief Returns the selected GPIO port pins' current directionality.
 * \param[in] port The selected GPIO port.
 * \return Desired pin's configuration at time of function call. Set bits
 * correspond to output pins, and cleared bits correspond to input pins.
 */
SXC_INLINE uint16_t gpio_get_config(enum GPIO_port_name port)
{
  safe_mode_assert(port < kNumGPIOPorts);
  switch(port){
  case GPIO_A:
    return _inp(GPAcfg);
  case GPIO_B:
    return _inp(GPBcfg);
  case GPIO_C:
    return _inp(GPCcfg);
  case GPIO_D:
    return _inp(GPDcfg);
  case GPIO_E:
    return _inp(GPEcfg);
  case GPIO_F:
    return _inp(GPFcfg);
  case GPIO_G:
    return _inp(GPGcfg);
  case GPIO_H:
    return _inp(GPHcfg);
  case GPIO_I:
    return _inp(GPIcfg);
  case GPIO_J:
    return _inp(GPJcfg);
  default:
    break;
  }
  return 0;
}

/**
 * \brief Reads the logical values on the pins of the selected GPIO port.
 * \param[in] port The GPIO port to target.
 * \return The selected GPIO port's pins' logical values. Returns 0 for an
 * invalid GPIO.
 */
SXC_INLINE uint16_t gpio_read(enum GPIO_port_name port)
{
  safe_mode_assert(port < kNumGPIOPorts);
  switch(port){
  case GPIO_A:
    return _inp(GPAin);
  case GPIO_B:
    return _inp(GPBin);
  case GPIO_C:
    return _inp(GPCin);
  case GPIO_D:
    return _inp(GPDin);
  case GPIO_E:
    return _inp(GPEin);
  case GPIO_F:
    return _inp(GPFin);
  case GPIO_G:
    return _inp(GPGin);
  case GPIO_H:
    return _inp(GPHin);
  case GPIO_I:
    return _inp(GPIin);
  case GPIO_J:
    return _inp(GPJin);
  default:
    break;
  }
  return 0;
}

/**
 * \brief Writes the individual bit values of the input to their corresponding
 * pins in the selected GPIO port. Only pins configured to be outputs are
 * affected. Only the lower byte of the input matters.
 * \param[in] input Lower byte: Bit values to write to corresponding pins.
 * \param[in] port The GPIO port to target.
 */
SXC_INLINE void gpio_write(uint16_t input, enum GPIO_port_name port)
{
  safe_mode_assert(port < kNumGPIOPorts);
  switch(port){
  case GPIO_A:
    _outp(input, GPAout);
    break;
  case GPIO_B:
    _outp(input, GPBout);
    break;
  case GPIO_C:
    _outp(input, GPCout);
    break;
  case GPIO_D:
    _outp(input, GPDout);
    break;
  case GPIO_E:
    _outp(input, GPEout);
    break;
  case GPIO_F:
    _outp(input, GPFout);
    break;
  case GPIO_G:
    _outp(input, GPGout);
    break;
  case GPIO_H:
    _outp(input, GPHout);
    break;
  case GPIO_I:
    _outp(input, GPIout);
    break;
  case GPIO_J:
    _outp(input, GPJout);
    break;
  default:
    break;
  }
}

/**
 * \brief Atomically sets the desired pin in the desired port to 1.
 * \param[in] pin_num The pin to set
 * \param[in] port The GPIO port to target.
 * \param[in] sem_num Semaphore to acquire for atomicity.
 */
SXC_INLINE void gpio_atomic_pin_set(uint16_t pin_num, enum GPIO_port_name port,
  uint16_t sem_num)
{
  sem_lock(sem_num);
  uint16_t value = gpio_read(port);
  gpio_write(value|(1 << pin_num), port);
  sem_unlock(sem_num);
}

/**
 * \brief Atomically sets the desired pin in the desired port to 0.
 * \param[in] pin_num The pin to set
 * \param[in] port The GPIO port to target.
 * \param[in] sem_num Semaphore to acquire for atomicity.
 */
SXC_INLINE void gpio_atomic_pin_unset(uint16_t pin_num, enum GPIO_port_name port,
  uint16_t sem_num)
{
  sem_lock(sem_num);
  uint16_t value = gpio_read(port);
  gpio_write(value&~(1 << pin_num), port);
  sem_unlock(sem_num);
}

/**
 * \brief Retrieve config register corresponding to given port.
 * \param[in] port The port
 */
SXC_CONSTEXPR uint16_t gpio_get_config_reg(enum GPIO_port_name port)
{
  return (port == GPIO_A) ? GPAcfg :
  (port == GPIO_B) ? GPBcfg :
  (port == GPIO_C) ? GPCcfg :
  (port == GPIO_D) ? GPDcfg :
  (port == GPIO_E) ? GPEcfg :
  (port == GPIO_F) ? GPFcfg :
  (port == GPIO_G) ? GPGcfg :
  (port == GPIO_H) ? GPHcfg :
  (port == GPIO_I) ? GPIcfg :
  (port == GPIO_J) ? GPJcfg :
  0;
}

/**
 * \brief Retrieve output register corresponding to given port.
 * \param[in] port The port
 */
SXC_CONSTEXPR uint16_t gpio_get_output_reg(enum GPIO_port_name port)
{
  return (port == GPIO_A) ? GPAout :
  (port == GPIO_B) ? GPBout :
  (port == GPIO_C) ? GPCout :
  (port == GPIO_D) ? GPDout :
  (port == GPIO_E) ? GPEout :
  (port == GPIO_F) ? GPFout :
  (port == GPIO_G) ? GPGout :
  (port == GPIO_H) ? GPHout :
  (port == GPIO_I) ? GPIout :
  (port == GPIO_J) ? GPJout :
  0;
}

/**
 * \brief Retrieve input register corresponding to given port.
 * \param[in] port The port
 */
SXC_CONSTEXPR uint16_t gpio_get_input_reg(enum GPIO_port_name port)
{
  return (port == GPIO_A) ? GPAin :
  (port == GPIO_B) ? GPBin :
  (port == GPIO_C) ? GPCin :
  (port == GPIO_D) ? GPDin :
  (port == GPIO_E) ? GPEin :
  (port == GPIO_F) ? GPFin :
  (port == GPIO_G) ? GPGin :
  (port == GPIO_H) ? GPHin :
  (port == GPIO_I) ? GPIin :
  (port == GPIO_J) ? GPJin :
  0;
}

#endif //SXC_LIB_GPIO_H_
