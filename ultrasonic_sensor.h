//
// Created by ai on 2023/4/6.
//

#ifndef SRC_ULTRASONIC_SENSOR_H
#define SRC_ULTRASONIC_SENSOR_H

#endif //SRC_ULTRASONIC_SENSOR_H
// ultrasonic sensor code//
//PD0 to echo, PC3 to trig//
#include "SystemClock.h"
#include "XPD.h"
#include "GPIO.h"
#include "register_consts.h"
#include "Thread.h"

// sys_clock_wait can only wait a maximum of 65535 ticks
// use a loop to get a longer delay.


void wait_10us()
{
    for (int i = 0; i < 98; ++i) {
        sys_clock_wait(10);
    }
//    for (int i = 0; i < 1; ++i) {
//        sys_clock_wait(304);
//   }
}
void wait_1ms(){ //actually is 1s//
    for(int i = 0; i < 9830; ++i) {
        sys_clock_wait(10000);
    }
    for(int i = 0; i < 4; ++i) {
        sys_clock_wait(1000);
    }
}
void wait_200ms(){
    for(int i = 0; i < 1966; ++i) {
        sys_clock_wait(10000);
    }
        sys_clock_wait(800);
}
//measure state of Echo pin//
void *button_reading_thread(void *)
{
    // Thread 1 loop for PD0//
    while (true) {
        uint16_t t;
        uint16_t distance;
        uint16_t port_d_state = gpio_read(GPIO_D);
        bool PD0state = (port_d_state & (1<<0));
        bool timed_out = t > 2353;
        if (!PD0state || timed_out) { //pin is low//
            distance = (t*17*10)/15;//formula for distance in mm//
            if(timed_out){ //max distance of 4m//
                xpd_puts("Out of range");
            }else {
                xpd_puts("distance: ");
                xpd_echo_int(distance, XPD_Flag_UnsignedDecimal);
                xpd_puts(" mm \n ");
            }
            t=0;
            wait_1ms(); //use a delay to wait for some time//

            //re-send signal//
            gpio_set_config((0x01 << 11), GPIO_C);
            gpio_write((0xff), GPIO_C);
            wait_10us();
            gpio_write((0x00), GPIO_C);

            port_d_state = gpio_read(GPIO_D);
            PD0state = (port_d_state & (1<<0));
            while(!PD0state){
                port_d_state = gpio_read(GPIO_D);
                PD0state = (port_d_state & (1<<0));
            }

        } else {
            //pin is high//
            //wait for 10us and increment t//
            wait_10us();
            ++t;  //10us per t//
        }
    }
}
void initialization(){
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    //set PD4 as input, receive PWM from Echo pin
    //default setting, PD0 low
    gpio_set_config((0x00 << 8), GPIO_D);
    gpio_write((0x00), GPIO_D);

    thread_setup(button_reading_thread, NULL, 1);
    thread_run(1);
    gpio_set_config((0x01 << 11), GPIO_C);
    gpio_write((0xff), GPIO_C);
    wait_10us();
    gpio_write((0x00), GPIO_C);
}
//int main() {
//    // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
//    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
//
//    //set PD4 as input, receive PWM from Echo pin
//    //default setting, PD0 low
//    gpio_set_config((0x00 << 8), GPIO_D);
//    gpio_write((0x00), GPIO_D);
//
//    thread_setup(button_reading_thread, NULL, 1);
//    thread_run(1);
//    gpio_set_config((0x01 << 11), GPIO_C);
//    gpio_write((0xff), GPIO_C);
//    wait_10us();
//    gpio_write((0x00), GPIO_C);
//
//    while (1) {
//        //First part: set PC3 as output, send 10us pulse to Tirg pin
//
//
//        //Second part: find duration of pulse from Echo pin//
//        //count the number of ticks = exact_time(s) * 98304MHz//
//        // not sure which function to use to count # of ticks//
//        //6144 ticks/ms//
//
//    }
//
//}