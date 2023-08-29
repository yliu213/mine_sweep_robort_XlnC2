//
// Created by ai on 2023/4/6.
//

#ifndef SRC_METAL_DETECTOR_H
#define SRC_METAL_DETECTOR_H

#endif //SRC_METAL_DETECTOR_H
//code for metal detector//
//connection: MCU Vcc to input, MCU ground to ground, PC2 to buzzer +//
//detected toggles PA0 LED//
#include "GPIO.h"
#include "Thread.h"
#include "register_consts.h"

// Thread that reads the state of a button and turns on an LED if the button
// is pressed.
// Threads must return (void *) and take an argument of (void *)
void *button_reading_thread(void *)
{
    // Thread 1 loop
    while (true) {
    uint16_t port_c_state = gpio_read(GPIO_F);
    bool is_button_pressed = (port_c_state & (1<<1));
    if (is_button_pressed) {
    } else { //goes high
        break;
    }
    }
}
void metal_detector_initialization(){
    // Set pin PC0 as input
    // Pins are set as an output by setting a 1 in position N+8, where N is
    // the GPIO pin number
    gpio_set_config((0x00 << 12), GPIO_C);

    // Set pin PA0 as output
    // Pins are set as an output by setting a 1 in position N+8, where N is
    // the GPIO pin number.
    gpio_set_config((0x01 << 8), GPIO_A);

    // Configure thread 1
    thread_setup(button_reading_thread, NULL, 1);

    // Start thread 1
    thread_run(1);

}
//    gpio_set_config((0x00 << 9), GPIO_F);
//    uint16_t port_c_state = gpio_read(GPIO_F);
//    bool is_button_pressed = (port_c_state & (1<<1));
//    if (is_button_pressed) {
//        xpd_puts("detected\n");
//    } else { //goes high
//        xpd_puts("nothing\n");
//    }

// main() runs in thread 0
//int main(void)
//{
//    // Set pin PC0 as input
//    // Pins are set as an output by setting a 1 in position N+8, where N is
//    // the GPIO pin number
//    gpio_set_config((0x00 << 12), GPIO_C);
//
//    // Set pin PA0 as output
//    // Pins are set as an output by setting a 1 in position N+8, where N is
//    // the GPIO pin number.
//    gpio_set_config((0x01 << 8), GPIO_A);
//
//    // Configure thread 1
//    thread_setup(button_reading_thread, NULL, 1);
//
//    // Start thread 1
//    thread_run(1);
//
//    return 0;
//}