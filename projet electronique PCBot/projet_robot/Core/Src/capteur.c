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
    /* Reset matériel via XSHUT */
    HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TOF_XSHUT_GPIO_Port, TOF_XSHUT_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    /* Vérification présence sur le bus I2C */
    if (HAL_I2C_IsDeviceReady(hi2c, VL53L0X_ADDRESS, 3, 100) != HAL_OK) {
        return;
    }

    /* Séquence d'init complète VL53L0X */
    writeReg(hi2c, 0x88, 0x00);
    writeReg(hi2c, 0x80, 0x01);
    writeReg(hi2c, 0xFF, 0x01);
    writeReg(hi2c, 0x00, 0x00);
    writeReg(hi2c, 0x91, 0x3C); /* stop_variable — obligatoire */
    writeReg(hi2c, 0x00, 0x01);
    writeReg(hi2c, 0xFF, 0x00);
    writeReg(hi2c, 0x80, 0x00);

    /* Démarrage en mode mesure continue */
    writeReg(hi2c, 0x00, 0x02); /* 0x02 = continu, 0x01 = single-shot */
    HAL_Delay(100);
}

uint16_t CAPTEUR_Read_Distance(I2C_HandleTypeDef *hi2c) {
    /* Attente que la donnée soit prête, avec timeout de 500ms */
    uint32_t t = HAL_GetTick();
    while ((readReg(hi2c, 0x13) & 0x01) == 0) {
        if (HAL_GetTick() - t > 500) {
            return 9999; /* timeout */
        }
    }

    /* Lecture des 2 octets de distance aux registres 0x1E / 0x1F */
    uint8_t reg = 0x1E;
    uint8_t data[2] = {0, 0};

    if (HAL_I2C_Master_Transmit(hi2c, VL53L0X_ADDRESS, &reg, 1, 100) == HAL_OK) {
        if (HAL_I2C_Master_Receive(hi2c, VL53L0X_ADDRESS, data, 2, 100) == HAL_OK) {
            uint16_t distance = ((uint16_t)data[0] << 8) | data[1];

            /* Clear interrupt pour la prochaine mesure */
            writeReg(hi2c, 0x0B, 0x01);

            /* Filtrage hors portée (>2m) */
            if (distance > 2000) {
                return 2001;
            }

            return distance; /* distance en mm */
        }
    }

    return 9999; /* erreur I2C */
}
