/*
 * moteur.h
 *
 *  Created on: Mar 19, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour le contrôle des moteurs DC.
 * Utilise PWM pour la vitesse et GPIO pour la direction.
 */

#ifndef INC_MOTEUR_H_
#define INC_MOTEUR_H_

#include "stm32g4xx_hal.h"

// Structure représentant un moteur DC
typedef struct {
    TIM_HandleTypeDef *htim;  // Handle du timer PWM
    uint32_t channel;         // Canal du timer (e.g., TIM_CHANNEL_1)
    GPIO_TypeDef *dir_port;   // Port GPIO pour la broche de direction
    uint16_t dir_pin;         // Numéro de la broche de direction
} Moteur;

// Prototypes des fonctions
void MOTEUR_Init(Moteur *m);                           // Initialisation du moteur
void MOTEUR_SetVitesse(Moteur *m, int32_t vitesse);    // Définition de la vitesse (-100 à 100)

#endif /* INC_MOTEUR_H_ */
