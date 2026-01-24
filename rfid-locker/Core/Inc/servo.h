/*
 * servo.h
 *
 *  Created on: Jan 23, 2026
 *      Author: Vivin
 */

#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

// Initialize Servo PWM drivers
void Servo_Init(void);

// Lock or Unlock locker by index (0,1,2)
void Servo_Lock(uint8_t locker_index);
void Servo_Unlock(uint8_t locker_index);

#endif /* SERVO_H */
