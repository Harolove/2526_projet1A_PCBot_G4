/*
 * comm.h
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour la communication inter-robots et vers la station de base.
 * Utilise UART pour envoyer/recevoir des messages structurés.
 */

#ifndef INC_COMM_H_
#define INC_COMM_H_

#include "stm32g4xx_hal.h"
#include "odometrie.h"  // Pour RobotPose
#include <stdint.h>
#include <stdbool.h>

// Types de messages pour la communication
typedef enum {
    MSG_POSE,        // Envoi de la position du robot
    MSG_OBSTACLE,    // Envoi d'un obstacle détecté
    MSG_ZONE_ASSIGN, // Assignation de zone
    MSG_FUSION_DATA  // Données pour fusion de cartes
} MessageType;

// Structure pour représenter un obstacle dans les communications
typedef struct {
    float x;             // Position X de l'obstacle
    float y;             // Position Y de l'obstacle
    float distance_cm;   // Distance mesurée
} CommObstacle;

// Structure générale d'un message de communication
typedef struct {
    MessageType type;    // Type du message
    uint8_t robot_id;    // ID du robot émetteur
    union {
        RobotPose pose;           // Données de pose
        CommObstacle obstacle;    // Données d'obstacle
        struct {
            float x_min, y_min, x_max, y_max;  // Limites de la zone
        } zone;
        struct {
            uint32_t count;       // Nombre d'obstacles
            CommObstacle obstacles[32];  // Liste d'obstacles pour fusion
        } fusion;
    } payload;  // Contenu variable selon le type
} CommMessage;

// Prototypes des fonctions
void COMM_Init(UART_HandleTypeDef *huart);                    // Initialisation de la communication
bool COMM_SendMessage(const CommMessage *msg);                // Envoi d'un message
bool COMM_ReceiveMessage(CommMessage *msg);                   // Réception d'un message
void COMM_SendToBase(const RobotPose *pose, const CommObstacle *obstacles, uint32_t count);  // Envoi à la base

#endif /* INC_COMM_H_ */