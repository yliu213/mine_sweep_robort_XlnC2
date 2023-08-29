//
// Created by ai on 2023/4/6.
//

#ifndef SRC_UART_SELF_H
#define SRC_UART_SELF_H

#endif //SRC_UART_SELF_H
#include "UART.h"
#include "GPIO.h"
#include "XPD.h"


int main() {
    struct GlobalPin pin1 = {GPIO_C,io_PC0,0x24,Polar_ActiveLow};
    struct GlobalPin pin2 = {GPIO_C,io_PC1,0x24,Polar_ActiveLow};
struct UART_Pin_Pair Cport = {pin1,pin2,9600}; //period???//

while (1) {
         xpd_echo_int(uart_read_byte(&Cport), XPD_Flag_UnsignedDecimal);
         xpd_puts("... \n");
        }
}