

#include "moteur.h"
#include <stdlib.h>

void MOTEUR_Init(Moteur *m) {
    __HAL_TIM_SET_COMPARE(m->htim, m->channel, 0);
}


void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse) {
    if (vitesse >  100) vitesse =  100;
    if (vitesse < -100) vitesse = -100;

    uint32_t period = __HAL_TIM_GET_AUTORELOAD(m->htim);

    if (vitesse == 0) {
        __HAL_TIM_SET_COMPARE(m->htim, m->channel, 0);
    } else if (vitesse > 0) {

        uint32_t pulse = ((uint32_t)vitesse * (period + 1)) / 100;
        if (pulse > period) pulse = period;
        __HAL_TIM_SET_COMPARE(m->htim, m->channel, pulse);
    } else {
        uint32_t pulse = ((uint32_t)(-vitesse) * (period + 1)) / 100;
        if (pulse > period) pulse = period;
        __HAL_TIM_SET_COMPARE(m->htim, m->channel, period - pulse);
    }
}
