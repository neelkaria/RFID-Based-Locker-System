/*
 * servo.c
 *
 *  Created on: Jan 23, 2026
 *      Author: Vivin
 */


#include "servo.h"
#include <stdio.h>

// include HAL Libraries ..


#define SERVO_LOCK_PULSE     1000  //closing 0 dgeree
#define SERVO_UNLOCK_PULSE   2000  // opening 180 degree
#define NUM_LOCKERS 3

// PWM pulse set for servo
static void set_servo_pulse(uint8_t locker_index, uint16_t pulse)
{
    switch(locker_index)
    {
        case 0: __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse); break;
        case 1: __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, pulse); break;
        case 2: __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse); break;
        default:
            printf("Invalid locker index: %d\n", locker_index);
            break;
    }
}

// Initialize PWM for servos
void Servo_Init(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

    // default all lockers locked
    for(uint8_t i = 0; i < NUM_LOCKERS; i++)
        Servo_Lock(i);

}

// Lock
void Servo_Lock(uint8_t locker_index)
{
    set_servo_pulse(locker_index, SERVO_LOCK_PULSE);
    printf("Locker %d locked.\n", locker_index);
}

// Unlock
void Servo_Unlock(uint8_t locker_index)
{
    set_servo_pulse(locker_index, SERVO_UNLOCK_PULSE);
    printf("Locker %d unlocked.\n", locker_index);
}
