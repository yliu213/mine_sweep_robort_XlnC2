/*                  Copyright Eleven Engineering 2014                         */
/* Author: Sam Cristall
 * License: GPLv3
 */
/** \file register_consts.h
 *  \brief Defines symbolic names for all register magic values. Provides a
 *  global array for non-XInC2 compilation targets (emulation).
 */
#ifndef _SXC_LIB_REG_H_
#define _SXC_LIB_REG_H_

#include <stdint.h>

/*                    XInC2 Special Function Registers                        */
static const uint16_t
    // SCU
    SCUreg = 0x00,
    SCUpc = 0x01, SCUcc = 0x02, SCUtime = 0x03, SCUpntr = 0x03, SCUbkpt = 0x04,
    SCUstop = 0x04, SCUwait = 0x05, SCUiopageWR = 0x05, SCUrsrc = 0x06,
    SCUup = 0x06, SCUiopageRD = 0x07, SCUdown = 0x07,

    // LFSR0
    LFSR0cfg = 0x08, LFSR0tapcfg = 0x09, LFSR0reg = 0x0A, LFSR0data = 0x0B,

    // LFSR1
    LFSR1cfg = 0x0C, LFSR1tapcfg = 0x0D, LFSR1reg = 0x0E, LFSR1data = 0x0F,

    // VPU
    VPUcfg0 = 0x10, VPUcfg1 = 0x11, VPUopAba = 0x12, VPUopBba = 0x13,
    VPUopadrmsk = 0x14, VPUopA = 0x15, VPUopB = 0x16, VPUrsba = 0x17,
    VPUrsadrsmk = 0x18, VPUrslsw = 0x19, VPUrsmsw = 0x1A, VPUrsext = 0x1B,
    VPUfilterLength = 0x1C,

    // SPI0
    SPI0rx = 0x34, SPI0tx = 0x34, SPI0cfg = 0x35,

    // SPI1
    SPI1rx = 0x36, SPI1tx = 0x36, SPI1cfg = 0x37,

    // SFU
    SFUrev = 0x38, SFUpack = 0x39, SFUpop = 0x3A, SFUls1 = 0x3B,

    // TimerA
    TMRAcfg = 0x3E, TMRAcount = 0x3F, TMRACCR0 = 0x40, TMRACCM0cfg = 0x41,
    TMRACCR1 = 0x42, TMRACCM1cfg = 0x43, TMRACCR2 = 0x44, TMRACCM2cfg = 0x45,
    TMRACCR3 = 0x46, TMRACCM3cfg = 0x47,

    // GPIO
    GPAin = 0x20, GPAout = 0x20, GPAcfg = 0x21, GPBin = 0x22, GPBout = 0x22,
    GPBcfg = 0x23, GPCin = 0x24, GPCout = 0x24, GPCcfg = 0x25, GPDin = 0x26,
    GPDout = 0x26, GPDcfg = 0x27, GPEin = 0x28, GPEout = 0x28, GPEcfg = 0x29,
    GPFin = 0x2A, GPFout = 0x2A, GPFcfg = 0x2B, GPGin = 0x2C, GPGout = 0x2C,
    GPGcfg = 0x2D, GPHin = 0x2E, GPHout = 0x2E, GPHcfg = 0x2F, GPIin = 0x30,
    GPIout = 0x30, GPIcfg = 0x31, GPJin = 0x32, GPJout = 0x32, GPJcfg = 0x33;


/*                      I/O Pl Addresses for iopage = 0                       */
static const uint16_t
    // TimerB
    TMRBcfg = 0x48,
    TMRBcount = 0x49, TMRBCCR0 = 0x4A, TMRBCCM0cfg = 0x4B, TMRBCCR1 = 0x4C,
    TMRBCCM1cfg = 0x4D,

    // BBU0
    BBU0cfg0 = 0x58, BBU0cfg1 = 0x59, BBU0tx = 0x5A, BBU0rx = 0x5A,
    BBU0brg = 0x5B, BBU0time = 0x5C, BBU0rx4 = 0x5D, BBU0rx6 = 0x5E,
    BBU0start = 0x5F,

    // BBU1
    BBU1cfg0 = 0x60, BBU1cfg1 = 0x61, BBU1tx = 0x62, BBU1rx = 0x62,
    BBU1brg = 0x63, BBU1time = 0x64, BBU1flow = 0x65, BBU1rx4 = 0x65,
    BBU1start = 0x67,

    // DASI
    DASIconfig0 = 0x68, DASIconfig1 = 0x69, DASIconfig2 = 0x6A,
    DASIstatus0 = 0x6B, DASIstatus1 = 0x6C, DASIstatus2 = 0x6D, DASIrx0 = 0x6B,
    DASIrx1 = 0x6C, DASIrx2 = 0x6D, DASItx0 = 0x6B, DASItx1 = 0x6C,
    DASItx2 = 0x6D, DASIcount0 = 0x6E, DASIcount1 = 0x6F,

    // SCX (Supervisory Control Extensions)
    SCXioCfgP = 0x70, SCXioCfgD = 0x71, SCXclkCfg = 0x72, SCXaltCfg = 0x73,
    SCXPLLCfg0 = 0x74, SCXPLLCfg1 = 0x75, SCXmemcol = 0x76, SCXcount = 0x77,

    // SMU
    SMUcfg0 = 0x78, SMUcfg1 = 0x79,

    // ADC
    ADCcfg0 = 0x7D, ADCcfg1 = 0x7E, ADCdata = 0x7F;


/*                      I/O Pl Addresses for iopage = 1                       */
static const uint16_t
    // Difference Quantizer0
    Quant0cfg = 0x48,
    Quant0pred = 0x49, Quant0dec = 0x49, Quant0step = 0x4A, Quant0delta = 0x4A,
    Quant0samp = 0x4B,

    // Difference Quantizer1
    Quant1cfg = 0x4C, Quant1pred = 0x4D, Quant1dec = 0x4D, Quant1step = 0x4E,
    Quant1delta = 0x4E, Quant1samp = 0x4F,

    // Difference Quantizer2
    Quant2cfg = 0x50, Quant2pred = 0x51, Quant2dec = 0x51, Quant2step = 0x52,
    Quant2delta = 0x52, Quant2samp = 0x53,

    // Difference Quantizer3
    Quant3cfg = 0x54, Quant3pred = 0x55, Quant3dec = 0x55, Quant3step = 0x56,
    Quant3delta = 0x56, Quant3samp = 0x57,

    // ADPCM Inverse Difference Quantizer
    InvQuant0cfg = 0x58, InvQuant0delta = 0x59, InvQuant0vpdiff = 0x59,
    InvQuant0step = 0x5A,

    // ADPCM Inverse Difference Quantizer
    InvQuant1cfg = 0x5C, InvQuant1delta = 0x5D, InvQuant1vpdiff = 0x5D,
    InvQuant1step = 0x5E,

    // ADPCM Inverse Difference Quantizer
    InvQuant2cfg = 0x60, InvQuant2delta = 0x61, InvQuant2vpdiff = 0x61,
    InvQuant2step = 0x62,

    // ADPCM Inverse Difference Quantizer
    InvQuant3cfg = 0x64, InvQuant3delta = 0x65, InvQuant3vpdiff = 0x65,
    InvQuant3step = 0x66,

    // Accumulator0
    Accum0cfg = 0x68, Accum0high = 0x69, Accum0low = 0x6A,
    Accum0addsubhigh = 0x6B, Accum0addlow = 0x6C, Accum0sublow = 0x6D,

    // Accumulator1
    Accum1cfg = 0x6E, Accum1high = 0x6F, Accum1low = 0x70,
    Accum1addsubhigh = 0x71, Accum1addlow = 0x72, Accum1sublow = 0x73;

#endif //_SXC_LIB_REG_H_
