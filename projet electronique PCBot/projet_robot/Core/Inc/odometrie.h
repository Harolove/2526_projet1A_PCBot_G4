/*
 * odometrie.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour l'odométrie du robot.
 * Calcule la position (x, y, theta) à partir des encodeurs des roues.
 */

#ifndef INC_ODOMETRIE_H_
#define INC_ODOMETRIE_H_

#include "stm32g4xx_hal.h"
#include <math.h>

// --- Constantes du Robot ---
#define PI 3.1415926535f      // Valeur de pi
#define DIAMETRE_ROUE 0.065f  // Diamètre des roues en mètres (65mm)
#define CPR 1024.0f           // Coups par tour des encodeurs
#define ENTRAXE 0.15f         // Distance entre les roues en mètres

// Structure pour stocker la pose (position et orientation) du robot
typedef struct {
    float x;                   // Position X en mètres
    float y;                   // Position Y en mètres
    float theta;               // Orientation en radians
    int32_t last_pulse_gauche; // Dernier comptage encodeur gauche
    int32_t last_pulse_droit;  // Dernier comptage encodeur droit
    uint8_t id;                // ID unique du robot pour multi-robots
} RobotPose;

// Prototypes des fonctions
void ODOM_Init(RobotPose *pose);                                           // Initialisation de la pose
void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD);  // Mise à jour depuis timers
void ODOM_UpdateFromCounts(RobotPose *pose, int32_t current_gauche, int32_t current_droit);  // Mise à jour depuis comptages

#endif /* INC_ODOMETRIE_H_ */
