/* Le principe repose sur le temps de trajet d'une onde sonore :

 - la stm32 envoie une impulsion courte (trigger) au capteur (pour lui dire d'émettre un son)
 - le capteur émet un son et attend l'écho
 - on mesure la durée pendant laquelle le signal "echo" reste à l'état haut
 - on convertit ce temps en distance

Au repos, le pin Echo est à 0V (état BAS).
Dès que le capteur a fini d'envoyer ses ultrasons, il met la pin Echo à 3.3V (état HAUT).
Dès que le capteur "entend" l'écho revenir, il remet la pin Echo à 0V.

*/

#include "capteur.h"

float CAPTEUR_Read_Distance(CAPTEUR_t *sensor) {
    uint32_t local_time = 0; //variable stockant la durée de l'écho
    uint32_t timeout = 0; //compteur pour éviter un blocage infini (si capteur débranché ou fil cassé par exemple)

    if (sensor == NULL) { //sécurité si le pointeur est vide
        return 0.0f;
    }

    //déclenchement (trigger)
    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (__HAL_TIM_GET_COUNTER(sensor->htim) < 10) {
    }

    HAL_GPIO_WritePin(sensor->trig_port, sensor->trig_pin, GPIO_PIN_RESET);

    //attente du retour
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_RESET) {
        timeout++;
        if (timeout > 40000) {
            return 0.0f;
        }
    }

    //mesure de la durée
    __HAL_TIM_SET_COUNTER(sensor->htim, 0);
    while (HAL_GPIO_ReadPin(sensor->echo_port, sensor->echo_pin) == GPIO_PIN_SET) {
        local_time = __HAL_TIM_GET_COUNTER(sensor->htim);
        if (local_time > 38000) {
            break;
        }
    }

    //conversion en cm
    //distance = (temps en µs * vitesse du son 0.034 cm/µs)/2 (aller-retour)
    return (float)local_time * 0.034f / 2.0f;
}

