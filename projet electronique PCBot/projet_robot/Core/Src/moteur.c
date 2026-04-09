#include "moteur.h"

void MOTEUR_Init(Moteur *m) {
    HAL_TIM_PWM_Start(m->htim, m->channel);
}

void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse) {
    uint32_t period;
    uint32_t pulse;

    if (vitesse >= 0) {
        HAL_GPIO_WritePin(m->dir_port, m->dir_pin, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(m->dir_port, m->dir_pin, GPIO_PIN_RESET);
        vitesse = -vitesse;
    }

    if (vitesse > 100) {
        vitesse = 100;
    }

    period = __HAL_TIM_GET_AUTORELOAD(m->htim);
    pulse = (vitesse * period) / 100;

    __HAL_TIM_SET_COMPARE(m->htim, m->channel, pulse);
}

