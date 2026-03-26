/*
 * zone.c
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Implémentation des fonctions de gestion de zones.
 * Assigne des quadrants à chaque robot et ajuste le mouvement pour rester en zone.
 */

#include "zone.h"

// Assignation de zone basée sur l'ID du robot (division en 4 quadrants)
void ZONE_AssignFromID(RobotID id, Zone *zone) {
    switch (id % 4) {
        case 0:  // Quadrant supérieur gauche
            zone->x_min = -5.0f; zone->x_max = 0.0f;
            zone->y_min = 0.0f; zone->y_max = 5.0f;
            break;
        case 1:  // Quadrant supérieur droit
            zone->x_min = 0.0f; zone->x_max = 5.0f;
            zone->y_min = 0.0f; zone->y_max = 5.0f;
            break;
        case 2:  // Quadrant inférieur gauche
            zone->x_min = -5.0f; zone->x_max = 0.0f;
            zone->y_min = -5.0f; zone->y_max = 0.0f;
            break;
        case 3:  // Quadrant inférieur droit
            zone->x_min = 0.0f; zone->x_max = 5.0f;
            zone->y_min = -5.0f; zone->y_max = 0.0f;
            break;
    }
}

// Vérifie si la position du robot est dans la zone assignée
bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone) {
    return (pose->x >= zone->x_min && pose->x <= zone->x_max &&
            pose->y >= zone->y_min && pose->y <= zone->y_max);
}

// Ajuste les vitesses des moteurs pour ramener le robot vers sa zone si hors limites
void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose) {
    if (!ZONE_IsInZone(pose, zone)) {
        // Réduire la vitesse et tourner vers le centre de la zone
        *left_speed /= 2;
        *right_speed /= 2;
        // Logique simple : tourner en fonction de la position hors zone
        if (pose->x < zone->x_min) *right_speed += 10;  // Tourner à droite si trop à gauche
        else if (pose->x > zone->x_max) *left_speed += 10;  // Tourner à gauche si trop à droite
        if (pose->y < zone->y_min) *right_speed += 10;  // Tourner à droite si trop bas
        else if (pose->y > zone->y_max) *left_speed += 10;  // Tourner à gauche si trop haut
    }
}