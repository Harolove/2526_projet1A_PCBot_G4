#ifndef LSM6DSOX_H
#define LSM6DSOX_H
#include "main.h"

#define LSM6D_ADDR      (0x6A << 1)
#define REG_WHO_AM_I    0x0F
#define REG_CTRL1_XL    0x10
#define REG_OUTX_L_A    0x28
#define REG_OUTY_L_A    0x2A

uint8_t LSM6D_Init(I2C_HandleTypeDef *hi2c);
int16_t LSM6D_Read_Accel_X(I2C_HandleTypeDef *hi2c);
int16_t LSM6D_Read_Accel_Y(I2C_HandleTypeDef *hi2c);

#endif
