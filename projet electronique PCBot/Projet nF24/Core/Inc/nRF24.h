#ifndef NRF24_H_
#define NRF24_H_

#include "stm32l4xx_hal.h"

// Commandes du nRF24
#define R_REGISTER         0x00
#define W_REGISTER         0x20
#define R_RX_PAYLOAD       0x61
#define W_TX_PAYLOAD       0xA0
#define FLUSH_TX           0xE1
#define FLUSH_RX           0xE2

// Registres importants
#define CONFIG      0x00
#define EN_AA       0x01
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define RX_ADDR_P0  0x0A
#define TX_ADDR     0x10

//Structure de données pour la cartographie
typedef struct {
    float x;
    float y;
    int robot_id;
} RobotPos_t;

// Prototypes
void NRF24_Init(SPI_HandleTypeDef *hspi);
void NRF24_Send(RobotPos_t *data);
uint8_t NRF24_ReadReg(uint8_t reg);

uint8_t NRF24_DataReady(void);
void NRF24_Receive(RobotPos_t *data);
void NRF24_StartListening(void);
#endif
