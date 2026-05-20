#ifndef INC_CAPTEUR_H_
#define INC_CAPTEUR_H_

#include "stm32g4xx_hal.h"

#define VL53L0X_ADDRESS 0x52

typedef struct {
    TIM_HandleTypeDef *htim;
} CAPTEUR_t;

void Capteur_Configure(I2C_HandleTypeDef *hi2c);
uint16_t CAPTEUR_Read_Distance(I2C_HandleTypeDef *hi2c);

#endif /* INC_CAPTEUR_H_ */
