/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

// Adjust pin name to your board specification.
// You can use LED1/LED2/LED3/LED4 if any is connected to PWM capable pin,
// or use any PWM capable pin, and see generated signal on logical analyzer.
PwmOut led(PA_15);
InterruptIn button(BUTTON1);
Semaphore sem(0); 
Thread t1;
Thread t2;
Thread t3;
volatile int button_switch = -1;

 
void pwm_thread(void const *name) { 
    while (1) {
        sem.acquire(); 
        if (*((int*)name) == 1) {
            printf("thread 1: -45 degree\n");
            led.write(0.05f);
            while (1) {
                if(button_switch % 3 != 0) 
                    break;
            }
        }
        else if (*((int*)name) == 2) {
            printf("thread 2: 0 degree\n");
            led.write(0.075f);
            while (1) {
                if(button_switch % 3 != 1) 
                    break;
            }
        }
        else { 
            printf("thread 3: 45 degree\n");
            led.write(0.1f);
            while (1) {
                if(button_switch % 3 != 2) 
                    break;
            } 
        }
        sem.release();
    }
}

void button_pressed()
{
    if (button_switch == -1) { 
        sem.release();
        led.period(0.02f);
    }
}

void button_released()
{
    ++button_switch;  
}

int main()
{
    button.fall(&button_pressed);  
    button.rise(&button_released); 
    const int a1 = 1;
    const int a2 = 2;
    const int a3 = 3;
    t1.start(callback(pwm_thread, (void *)&a1)); 
    t2.start(callback(pwm_thread, (void *)&a2)); 
    t3.start(callback(pwm_thread, (void *)&a3)); 
    while (1);
}
