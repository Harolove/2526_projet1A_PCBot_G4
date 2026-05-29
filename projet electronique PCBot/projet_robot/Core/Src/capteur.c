#include "capteur.h"
#include <stdint.h>

// On définit l'adresse par défaut du VL53L0X (7 bits)
#define VL53L0X_DEFAULT_ADDR 0x29

static HAL_StatusTypeDef writeReg(I2C_HandleTypeDef *hi2c, uint8_t addr8bit, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    return HAL_I2C_Master_Transmit(hi2c, addr8bit, data, 2, 100);
}

static uint8_t readReg(I2C_HandleTypeDef *hi2c, uint8_t addr8bit, uint8_t reg) {
    uint8_t value = 0;
    if (HAL_I2C_Master_Transmit(hi2c, addr8bit, &reg, 1, 100) == HAL_OK) {
        HAL_I2C_Master_Receive(hi2c, addr8bit, &value, 1, 100);
    }
    return value;
}

void Capteur_SetAddress(I2C_HandleTypeDef *hi2c, uint8_t old_addr_7bit, uint8_t new_addr_7bit) {
    uint8_t data = new_addr_7bit & 0x7F;
    // Registre 0x8A pour changer l'adresse I2C
    HAL_I2C_Mem_Write(hi2c, old_addr_7bit << 1, 0x8A, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

void Capteur_Init_Single(I2C_HandleTypeDef *hi2c, uint8_t addr_7bit) {
    uint8_t addr8bit = addr_7bit << 1;

    // Initialisation minimale pour activer le capteur
    // On met le capteur en mode "2V8" si nécessaire (selon ton hardware)
    writeReg(hi2c, addr8bit, 0x89, readReg(hi2c, addr8bit, 0x89) | 0x01);
    writeReg(hi2c, addr8bit, 0x00, 0x01); // Start VL53L0X
}

uint16_t CAPTEUR_Read_Distance(I2C_HandleTypeDef *hi2c, uint8_t devAddr_7bit) {
    uint32_t t = HAL_GetTick();
    uint8_t addr8bit = devAddr_7bit << 1;

    // 1. Attendre que la mesure soit prête
    while ((readReg(hi2c, addr8bit, 0x13) & 0x01) == 0) {
        if (HAL_GetTick() - t > 100) return 9999; // Timeout
    }

    // 2. Lecture du registre de distance (0x1E)
    uint8_t reg = 0x1E;
    uint8_t data[2] = {0, 0};

    if (HAL_I2C_Master_Transmit(hi2c, addr8bit, &reg, 1, 100) == HAL_OK) {
        if (HAL_I2C_Master_Receive(hi2c, addr8bit, data, 2, 100) == HAL_OK) {
            uint16_t distance = ((uint16_t)data[0] << 8) | data[1];

            // 3. Clear l'interruption pour la mesure suivante
            writeReg(hi2c, addr8bit, 0x0B, 0x01);

            if (distance > 2000) return 2001;
            return distance;
        }
    }

    return 9999; // Erreur I2C
}
