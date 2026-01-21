#ifndef PN532_TYPES_H
#define PN532_TYPES_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

//#define PN532_USE_I2C
#define PN532_USE_SPI
#define PN532_MAX_INSTANCES		3

typedef struct PN532 PN532;

typedef struct {

#ifdef PN532_USE_I2C

	I2C_HandleTypeDef *hi2c;

	GPIO_TypeDef *RST_Port;
	uint8_t RST_Pin;

	GPIO_TypeDef *IRQ_Port;
	uint8_t IRQ_Pin;

	uint8_t i2c_addr;
#endif

#ifdef PN532_USE_SPI

	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *CS_Port;
	uint16_t CS_Pin;

#endif

	uint8_t buffer[64];

}s_PN532_Module;


struct PN532 {

    /* PN532 Module "HAL" */
    s_PN532_Module module_hal;

	/* PN532 Transport Layer Function Pointers */
    int (*reset)(void);
    int (*read_data)(PN532* dev, uint8_t* data, uint16_t count);
    int (*write_data)(PN532* dev, uint8_t *data, uint16_t count);
    bool (*wait_ready)(PN532* dev, uint32_t timeout);
    int (*wakeup)(PN532* dev);

    void (*log)(const char* log);

    /* Data Buffer */
    uint8_t packetBuffer[64];

    /* TODO: FreeRTOS */
    void (*lock)(void);
    void (*unlock)(void);

};


#endif
