/*
 * capteur.c
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#include "capteur.h"

float CAPTEUR_Read_Distance(CAPTEUR_t *sensor) {
    uint32_t local_time = 0;

    // impulsion de 10µs sur TRIG
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_SET);

    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (__HAL_TIM_GET_COUNTER(sensor->htim) < 10);

    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);

    // Attendre que ECHO passe à HAUT
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_RESET);

    // Mesurer la durée pendant laquelle ECHO est à HAUT
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_SET) {
        local_time = __HAL_TIM_GET_COUNTER(sensor->htim);
        if (local_time > 38000) break; // Timeout (hors de portée)
    }

    // Conversion en cm
    return (float)local_time * 0.034f / 2.0f;
}

