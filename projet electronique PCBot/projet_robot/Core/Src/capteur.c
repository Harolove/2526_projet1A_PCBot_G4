/*
 * capteur.c
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 *
 * Implémentation des fonctions pour le capteur ultrasonique.
 * Mesure la distance en envoyant une impulsion et en mesurant le temps de retour de l'écho.
 */

#include "capteur.h"

// Fonction pour lire la distance en cm
float CAPTEUR_Read_Distance(CAPTEUR_t *sensor) {
    uint32_t local_time = 0;

    // Envoyer une impulsion de 10µs sur la broche TRIGGER pour déclencher la mesure
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_SET);

    // Réinitialiser le compteur du timer
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    // Attendre 10 microsecondes
    while (__HAL_TIM_GET_COUNTER(sensor->htim) < 10);

    // Remettre TRIGGER à bas
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);

    // Attendre que la broche ECHO passe à l'état haut (début de l'écho)
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_RESET);

    // Mesurer la durée pendant laquelle ECHO reste à haut (temps de vol de l'onde)
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_SET) {
        local_time = __HAL_TIM_GET_COUNTER(sensor->htim);
        if (local_time > 38000) break; // Timeout si hors de portée (environ 6m)
    }

    // Convertir le temps en distance
    return CAPTEUR_ConvertTimeToDistance(local_time);
}

// Fonction de conversion : distance = (temps_us * vitesse_son) / 2
// Vitesse du son ≈ 343 m/s = 0.0343 cm/µs, divisé par 2 pour l'aller-retour
float CAPTEUR_ConvertTimeToDistance(uint32_t local_time_us) {
    return (float)local_time_us * 0.034f / 2.0f;
}

