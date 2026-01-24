/* Application code for RFID Locker System */

#include "app_rfid.h"
#include <string.h>
#include "../../Drivers/PN532/pn532.h"

extern SPI_HandleTypeDef hspi1;
extern s_Locker lockers[PN532_MAX_INSTANCES];

/* Port Config Arrays for NFC Modules */

static GPIO_TypeDef *cs_port [PN532_MAX_INSTANCES] = { \
	CS0_GPIO_Port,
	CS1_GPIO_Port,
	CS2_GPIO_Port,
};

static uint16_t cs_pin [PN532_MAX_INSTANCES] = { \
	CS0_Pin,
	CS1_Pin,
	CS2_Pin,
};

static GPIO_TypeDef *testLED_Port [PN532_MAX_INSTANCES] = { \
	LD1_GPIO_Port,
	LD2_GPIO_Port,
	LD3_GPIO_Port,
};

static uint16_t testLED_Pin [PN532_MAX_INSTANCES] = { \
	LD1_Pin,
	LD2_Pin,
	LD3_Pin,
};


void rfid_Init(s_Locker *_locker)
{
	_locker->rfid.module_hal.hspi	= &hspi1;
	_locker->rfid.module_hal.CS_Port = cs_port[_locker->index];
	_locker->rfid.module_hal.CS_Pin 	= cs_pin[_locker->index];
}


bool rfid_UID_Exists(s_Locker lockers[], uint8_t len, uint8_t* uid)
{
	for( uint8_t i = 0; i < PN532_MAX_INSTANCES; i++)
	{
		// skip empty lockers
		if(!lockers[i].assigned)
			continue;

		// compare UIDs
		if(memcmp(lockers[i].uid, uid, len) == 0)
			return true;
	}
	return false;
}

void rfid_FSM(s_Locker *_locker)
{
	uint8_t buff[3] = {0};

	switch (_locker->locker_state)
	{
		case LOCKER_INIT:
			/* Initialize Locker modules and PN532 modules */
			rfid_Init(_locker);
			servo_Init(&_locker->servo, _locker->index);
			HAL_Delay(1000);
			PN532_SPI_Init(&_locker->rfid);

			if (PN532_GetFirmwareVersion(&_locker->rfid, buff) == PN532_STATUS_OK) {

				printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);
				printf("Waiting for RFID/NFC card...\r\n");

				HAL_GPIO_WritePin(testLED_Port[_locker->index], testLED_Pin[_locker->index], GPIO_PIN_RESET);
				PN532_SamConfiguration(&_locker->rfid);

				_locker->locker_state = LOCKER_IDLE;
			} else {

				printf("PN532 Not Found \r\n");

				_locker->locker_state = LOCKER_ERROR;
			}
			break;

		case LOCKER_IDLE:
		{
			/* Wait to Observe/Read Cards */

			uint8_t uid[6];
			int8_t uid_len = 0;

			uid_len = PN532_ReadPassiveTarget(&_locker->rfid, uid, PN532_MIFARE_ISO14443A, 100);

			if (uid_len > 0)
			{
				/* UID Detected */

				_locker->last_uid_len = uid_len;
				memcpy(_locker->last_uid, uid, uid_len);

				_locker->locker_state = LOCKER_CARD_DETECTED;
			}

			break;
		}
		case LOCKER_CARD_DETECTED:

			/* Assign/Unassign Cards, change state accordingly */

			/* Locker not assigned to a card */
			if(!_locker->assigned)
			{

				/* Card is not assigned to any Locker, assign to current one */
				if(!rfid_UID_Exists(lockers, _locker->last_uid_len, _locker->last_uid))
				{
					memcpy(_locker->uid, _locker->last_uid, _locker->last_uid_len);

					_locker->uid_len = _locker->last_uid_len;

					_locker->locker_state = LOCKER_LOCKING;
				}

				/* Card being assigned to a Locker is already assigned to another Locker */
				else
				{
					_locker->locker_state = LOCKER_ACCESS_DENIED;
				}
			}

			/* If locker has already been assigned a card */
			else
			{
				/* Check if card matches assigned Locker */
				if( (memcmp(_locker->last_uid, _locker->uid, _locker->last_uid_len) == 0) && \
					(_locker->uid_len == _locker->last_uid_len))
				{

					_locker->locker_state = LOCKER_UNLOCKING;
				}

				else
				{
					_locker->locker_state = LOCKER_ACCESS_DENIED;
				}
			}

			break;

		case LOCKER_LOCKING:

			/* Lock the Locker */
			HAL_GPIO_WritePin(testLED_Port[_locker->index], testLED_Pin[_locker->index], GPIO_PIN_SET);
			servo_Lock(&_locker->servo);

			_locker->assigned = true;

			/* Cool-down period before next read */
			_locker->locker_timeout = HAL_GetTick();
			_locker->locker_state = LOCKER_TIMEOUT;
			break;

		case LOCKER_UNLOCKING:

			/* Unlock the Locker, reset variables */
			HAL_GPIO_WritePin(testLED_Port[_locker->index], testLED_Pin[_locker->index], GPIO_PIN_RESET);
			servo_Unlock(&_locker->servo);

			_locker->uid_len = 0;
		    _locker->last_uid_len = 0;

			memset(_locker->uid, 0, sizeof(_locker->uid));
			memset(_locker->last_uid, 0, sizeof(_locker->last_uid));

			_locker->assigned = false;

			/* Cool-down period before next read */
			_locker->locker_timeout = HAL_GetTick();
			_locker->locker_state = LOCKER_TIMEOUT;
			break;

		case LOCKER_ACCESS_DENIED:
			/* Do something for Access Denied TODO: LED Patterns */

			_locker->locker_state = LOCKER_IDLE;
			break;

		case LOCKER_TIMEOUT:

			if( HAL_GetTick() - _locker->locker_timeout >= LOCKER_OPERATION_TIMEOUT)
			{
				_locker->locker_state = LOCKER_IDLE;
			}

			break;

		case LOCKER_ERROR:

			break;

		default:
			break;
	}
}

