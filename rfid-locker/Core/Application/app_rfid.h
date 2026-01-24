#ifndef APP_RFID_H
#define APP_RFID_H

#include "main.h"
#include "../../Drivers/PN532/pn532_stm32f1.h"
#include "servo.h"
#define LOCKER_OPERATION_TIMEOUT				3000

typedef enum {
	LOCKER_INIT,
	LOCKER_IDLE,
	LOCKER_CARD_DETECTED,
	LOCKER_LOCKING,
	LOCKER_UNLOCKING,
	LOCKER_ACCESS_DENIED,
	LOCKER_TIMEOUT,
	LOCKER_ERROR
}e_LockerState;


typedef struct {

	PN532 rfid;
	servo_module servo;

	/* Book-keeping for each module */
	uint8_t index;

	//ADD TIMER AND CHANNEL FOR PWM

	/* UID and Length of the read card */
	uint8_t uid[6];
	int8_t uid_len;
	uint8_t last_uid[6];
	int8_t last_uid_len;
	bool assigned;

	e_LockerState locker_state;		/* State of the locker */
	uint32_t locker_timeout;		/* Timeout after card read and operation */

	uint16_t error_state;			/* TODO: Define Error Types */

}s_Locker;

/* TODO: Change void to Error Type */
void rfid_FSM (s_Locker *locker);
void rfid_Init(s_Locker *locker);
bool rfid_UID_Exists(s_Locker lockers[], uint8_t uid_len, uint8_t* uid);
#endif	/* APP_RFID_H */
