/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file VPU.h
 *  \brief Interface for the XInC2 vector processing unit. Essentially, these
 *  are DSP-esque operations on very wide operands. NOT thread-safe: requires
 *  manual semaphore management to ensure only one thread utilizes the VPU
 *  at a time.
 */
#ifndef _SXC_LIB_VPU_H_
#define _SXC_LIB_VPU_H_

#include "SXC.h"
#include "register_consts.h"

/**
 * \defgroup VPU_config_registers
 * List of selectable VPU configuration registers
 * @{
 */
enum VPU_config_register {
  VPU_CFG0 = 0,
  VPU_CFG1 = 1,
  VPU_ADDRMASK = 2,
  kNumVPUCfgReg
};
/** @} */


/**
 * \defgroup VPU_CFG0_register_flags
 * List of bitwise-ORable flags suitable for configuring the VPU for the cfg0
 * register
 * @{
 */
enum VPU_CFG0_flag {
  VPU_SCALAR_MODE = 0x0,
  VPU_DMA_MODE = 0x1, //mutually exclusive with VPU_OPB_REG
  VPU_OPB_REG = 0x1 << 1, //mutually exclusive with VPU_DMA_MODE
  VPU_DEST_REG = 0x1 << 2,
  VPU_DEST_DMA = 0x0, //mutually exclusive with VPU_DEST_REG
  VPU_WRITE_NONE = 0x0,
  VPU_WRITE_LSW = 0x1 << 3,
  VPU_WRITE_MSW = 0x1 << 4,
  VPU_WRITE_EXT = VPU_WRITE_LSW | VPU_WRITE_MSW, //all write modes are mutually exclusive
  VPU_OPA_IS_MQ = 0x0,
  VPU_OPA_IS_OPA = 0x1 << 5, //mutually exclusive with VPU_OPA_IS_MQ
  VPU_OPB_IS_ACCQ = 0x0,
  VPU_OPB_IS_OPB = 0x1 << 6, //mutually exclusive with VPU_OPB_IS_ACCQ
  VPU_ACC_SRC_ASQ = 0x0,
  VPU_ACC_SRC_QMQ = 0x1 << 7,
  VPU_OPA_OUTER_ADDR_INC_BY_0 = 0x0,
  VPU_OPA_OUTER_ADDR_INC_BY_1 = 0x1 << 8,
  VPU_OPA_OUTER_ADDR_INC_BY_2 = 0x3 << 8, //OPA increments are mutually exclusive
  VPU_OPB_OUTER_ADDR_INC_BY_0 = 0x0,
  VPU_OPB_OUTER_ADDR_INC_BY_1 = 0x1 << 10, //mutually exclusive with other OPB inc
  VPU_RESULT_OUTER_ADDR_INC_BY_0 = 0x0,
  VPU_RESULT_OUTER_ADDR_INC_BY_1 = 0x1 << 11, //mutually exclusive with other OPB inc
  VPU_SHIFT_RESULT_LEFT = 0x1 << 12,
  VPU_SELECT_SUB = 0x1 << 13,
  VPU_AUTO_CLEAR_OPB = 0x1 << 14,
  VPU_CLEAR_ACC = 0x1 << 15,
};
/** @} */

/**
 * \defgroup VPU_CFG1_register_flags
 * List of bitwise-ORable flags suitable for configuring the VPU for the cfg1
 * register
 * @{
 */
enum VPU_CFG1_flag {
  VPU_OPA_UNSIGNED = 0x1,
  VPU_OPB_UNSIGNED = 0x2,
  VPU_FRACTIONAL_MODE = 0x4,
  VPU_ROUNDING_MODE = 0x4 | 0x8, //fractional mode MUST be on
  VPU_FRACTIONAL_SATURATION = 0x0,
  VPU_INTEGER_SATURATION = 0x10,
  VPU_NO_SATURATION = 0x20, //all saturation selections are mutually exclusive
  VPU_PIPELINING = 0x40,
};
/** @} */

static const uint16_t kVpuIntMswCfg = VPU_AUTO_CLEAR_OPB|
  VPU_RESULT_OUTER_ADDR_INC_BY_1|
  VPU_OPB_OUTER_ADDR_INC_BY_1|VPU_OPA_OUTER_ADDR_INC_BY_1|VPU_OPA_UNSIGNED|
  VPU_OPB_UNSIGNED;

static const uint16_t kVpuFracMswCfg = VPU_AUTO_CLEAR_OPB|
  VPU_RESULT_OUTER_ADDR_INC_BY_1|
  VPU_OPB_OUTER_ADDR_INC_BY_1|VPU_OPA_OUTER_ADDR_INC_BY_1|
  VPU_FRACTIONAL_SATURATION;

/**
 * \brief Determines whether the VPU is currently in use by a thread.
 * \return 0 if not running, 1 if running
 */
SXC_INLINE uint16_t vpu_is_running(void)
{
  return (_inp(VPUcfg1) & (0x1 << 15)) ? 1 : 0;
}

/**
 * \brief Sets up the operation to be carried out by the VPU in further function
 * calls. Operation is selected by bitwise-OR of configuration flags and the
 * operand address mask. Further details in XInC2 manual.
 * \param[in] cfg0 CFG0 register flags to set up the VPU operation.
 * \param[in] cfg1 CFG1 regiser flags to further configure the VPU operation.
 * \param[in] opadrmask Operand address mask. The lower byte refers to operand
 * A's mask, and the upper one to operand B's.
 */
SXC_INLINE void vpu_set_config(uint16_t cfg0, uint16_t cfg1, uint16_t opadrmask)
{
  _outp(cfg0, VPUcfg0);
  _outp(cfg1, VPUcfg1);
  _outp(opadrmask, VPUopadrmsk);
}

/**
 * \brief Returns the value (configuration) in the chosen VPU register.
 * \param[in] reg Desired VPU register.
 * \return The desired VPU register's configuration. 0 if invalid input argument.
 */
SXC_INLINE uint16_t vpu_get_config(enum VPU_config_register reg)
{
  safe_mode_assert(reg < kNumVPUCfgReg);
  uint16_t addr = 0;
  switch(reg) {
  case VPU_CFG0:
    addr = VPUcfg0;
    break;
  case VPU_CFG1:
    addr = VPUcfg1;
    break;
  case VPU_ADDRMASK:
    addr = VPUopadrmsk;
    break;
  default:
    break;
  }
  return _inp(addr);
  return 0;
}

/**
 * \brief Executes the operation configured in the VPU after vpu_setup is
 * called, with the specified operands. Further details in XInC2 manual.
 * \detailed All arguments must be pre-allocated. No bounds checking or
 * dynamic memory allocation is performed: BE VERY MEMORY-WARY.
 * \param[in] opA Array containing first operand values.
 * \param[in] opA_len Length of the aforementioned array.
 * \param[in] opB Array containing second operand values.
 * \param[in] opB_len Length of the aforementioned array.
 * \param[in] res Array to store the results of the vector operation.
 * \param[in] res_len Length of the aforementioned array.
 * \return Returns res (pointer to first element of result array).
 */
SXC_INLINE uint16_t* vpu_start(uint16_t *opA, uint16_t opA_len,
                           uint16_t *opB, uint16_t opB_len,
                           uint16_t *res, uint16_t res_len)
{
  _outp( ((opA_len - 1) << 8)|(opB_len - 1), VPUopadrmsk); //pure magic, no docs
  _outp( res_len - 1, VPUrsadrsmk);
  _outp((uint16_t)opA, VPUopAba);
  _outp((uint16_t)opB, VPUopBba);
  _outp((res_len << 8)|1, VPUfilterLength); //not sure why the outer loop len is this
  uint16_t result_addr = (uint16_t)res;
  _outp(result_addr, VPUrsba);
  _outp(result_addr, VPUopA); //no idea what is with these last 3 lines:
                              //some sort of micro-optimization
}

#endif //SXC_LIB_VPU_H_
