/*
 * comm.c
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Implémentation des fonctions de communication.
 * Gère l'envoi/réception de messages via UART pour inter-robots et vers la base.
 */

#include "comm.h"
#include <string.h>
#include <stdio.h>

static UART_HandleTypeDef *comm_huart;  // Handle UART pour la communication
static uint8_t rx_buffer[sizeof(CommMessage)];  // Buffer pour réception

// Initialisation de la communication UART
void COMM_Init(UART_HandleTypeDef *huart) {
    comm_huart = huart;
    // Démarrer la réception en interruption pour messages entrants
    HAL_UART_Receive_IT(comm_huart, rx_buffer, sizeof(CommMessage));
}

// Envoi d'un message structuré via UART
bool COMM_SendMessage(const CommMessage *msg) {
    uint8_t buffer[sizeof(CommMessage)];
    memcpy(buffer, msg, sizeof(CommMessage));  // Copier la structure en buffer
    HAL_StatusTypeDef status = HAL_UART_Transmit(comm_huart, buffer, sizeof(CommMessage), 100);  // Timeout 100ms
    return status == HAL_OK;
}

// Réception d'un message (polling pour simplicité)
bool COMM_ReceiveMessage(CommMessage *msg) {
    uint8_t buffer[sizeof(CommMessage)];
    HAL_StatusTypeDef status = HAL_UART_Receive(comm_huart, buffer, sizeof(CommMessage), 10);  // Timeout 10ms
    if (status == HAL_OK) {
        memcpy(msg, buffer, sizeof(CommMessage));  // Copier dans la structure
        return true;
    }
    return false;
}

// Envoi des données (pose + obstacles) à la station de base en format JSON
void COMM_SendToBase(const RobotPose *pose, const CommObstacle *obstacles, uint32_t count) {
    if (comm_huart == NULL) {
        return;
    }

    char buffer[512];
    // Début du JSON avec ID et pose
    int len = snprintf(buffer, sizeof(buffer), "{\"robot_id\":%d,\"pose\":{\"x\":%.2f,\"y\":%.2f,\"theta\":%.2f},\"obstacles\":[",
                       pose->id, pose->x, pose->y, pose->theta);
    // Ajouter chaque obstacle
    for (uint32_t i = 0; i < count && len < sizeof(buffer) - 50; i++) {
        len += snprintf(buffer + len, sizeof(buffer) - len, "{\"x\":%.2f,\"y\":%.2f,\"dist\":%.1f}%s",
                        obstacles[i].x, obstacles[i].y, obstacles[i].distance_cm, (i < count - 1) ? "," : "");
    }
    // Fermer le JSON
    len += snprintf(buffer + len, sizeof(buffer) - len, "]}\r\n");
    HAL_UART_Transmit(comm_huart, (uint8_t*)buffer, len, 100);
}

// Callback pour la réception UART (à appeler depuis stm32g4xx_it.c)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == comm_huart) {
        // Traiter le message reçu (copier rx_buffer dans une variable globale si nécessaire)
        // Relancer la réception
        HAL_UART_Receive_IT(comm_huart, rx_buffer, sizeof(CommMessage));
    }
}