#include "capteur.h"

float CAPTEUR_Read_Distance(CAPTEUR_t *sensor) {
    uint32_t local_time = 0;
    uint32_t timeout = 0;

    if (sensor == NULL) {
        return 0.0f;
    }

    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (__HAL_TIM_GET_COUNTER(sensor->htim) < 10) {
    }

    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);

    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_RESET) {
        timeout++;
        if (timeout > 40000) {
            return 0.0f;
        }
    }

    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_SET) {
        local_time = __HAL_TIM_GET_COUNTER(sensor->htim);
        if (local_time > 38000) {
            break;
        }
    }

    return (float)local_time * 0.034f / 2.0f;
}

