/*
 * moteur.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#ifndef INC_MOTEUR_H_
#define INC_MOTEUR_H_

#include "stm32g4xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim; // Timer du PWM
    uint32_t channel;        // Canal du Timer (ex: TIM_CHANNEL_1)
    GPIO_TypeDef *dir_port;  // Port de la broche direction
    uint16_t dir_pin;        // Numéro de la broche direction
} Moteur;

void MOTEUR_Init(Moteur *m);
void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse); // Vitesse de -100 à 100



#endif /* INC_MOTEUR_H_ */
