#include "GPIO.h"
#include "register_consts.h"
#include "SystemClock.h"
#include "XPD.h"
#include "PWM.h"
#include "ultrasonic_sensor.h"
#include "Thread.h"
void ultrasonic_initialization(){
    sys_clock_init(crys_12_288_MHz, _98_304_MHz);
    //set PD4 as input, receive PWM from Echo pin
    //default setting, PD0 low
    gpio_set_config((0x00 << 9), GPIO_J);
    gpio_write((0x00), GPIO_J);

//    thread_setup(button_reading_thread, NULL, 1);
//    thread_run(1);
    gpio_set_config((0x01 << 10), GPIO_J);
    gpio_write((0xff), GPIO_J);
    wait_10us();
    gpio_write((0x00), GPIO_J);
}
int receive_scan_length(){ //use PD7//
    uint16_t t;
    uint16_t length;
    uint16_t port_d_state = gpio_read(GPIO_D);
    bool PD0state = (port_d_state & (1<<7));
    if (!PD0state) { //pin is low//
        length = t;
        return length;
    } else {
        wait_200ms();
        ++t;
    }
}
int receive_scan_width(){ //use PD6//
    uint16_t t;
    uint16_t width;
    uint16_t port_d_state = gpio_read(GPIO_D);
    bool PD0state = (port_d_state & (1<<6));
    if (!PD0state) { //pin is low//
        width = t;
        return width;
    } else {
        wait_200ms();
        ++t;
    }
}

int main() {
//    gpio_write((0x00), GPIO_J);
//        gpio_set_config((0x00 << 11), GPIO_J);
//        while(1) {
//            uint16_t port_c_state = gpio_read(GPIO_J);
//            bool is_button_pressed = (port_c_state & (1 << 3));
//            if (is_button_pressed) {
//                for(int a=0;a<5;++a){
//                    wait_1ms();
//                }
//                break;
//            } else {
//            }
//        }

//    gpio_set_config((0x00 << 15), GPIO_D);
//    gpio_set_config((0x00 << 14), GPIO_D);
//    int length,width;
//    length = receive_scan_length(); //PD7//
//    width = receive_scan_width();   //PD6//
//    xpd_echo_int(length, XPD_Flag_UnsignedDecimal);
//    xpd_echo_int(width, XPD_Flag_UnsignedDecimal);
//    for(int a=0;a<5;++a){
//                    wait_1ms();
//                }
//    left_and_re_adjust();
//    for(int a=0;a<2;++a) {//adjust steering position//
//        forward();
//    }
//    for(int a=0;a<5;++a){
//        wait_1ms();
//    }
    ultrasonic_initialization(); //PJ1 to echo, PJ2 to trig//

    while (1) {
        uint16_t t;
        uint16_t distance;
        uint16_t port_d_state = gpio_read(GPIO_J);
        bool PD0state = (port_d_state & (1 << 1));
        bool timed_out = t > 2353;
        if (!PD0state || timed_out) { //pin is low//
            distance = (t * 17 * 10) / 15;//formula for distance in mm//
            if (timed_out) {
                xpd_puts(" out of range \n ");
            } else {
                xpd_puts("distance: ");
                xpd_echo_int(distance, XPD_Flag_UnsignedDecimal);
                xpd_puts(" mm \n ");
//                if ((distance < 200) && (distance != 0)) {
//                    avoid();
//                    path();
//                    break;
//                } else {
//                    path();
//                    break;
//                }
            }
            t = 0;
            wait_1ms(); //use a delay to wait for some time//

            //re-send signal//
            gpio_set_config((0x01 << 10), GPIO_J);
            gpio_write((0xff), GPIO_J);
            wait_10us();
            gpio_write((0x00), GPIO_J);

            port_d_state = gpio_read(GPIO_J);
            PD0state = (port_d_state & (1 << 1));
            while (!PD0state) {
                port_d_state = gpio_read(GPIO_J);
                PD0state = (port_d_state & (1 << 1));
            }
    } else {
            //pin is high//
            //wait for 10us and increment t//
            wait_10us();
            ++t;  //10us per t//
        }
    }
}



