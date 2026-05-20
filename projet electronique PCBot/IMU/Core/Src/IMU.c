#include "IMU.h"

uint8_t LSM6D_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t id = 0;
    HAL_I2C_Mem_Read(hi2c, LSM6D_ADDR, REG_WHO_AM_I, 1, &id, 1, 100);
    if (id == 0x6C) {
        uint8_t config = 0x40;
        HAL_I2C_Mem_Write(hi2c, LSM6D_ADDR, REG_CTRL1_XL, 1, &config, 1, 100);//On écrit la configuration choisi (config) dans le registre de commande 0x10 pour que l'accelerometre s'allume
        return 1;
    }
    return 0;
}

int16_t LSM6D_Read_Accel_X(I2C_HandleTypeDef *hi2c) {// int16_t: renvoie un entier signé sur 16 bits, car l'accélération peut être positive ou négative ( gauche-droite
//*hi2c : pointeur vers la configuration I2C la STM32. Pour savoir quel "canal" utiliser pour parler au capteur.
    uint8_t data[2];
    if (HAL_I2C_Mem_Read(hi2c, LSM6D_ADDR, REG_OUTX_L_A, 1, data, 2, 100) == HAL_OK) { // On va à l'adresse 0x28 (REG_OUTX_L_A).
        return (int16_t)((data[1] << 8) | data[0]);
    }
    return 0;
}

int16_t LSM6D_Read_Accel_Y(I2C_HandleTypeDef *hi2c) {
    uint8_t data[2];
    if (HAL_I2C_Mem_Read(hi2c, LSM6D_ADDR, REG_OUTY_L_A, 1, data, 2, 100) == HAL_OK) {
        return (int16_t)((data[1] << 8) | data[0]);
    }
    return 0;
}
