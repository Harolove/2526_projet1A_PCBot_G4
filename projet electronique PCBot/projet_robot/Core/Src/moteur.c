/*
 * moteur.c
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#include "moteur.h"

void MOTEUR_Init(Moteur *m) {
    HAL_TIM_PWM_Start(m->htim, m->channel);
}

void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse) {
    if (vitesse >= 0) {
        HAL_GPIO_WritePin(m->dir_port, m->dir_pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(m->dir_port, m->dir_pin, GPIO_PIN_RESET);
        vitesse = -vitesse; // On repasse en positif pour le PWM
    }

    if (vitesse > 100) vitesse = 100; // limite

    uint32_t period = __HAL_TIM_GET_AUTORELOAD(m->htim); // rapport cyclique
    uint32_t pulse = (vitesse * period) / 100; // registre ARR du timer

    __HAL_TIM_SET_COMPARE(m->htim, m->channel, pulse);
}

