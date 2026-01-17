#ifndef APP_RFID_H
#define APP_RFID_H


#include "../../Drivers/PN532/pn532_stm32f1.h"

typedef enum {
	LOCKER_INIT,
	LOCKER_CARD_DETECTED,
	LOCKER_LOCKING,
	LOCKER_UNLOCKING,
	LOCKER_IDLE,
	LOCKER_ERROR
}e_LockerState;


typedef struct {

	PN532_Module *rfid;

	//ADD TIMER AND CHANNEL FOR PWM

	uint8_t assigned_uid[10];
	uint8_t assigned_uid_len;

	LockerState locker_state;
}s_Locker;
#endif	/* APP_RFID_H */
