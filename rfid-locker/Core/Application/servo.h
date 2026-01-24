/*
 * servo.h
 *
 *  Created on: Jan 23, 2026
 *      Author: Vivin
 */

#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define SERVO_LOCK_PULSE_US     700 //closing 0 dgeree
#define SERVO_UNLOCK_PULSE_US   2500  // opening 180 degree
#define SERVO_PWM_PERIOD_US		20000
typedef struct {

	TIM_HandleTypeDef *htim;
	uint32_t servo_channel;

	uint32_t max_period;
	uint32_t min_period;

}servo_module;

// Initialize Servo PWM drivers
void servo_Init(servo_module *servo, uint8_t index);

// Lock or Unlock locker by index (0,1,2)
void servo_Lock(servo_module *servo);
void servo_Unlock(servo_module *servo);

#endif /* SERVO_H */
