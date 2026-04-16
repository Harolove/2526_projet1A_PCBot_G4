#include "moteur.h"

// Initialisation : on démarre les PWM pour les deux canaux et leurs compléments
void MOTEUR_Init(Moteur *m) {
    HAL_TIM_PWM_Start(m->htim, m->channel);      // Démarre CH1 (ou CH2)
    HAL_TIMEx_PWMN_Start(m->htim, m->channel);   // Démarre CH1N (ou CH2N)
}

void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse) {
    if (vitesse > 100) vitesse = 100;
    if (vitesse < -100) vitesse = -100; // On définit une convention la vitesse sera comprise entre -100% (arrière toute) et +100% (avant toute).

    uint32_t period = __HAL_TIM_GET_AUTORELOAD(m->htim); // Cette ligne récupère la valeur maximale que le compteur peut atteindre càd 49
    uint32_t pulse = (abs(vitesse) * period) / 100; // pulse: puissance fournie pour une periode

    if (vitesse >= 0) { // sens vers l'avant
        __HAL_TIM_SET_COMPARE(m->htim, m->channel, pulse); // met le signal PWM sur la pin principale (CH1) et force l'autre pin complémentaire CH1N à 0 (LOW)
        // Elle dit au Timer : "Pendant chaque cycle, reste à l'état HAUT jusqu'à ce que tu atteignes la valeur pulse". Cela génère le signal carré (PWM) sur CH1. Comme CH1N est sa "complémentaire", elle reçoit automatiquement l'inverse
        // Configuration pour que CH1N reste à 0 pendant que CH1 pulse

    } else { // sens vers l'arrière
        __HAL_TIM_SET_COMPARE(m->htim, m->channel, period - pulse); // inverse de ce qu'on a mis pour l'avant
    }
    // Active la sortie principale (ex: CH1 sur PA8)
    m->htim->Instance->CCER |= (1 << (m->channel - 1));
    // Active la sortie complémentaire (ex: CH1N sur PA11)
    m->htim->Instance->CCER |= (1 << (m->channel - 1 + 2));
}
