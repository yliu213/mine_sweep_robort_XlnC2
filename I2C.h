/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file I2C.h
 *  \brief Convenience functions and implementation using 2 arbitrary GPIO pins
 *  in order to utilize the I2C protocol.
 */
#ifndef _SXC_LIB_I2C_H_
#define _SXC_LIB_I2C_H_

#include "SXC.h"
#include "GPIO.h"
#include "SystemClock.h"
#include "Structs.h"

//enum I2C_Mode {
//  I2C_Master,
//  I2C_Slave
//};

typedef struct I2C_Pin_Pair {
  GlobalPin const sda;
  GlobalPin const scl;
  uint16_t const addr;
  uint16_t const timeout;
} I2C_Pin_Pair;

/**
 * \brief Initializes the state of the I2C pin pair to a starting/recoverable
 * state.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 */
SXC_INLINE void i2c_init(I2C_Pin_Pair const * pins)
{
  globalPin_set_dir(PinDir_Input, &pins->scl);
  globalPin_set_dir(PinDir_Input, &pins->sda);
}

/**
 * \brief Writes the START sequence onto the I2C line.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 */
SXC_INLINE void i2c_write_start(I2C_Pin_Pair const * pins)
{
  while(!globalPin_read_raw(&pins->scl));
  globalPin_set_dir(PinDir_Output, &pins->sda);
  globalPin_write_raw(0, &pins->sda);
  internal_busy_wait(2*pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Output, &pins->scl);
  globalPin_write_raw(0, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->sda);
}

/**
 * \brief Writes the STOP sequence onto the I2C line.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 */
SXC_INLINE void i2c_write_stop(I2C_Pin_Pair const * pins)
{
  globalPin_set_dir(PinDir_Output, &pins->sda);
  globalPin_set_dir(PinDir_Output, &pins->scl);
  globalPin_write_raw(0, &pins->sda);
  globalPin_write_raw(0, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->scl);
  internal_busy_wait(2*pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->sda);
}

/**
 * \brief Writes an 8-bit byte (the LSB of byte) onto the I2C line. Returns 1 if
 * an ACK is read on the line after writing, returns 0 else. May only be called
 * if the device is set as an I2C master; otherwise, undefined behaviour occurs.
 * \param [in] byte 16-bit word whose LSB is written onto the I2C line. MSB is
 * ignored.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 * \return 1 if an ACK is read on the line after writing, 0 else
 */
SXC_INLINE uint16_t i2c_write_byte(uint16_t byte, I2C_Pin_Pair const * pins)
{
  for (int16_t i = 7; i >= 0; --i) {
    if(_rol(1, i) & byte) {
      globalPin_set_dir(PinDir_Input, &pins->sda);
    }
    else {
      globalPin_set_dir(PinDir_Output, &pins->sda);
      globalPin_write_raw(0, &pins->sda);
    }
    internal_busy_wait(pins->timeout-kRawWriteSysTicks);

    globalPin_set_dir(PinDir_Input, &pins->scl);
    internal_busy_wait(2*pins->timeout-kRawWriteSysTicks);
    globalPin_set_dir(PinDir_Output, &pins->scl);
    globalPin_write_raw(0, &pins->scl);
  }

  globalPin_set_dir(PinDir_Input, &pins->sda);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);

  while(!globalPin_read_raw(&pins->scl));

  uint16_t retVal = (uint16_t)(!globalPin_read_raw(&pins->sda));
  internal_busy_wait(pins->timeout-kRawReadSysTicks);
  globalPin_set_dir(PinDir_Output, &pins->scl);
  globalPin_write_raw(0, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  return retVal;
}

/**
 * \brief Reads an 8-bit byte from the I2C line. May only be called if the device
 * is set as an I2C master; else, undefined behaviour occurs.
 * \param [in] nack If non-zero, respond with a "NACK" on the line after reading.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 * \return The 8-bit byte in the LSB of a 16-bit word
 */
SXC_INLINE uint16_t i2c_read_byte(uint16_t nack, I2C_Pin_Pair const * pins)
{
  uint16_t byte = 0;
  for (int16_t i = 7; i >= 0; i--) {
    globalPin_set_dir(PinDir_Input, &pins->scl);
    while (!globalPin_read_raw(&pins->scl));
    internal_busy_wait(pins->timeout-kRawReadSysTicks);
    byte += byte + globalPin_read_raw(&pins->sda);
    globalPin_set_dir(PinDir_Output, &pins->scl);
    globalPin_write_raw(0, &pins->scl);
    internal_busy_wait(pins->timeout-kRawReadSysTicks);
  }

  if (nack) {
    globalPin_set_dir(PinDir_Input, &pins->sda);
  }
  else {
    globalPin_set_dir(PinDir_Output, &pins->sda);
    globalPin_write_raw(0, &pins->sda);
  }

  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);

  while (!globalPin_read_raw(&pins->scl));

  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Output, &pins->scl);
  globalPin_write_raw(0, &pins->scl);
  internal_busy_wait(pins->timeout-kRawWriteSysTicks);
  globalPin_set_dir(PinDir_Input, &pins->sda);
  return byte;
}

/**
 * \brief Writes an 8-bit byte (the LSB of byte) to the device with the specified
 * address. Returns 1 if the operation succeeds and is ACKed at all stages; 0
 * else. Function may only be called if the device is set as an I2C master;
 * otherwise, undefined behaviour occurs.
 * \param [in] dest_addr 16-bit word whose LSB is the I2C address to write to.
 * \param [in] byte 16-bit word whose LSB is written. MSB is ignored.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 * \return 1 if all expected ACKs are read on the line after writing, 0 else
 */
SXC_INLINE uint16_t i2c_write_to_addr(uint16_t dest_addr, uint16_t byte,
  I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  if (!i2c_write_byte(dest_addr, pins))
    goto Abort;
  if (!i2c_write_byte(byte, pins))
    goto Abort;
  i2c_write_stop(pins);
  return 1;

  Abort:
  i2c_init(pins);
  return 0;
}

/**
 * \brief Writes an 8-bit byte (the LSB of byte) to the register of the device
 * with the specified addresses.
 * Returns 1 if the operation succeeds and is ACKed at all stages; 0 else.
 * Function may only be called if the device is set as an I2C master;
 * otherwise, undefined behaviour occurs.
 * \param [in] dest_addr 16-bit word whose LSB is the I2C address to write to.
 * \param [in] dest_reg 16-bit word whose LSB is the address of the register to
 * write to.
 * \param [in] byte 16-bit word whose LSB is written. MSB is ignored.
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 * \return 1 if all expected ACKs are read on the line after writing, 0 else
 */
SXC_INLINE uint16_t i2c_write_to_register(uint16_t dest_addr, uint16_t dest_reg,
  uint16_t byte, I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  if (!i2c_write_byte(dest_addr, pins))
    goto Abort;
  if (!i2c_write_byte(dest_reg, pins))
    goto Abort;
  if (!i2c_write_byte(byte, pins))
    goto Abort;
  i2c_write_stop(pins);
  return 1;

  Abort:
  i2c_init(pins);
  return 0;
}

/**
 * \brief Writes an array of 8-bit bytes (the LSBs of the array elements) to the
 * register of the device with the specified addresses. No array bound checks
 * are done.
 * Returns 1 if the operation succeeds and is ACKed at all stages; 0 else.
 * Function may only be called if the device is set as an I2C master;
 * otherwise, undefined behaviour occurs.
 * \param [in] dest_addr 16-bit word whose LSB is the I2C address to write to.
 * \param [in] dest_reg 16-bit word whose LSB is the address of the register to
 * write to.
 * \param [in] buf Array of 16-bit words whose LSBs are written. MSBs are ignored.
 * \param [in] len Length of the array (number of bytes to write)
 * \param [in] pins Pointer to the I2C_Pin_Pair of interest
 * \return 1 if all expected ACKs are read on the line after writing, 0 else
 */
SXC_INLINE uint16_t i2c_write_buf_to_register(uint16_t dest_addr,
  uint16_t dest_reg, uint16_t* buf, size_t len, I2C_Pin_Pair const * pins)
{
  i2c_write_start(pins);
  if (!i2c_write_byte(dest_addr, pins))
    goto Abort;
  if (!i2c_write_byte(dest_reg, pins))
    goto Abort;
  for (size_t i = 0; i < len ; i++) {
    if (!i2c_write_byte(buf[i], pins))
      goto Abort;
  }
  i2c_write_stop(pins);
  return 1;

  Abort:
  i2c_init(pins);
  return 0;
}
#endif //_SXC_LIB_I2C_H_
