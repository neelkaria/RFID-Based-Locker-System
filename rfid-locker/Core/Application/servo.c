/*
 * servo.c
 *
 *  Created on: Jan 23, 2026
 *      Author: Vivin
 */


#include "servo.h"
#include <stdio.h>
#include "../../Drivers/PN532/pn532_stm32f1.h"
// include HAL Libraries ..
extern TIM_HandleTypeDef htim2;

static uint32_t servo_channels [PN532_MAX_INSTANCES] = { \
	TIM_CHANNEL_1,
	TIM_CHANNEL_2,
	TIM_CHANNEL_3,
};



// Initialize PWM for servos
void servo_Init(servo_module *servo, uint8_t index)
{
	servo->htim = &htim2;
	servo->servo_channel = servo_channels[index];

	servo->max_period = (((servo->htim->Init.Period + 1) * SERVO_LOCK_PULSE_US) / SERVO_PWM_PERIOD_US);
	servo->min_period = (((servo->htim->Init.Period + 1) * SERVO_UNLOCK_PULSE_US) / SERVO_PWM_PERIOD_US);

    HAL_TIM_PWM_Start(servo->htim, servo->servo_channel);
	servo_Unlock(servo);

}

// Lock
void servo_Lock(servo_module *servo)
{
	__HAL_TIM_SET_COMPARE(servo->htim, servo->servo_channel, servo->max_period);
}

// Unlock
void servo_Unlock(servo_module *servo)
{
	__HAL_TIM_SET_COMPARE(servo->htim, servo->servo_channel, servo->min_period);
}
