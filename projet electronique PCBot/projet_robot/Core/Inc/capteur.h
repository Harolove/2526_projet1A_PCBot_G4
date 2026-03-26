/*
 * capteur.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour la gestion du capteur ultrasonique.
 * Le capteur mesure la distance en utilisant un signal TRIGGER et ECHO.
 */

#ifndef INC_CAPTEUR_H_
#define INC_CAPTEUR_H_

#include "stm32g4xx_hal.h"

// Structure représentant un capteur ultrasonique HC-SR04
typedef struct {
    GPIO_TypeDef *trig_port;     // Port GPIO pour la broche TRIGGER
    uint16_t trig_pin;           // Numéro de la broche TRIGGER
    GPIO_TypeDef *echo_port;     // Port GPIO pour la broche ECHO
    uint16_t echo_pin;           // Numéro de la broche ECHO
    TIM_HandleTypeDef *htim;     // Timer pour mesurer le temps en microsecondes
} CAPTEUR_t;

// Prototypes des fonctions
// Lit la distance mesurée par le capteur en cm
float CAPTEUR_Read_Distance(CAPTEUR_t *sensor);
// Convertit le temps de vol en distance (formule : distance = temps_us * vitesse_son / 2)
float CAPTEUR_ConvertTimeToDistance(uint32_t local_time_us);

#endif /* INC_CAPTEUR_H_ */
