/*
 * odometrie.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#ifndef INC_ODOMETRIE_H_
#define INC_ODOMETRIE_H_

#include "stm32g4xx_hal.h"
#include <math.h>

// --- Constantes du Robot ---
#define PI 3.1415926535f
#define DIAMETRE_ROUE 0.065f  // 65mm
#define CPR 1024.0f           // Coups par tour (selon ton encodeur)
#define ENTRAXE 0.15f         // Distance entre les roues

// Structure pour stocker l'état du robot
typedef struct {
    float x;
    float y;
    float theta;
    int32_t last_pulse_gauche;
    int32_t last_pulse_droit;
} RobotPose;

// Prototypes
void ODOM_Init(RobotPose *pose);
void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD);
void ODOM_UpdateFromCounts(RobotPose *pose, int32_t current_gauche, int32_t current_droit);


#endif /* INC_ODOMETRIE_H_ */
