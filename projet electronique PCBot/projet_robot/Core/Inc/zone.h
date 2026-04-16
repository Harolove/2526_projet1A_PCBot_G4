/*
 * Ce fichier d'en-tête déclare l'interface du module de gestion de zone :
 * - type Zone (bornes min/max en X et Y),
 * - type RobotID,
 * - fonctions d'attribution, de test et d'ajustement du mouvement.
 */
#ifndef INC_ZONE_H_   // Vérifie si la garde d'inclusion n'est pas encore définie.
#define INC_ZONE_H_   // Définit la garde d'inclusion pour éviter les inclusions multiples.

#include <stdint.h>   // Inclut les types entiers de taille fixe (ex: uint8_t, int32_t).
#include <stdbool.h>   // Inclut le type bool et les valeurs true/false.
#include "odometrie.h"   // Inclut le type RobotPose utilisé dans les prototypes.

typedef struct {   // Déclare la structure représentant les limites d'une zone.
    float x_min, y_min;   // Stocke les bornes minimales de la zone sur les axes X et Y.
    float x_max, y_max;   // Stocke les bornes maximales de la zone sur les axes X et Y.
} Zone;   // Nomme ce type de structure "Zone".

typedef uint8_t RobotID;   // Définit le type d'identifiant robot sur 8 bits non signés.

void ZONE_AssignFromID(RobotID id, Zone *zone);   // Déclare la fonction qui assigne une zone selon l'identifiant robot.
bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone);   // Déclare la fonction qui teste si une pose appartient à une zone.
void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose);   // Déclare la fonction qui corrige les vitesses si le robot sort de la zone.

#endif /* INC_ZONE_H_ */   // Termine la garde d'inclusion du fichier d'en-tête.