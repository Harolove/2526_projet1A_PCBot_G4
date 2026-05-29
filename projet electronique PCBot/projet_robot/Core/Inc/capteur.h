#ifndef __CAPTEUR_H
#define __CAPTEUR_H

#include "stm32g4xx_hal.h"


typedef struct {
    uint8_t address;
    uint16_t last_distance;
} CAPTEUR_t;

void Capteur_SetAddress(I2C_HandleTypeDef *hi2c, uint8_t old_addr_7bit, uint8_t new_addr_7bit);
void Capteur_Init_Single(I2C_HandleTypeDef *hi2c, uint8_t addr_7bit);
uint16_t CAPTEUR_Read_Distance(I2C_HandleTypeDef *hi2c, uint8_t devAddr_7bit);

#endif
