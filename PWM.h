//
// Created by Yongqing Liu on 2023/4/6.
//

#ifndef SRC_PWM_H
#define SRC_PWM_H
#endif //SRC_PWM_H

#include "SystemClock.h"
#include "GPIO.h"

// sys_clock_wait can only wait a maximum of 65535 ticks
// use a loop to get a longer delay.
void long_wait()
{
    for (int i = 0; i < 5000; ++i) {
        sys_clock_wait(10000);
    }
}
void delay(){
    for(int i =0;i<5;++i){
        long_wait();
    }
}
void right_and_re_adjust() {
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    gpio_set_config((0x01 << 12), GPIO_C);
    gpio_write((0xff), GPIO_C);
    for (int i = 0; i < 186; ++i) {   //turn right//
        sys_clock_wait(1000);
    }
    gpio_write((0x00), GPIO_C);
    for (int a = 0; a < 2; ++a) {
        long_wait();
    }
    for (int m = 0; m < 20; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_C);
        gpio_write((0xff), GPIO_C);
        for (int i = 0; i < (146 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_C);
        for (int y = 0; y < (1573 - (146 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}
void right(){
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    gpio_set_config((0x01 << 12), GPIO_C);
    gpio_write((0xff), GPIO_C);
    for (int i = 0; i < 186; ++i) {   //turn right//
        sys_clock_wait(1000);
    }
    gpio_write((0x00), GPIO_C);
    for (int a = 0; a < 2; ++a) {
        long_wait();
    }
}
void right_re_ajust(){
    for (int m = 0; m < 20; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_C);
        gpio_write((0xff), GPIO_C);
        for (int i = 0; i < (146 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_C);
        for (int y = 0; y < (1573 - (146 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}
void left(){
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    gpio_set_config((0x01 << 12), GPIO_C);
    gpio_write((0xff), GPIO_C);
    for (int i = 0; i < 103; ++i) {   //turn left//
        sys_clock_wait(1000);
    }
    gpio_write((0x00), GPIO_C);
    for (int a = 0; a < 2; ++a) {
        long_wait();
    }
}
void left_re_adjust(){
    for (int m = 0; m < 13; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_C);
        gpio_write((0xff), GPIO_C);
        for (int i = 0; i < (146 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_C);
        for (int y = 0; y < (1573 - (146 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}
void left_and_re_adjust(){
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    gpio_set_config((0x01 << 12), GPIO_C);
    gpio_write((0xff), GPIO_C);
    for (int i = 0; i < 103; ++i) {   //turn left//
        sys_clock_wait(1000);
    }
    gpio_write((0x00), GPIO_C);
    for (int a = 0; a < 2; ++a) {
        long_wait();
    }
    for (int m = 0; m < 20; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_C);
        gpio_write((0xff), GPIO_C);
        for (int i = 0; i < (146 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_C);
        for (int y = 0; y < (1573 - (146 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}

void forward(){ //for 7 times//
    for (int n = 146; n < 162; ++n) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_D);
        gpio_write((0xff), GPIO_D);
        for (int i = 0; i < n; ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_D);
        for (int k = 0; k < (1573 - n); ++k) {
            sys_clock_wait(1000);
        }
    }
    //reverse duty,turn left//
    for (int m = 0; m < 16; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_D);
        gpio_write((0xff), GPIO_D);
        for (int i = 0; i < (162 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_D);
        for (int y = 0; y < (1573 - (162 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}
void backward(){ //for 2 times //
    for (int m = 0; m < 25; ++m) {
        // Configure system clock for 98.304 MHz from a 12.288 MHz xtal
        sys_clock_init(crys_12_288_MHz, _98_304_MHz);
        gpio_set_config((0x01 << 12), GPIO_D);
        gpio_write((0xff), GPIO_D);
        for (int i = 0; i < (146 - m); ++i) {
            sys_clock_wait(1000);
        }
        // to ensure each cycle is 20ms//
        gpio_write((0x00), GPIO_D);
        for (int y = 0; y < (1573 - (146 - m)); ++y) {
            sys_clock_wait(1000);
        }
    }
}
void left_forward(){
    left();
    for(int i =0;i<4;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    left_re_adjust();
}
void right_forward(){
    right();
    for(int i =0;i<5;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    right_re_ajust();
}
void path(){
    for(int i =0;i<7;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    right_forward();
    for(int i =0;i<5;++i){
        long_wait();
    }
    for(int i =0;i<2;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    right_forward();
    for(int i =0;i<5;++i){
        long_wait();
    }
    for(int i =0;i<7;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    left_forward();
    for(int i =0;i<5;++i){
        long_wait();
    }
    for(int i =0;i<2;++i) {
        forward();
    }
    for(int i =0;i<5;++i){
        long_wait();
    }
    left_forward();
}
void avoid(){
    for (int a = 0; a < 5; ++a) {
        backward();
    }
    delay();
    right_forward();
    delay();
    for (int a = 0; a < 3; ++a) {
        forward();
    }
    delay();
    left_forward();
    delay();
    for (int a = 0; a < 7; ++a) {
        forward();
    }
}