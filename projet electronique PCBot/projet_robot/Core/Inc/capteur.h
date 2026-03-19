/*
 * capteur.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 */

#ifndef INC_CAPTEUR_H_
#define INC_CAPTEUR_H_

#include "stm32g4xx_hal.h"

typedef struct {
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    TIM_HandleTypeDef *htim; // Un timer pour mesurer le temps (microsecondes)
} HCSR04_t;

// Prototype
float HCSR04_Read_Distance(CAPTEUR_t *sensor);


#endif /* INC_CAPTEUR_H_ */
