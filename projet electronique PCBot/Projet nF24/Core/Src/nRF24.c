#include "nRF24.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2; //structure utilisée par la bibliothèque HAL pour piloter le périphérique SPI (vitesse, mode...)

void NRF24_CSN_RESET() {
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET);
}//On change l'état de la broche PC0 à 0 (relié à CSN: le signal pour le bus SPI. On le met à 0 (Reset) pour dire au module avec lequel qu'on communique qu'on va communiquer avec lui sur ce bus, et dès qu'on a finis on le remet à 1

void NRF24_CSN_SET() {
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
}//remet la pin à 3.3V, c'est le signal de fin. Pour que le module ignore le bus SPI

void NRF24_CE_Enable() {
    HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_SET);
}//la broche CE gère le lien avec l'antenne radio, on met la pin PB12 à 3.3V pour activer l'antenne afin d'être en mode émission ou réception

void NRF24_CE_Disable() {
    HAL_GPIO_WritePin(NRF_CE_GPIO_Port, NRF_CE_Pin, GPIO_PIN_RESET);
}//On remet la pin PB12 à 0V. Permet de pour changer de mode et aussi pour économiser de l'énergie.

void NRF24_WriteReg(uint8_t reg, uint8_t data) {//reg: adresse du registre dans lequel on écrit, data c'est la valeur (l'octet) qu'on met dans le registre
    uint8_t buf[2];//tableau de 2 octets qui contient le registre dans lequel on veut écrire et la donnée.
    buf[0] = reg | W_REGISTER; //W_REGISTER est une commande (0x20) pour écrire
    buf[1] = data;
    NRF24_CSN_RESET();//pin CSN à 0V pour transmettre données
    HAL_SPI_Transmit(&hspi2, buf, 2, 100);//On utilise le port SPI numéro 2, n envoie le contenu de notre tableau (instruction + donnée), envoie 2 octets.
    NRF24_CSN_SET(); //CSN à 3.3V pour fin de transmission de données pour qu'il la traite
}
uint8_t NRF24_ReadReg(uint8_t reg) {// lit ce qu'il a dans le registre reg. Prend en entrée l'adresse du registre reg et renvoie un octet (uint8_t) qui est la valeur lue.
    uint8_t command = reg & 0x1F;
    uint8_t status;
    uint8_t value;

    NRF24_CSN_RESET();
    HAL_SPI_TransmitReceive(&hspi2, &command, &status, 1, 100);
    HAL_SPI_Receive(&hspi2, &value, 1, 100);
    NRF24_CSN_SET();

    return value;
}

void NRF24_Init(SPI_HandleTypeDef *hspi) {
    NRF24_CE_Disable();

    // On s'assure que le module est PWR_UP (0x02) et en mode RX (0x01) -> 0x03
    // Ton 0x0C d'avant désactivait les interruptions, 0x0B ou 0x0F est souvent plus sûr
    NRF24_WriteReg(CONFIG, 0x0B);
    NRF24_WriteReg(EN_AA, 0x01);    // Auto Ack activé
    NRF24_WriteReg(RF_CH, 76);      // Canal 76
    NRF24_WriteReg(RF_SETUP, 0x06); // 1Mbps, 0dBm

    uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

    // Config TX Address
    NRF24_CSN_RESET();
    uint8_t reg_tx = TX_ADDR | W_REGISTER;
    HAL_SPI_Transmit(&hspi2, &reg_tx, 1, 100);
    HAL_SPI_Transmit(&hspi2, addr, 5, 100);
    NRF24_CSN_SET();

    // Config RX Address Pipe 0
    NRF24_CSN_RESET();
    uint8_t reg_rx = RX_ADDR_P0 | W_REGISTER;
    HAL_SPI_Transmit(&hspi2, &reg_rx, 1, 100);
    HAL_SPI_Transmit(&hspi2, addr, 5, 100);
    NRF24_CSN_SET();

    // Taille du payload pour le pipe 0
    NRF24_WriteReg(0x11, sizeof(RobotPos_t));

    NRF24_StartListening();
}

void NRF24_Send(RobotPos_t *data) {
    NRF24_CE_Disable();

    uint8_t config = NRF24_ReadReg(CONFIG);
    NRF24_WriteReg(CONFIG, config & 0xFE); // PRIM_RX = 0 (Mode TX)

    uint8_t flush_cmd = FLUSH_TX;
    NRF24_CSN_RESET();
    HAL_SPI_Transmit(&hspi2, &flush_cmd, 1, 100);
    NRF24_CSN_SET();

    uint8_t cmd = W_TX_PAYLOAD;
    NRF24_CSN_RESET();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);
    HAL_SPI_Transmit(&hspi2, (uint8_t*)data, sizeof(RobotPos_t), 100);
    NRF24_CSN_SET();

    NRF24_CE_Enable();
    HAL_Delay(1);
    NRF24_CE_Disable();
    NRF24_WriteReg(STATUS, (1 << 5) | (1 << 4));

    NRF24_StartListening();
}

uint8_t NRF24_DataReady(void) {
    uint8_t status = NRF24_ReadReg(STATUS);
    if (status & (1 << 6)) return 1;
    return 0;
}

void NRF24_Receive(RobotPos_t *data) {
    uint8_t cmd = R_RX_PAYLOAD;
    NRF24_CSN_RESET();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);
    HAL_SPI_Receive(&hspi2, (uint8_t*)data, sizeof(RobotPos_t), 100);
    NRF24_CSN_SET();
    NRF24_WriteReg(STATUS, (1 << 6));
}

void NRF24_StartListening(void) {
    uint8_t config = NRF24_ReadReg(CONFIG);
    NRF24_WriteReg(CONFIG, config | 0x01);

    uint8_t cmd = FLUSH_RX;
    NRF24_CSN_RESET();
    HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);
    NRF24_CSN_SET();

    NRF24_CE_Enable();
}
