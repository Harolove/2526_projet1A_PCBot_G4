#include "capteur.h"
#include "main.h"

static HAL_StatusTypeDef writeReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return HAL_I2C_Master_Transmit(hi2c, VL53L0X_ADDRESS, data, 2, 100);
}

static uint8_t readReg(I2C_HandleTypeDef *hi2c, uint8_t reg) {
    uint8_t value = 0;
    HAL_I2C_Master_Transmit(hi2c, VL53L0X_ADDRESS, &reg, 1, 100);
    HAL_I2C_Master_Receive(hi2c, VL53L0X_ADDRESS, &value, 1, 100);
    return value;
}

void Capteur_Configure(I2C_HandleTypeDef *hi2c) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Capteur 1
        // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_X, GPIO_PIN_RESET); // Capteur 2
        // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_Y, GPIO_PIN_RESET); // Capteur 3
        HAL_Delay(100);

        // Permet d'allumé que celui qui est branché
        HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_SET);
        HAL_Delay(100);

        hi2c->Instance->CR1 &= ~(I2C_CR1_PE);
        HAL_Delay(10);
        hi2c->Instance->CR1 |= I2C_CR1_PE;

        HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_SET);
        HAL_Delay(50);

    uint8_t sensor_id = readReg(hi2c, 0xC0);

    // Vérification présence sur le bus I2C
    if (HAL_I2C_IsDeviceReady(hi2c, VL53L0X_ADDRESS, 3, 100) != HAL_OK) {
        return;
    }

    writeReg(hi2c, 0x00, 0x02);
    HAL_Delay(100);
}

uint16_t CAPTEUR_Read_Distance(I2C_HandleTypeDef *hi2c) {
    uint32_t t = HAL_GetTick();
    while ((readReg(hi2c, 0x13) & 0x01) == 0) {
        if (HAL_GetTick() - t > 500) {
            return 9999; /* timeout */
        }
    }

    // Lecture des 2 octets de distance aux registres 0x1E / 0x1F
    uint8_t reg = 0x1E;
    uint8_t data[2] = {0, 0};

    if (HAL_I2C_Master_Transmit(hi2c, VL53L0X_ADDRESS, &reg, 1, 100) == HAL_OK) {
        if (HAL_I2C_Master_Receive(hi2c, VL53L0X_ADDRESS, data, 2, 100) == HAL_OK) {
            uint16_t distance = ((uint16_t)data[0] << 8) | data[1];
            writeReg(hi2c, 0x0B, 0x01);
            if (distance > 2000) {
                return 2001;
            }

            return distance; //distance en mm
        }
    }

    return 9999; //erreur I2C
}
