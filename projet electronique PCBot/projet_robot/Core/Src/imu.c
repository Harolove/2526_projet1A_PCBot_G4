#include "main.h"
#include <stdint.h>

#define LSM6DSOX_ADDR (0x6B << 1)
#define WHO_AM_I_REG  0x0F

uint8_t IMU_Check(I2C_HandleTypeDef *hi2c) {
    uint8_t id = 0;
    // lecture registre who_am_i
    if (HAL_I2C_Mem_Read(hi2c, LSM6DSOX_ADDR, WHO_AM_I_REG, I2C_MEMADD_SIZE_8BIT, &id, 1, 100) == HAL_OK) {
        return id;
    }
    return 0x00;
}
