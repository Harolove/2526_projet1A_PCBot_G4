/*
 * odometrie.c
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#include "odometrie.h"

void ODOM_Init(RobotPose *pose) {
    pose->x = 0.0f;
    pose->y = 0.0f;
    pose->theta = 0.0f;
    pose->last_pulse_gauche = 0;
    pose->last_pulse_droit = 0;
}

void ODOM_Update(RobotPose *pose, TIM_HandleTypeDef *htimG, TIM_HandleTypeDef *htimD) {

    int32_t current_gauche = (int32_t)__HAL_TIM_GET_COUNTER(htimG);
    int32_t current_droit = (int32_t)__HAL_TIM_GET_COUNTER(htimD);

    // Déplacement relatif
    int32_t diff_g = current_gauche - pose->last_pulse_gauche;
    int32_t diff_d = current_droit - pose->last_pulse_droit;

    // Distance en m
    float dist_g = (float)diff_g * (PI * DIAMETRE_ROUE) / CPR;
    float dist_d = (float)diff_d * (PI * DIAMETRE_ROUE) / CPR;

    // Calcul cinématique
    float d_distance = (dist_d + dist_g) / 2.0f;
    float d_theta = (dist_d - dist_g) / ENTRAXE;

    // Mise à jour de la position globale
    pose->x += d_distance * cosf(pose->theta);
    pose->y += d_distance * sinf(pose->theta);
    pose->theta += d_theta;

    // Sauvegarde pour le prochain cycle
    pose->last_pulse_gauche = current_gauche;
    pose->last_pulse_droit = current_droit;
}

