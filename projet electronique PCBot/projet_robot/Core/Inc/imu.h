#ifndef INC_IMU_H_
#define INC_IMU_H_

#include "stm32g4xx_hal.h" // Ou "main.h"

uint8_t IMU_Check(I2C_HandleTypeDef *hi2c);

#endif /* INC_IMU_H_ */
