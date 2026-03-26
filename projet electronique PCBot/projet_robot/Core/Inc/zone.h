/*
 * zone.h
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour la gestion des zones de couverture.
 * Assigne des zones spécifiques à chaque robot pour éviter les chevauchements.
 */

#ifndef INC_ZONE_H_
#define INC_ZONE_H_

#include "odometrie.h"

// Structure définissant une zone rectangulaire
typedef struct {
    float x_min, y_min;  // Coin inférieur gauche
    float x_max, y_max;  // Coin supérieur droit
} Zone;

typedef uint8_t RobotID;  // Type pour l'ID du robot

// Prototypes des fonctions
void ZONE_AssignFromID(RobotID id, Zone *zone);                                      // Assignation de zone basée sur ID
bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone);                         // Vérifie si le robot est dans sa zone
void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose);  // Ajuste la vitesse pour rester en zone

#endif /* INC_ZONE_H_ */