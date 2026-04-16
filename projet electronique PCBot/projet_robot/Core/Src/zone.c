/*
 * Ce fichier implémente la gestion des zones de déplacement du robot :
 * - attribution d'une zone selon l'identifiant du robot,
 * - test d'appartenance d'une pose à une zone,
 * - ajustement des vitesses quand le robot sort de sa zone.
 */
#include "zone.h"   // Inclut les types et prototypes du module de gestion des zones.

void ZONE_AssignFromID(RobotID id, Zone *zone) {
    // Le robot va partout dans le carré de 16m²
    zone->x_min = -2.0f;
    zone->x_max =  2.0f;
    zone->y_min = -2.0f;
    zone->y_max =  2.0f;
}

bool ZONE_IsInZone(const RobotPose *pose, const Zone *zone) {   // Vérifie si la pose du robot est à l'intérieur de la zone.
    if (pose->x < zone->x_min) {   // Teste si X est à gauche de la borne minimale.
        return false;   // Retourne faux: la pose est hors zone.
    }   // Fin du test de borne minimale en X.
    if (pose->x > zone->x_max) {   // Teste si X dépasse la borne maximale.
        return false;   // Retourne faux: la pose est hors zone.
    }   // Fin du test de borne maximale en X.
    if (pose->y < zone->y_min) {   // Teste si Y est en dessous de la borne minimale.
        return false;   // Retourne faux: la pose est hors zone.
    }   // Fin du test de borne minimale en Y.
    if (pose->y > zone->y_max) {   // Teste si Y dépasse la borne maximale.
        return false;   // Retourne faux: la pose est hors zone.
    }   // Fin du test de borne maximale en Y.

    return true;   // Retourne vrai: la pose est dans toutes les bornes de la zone.
}   // Fin de la fonction de test d'appartenance à la zone.

void ZONE_AdjustMovement(const Zone *zone, int32_t *left_speed, int32_t *right_speed, const RobotPose *pose) {   // Corrige la commande moteur si le robot est hors zone.
    if (!ZONE_IsInZone(pose, zone)) {   // N'agit que si la pose courante est hors de la zone autorisée.
        *left_speed /= 2;   // Réduit la vitesse de la roue gauche pour ralentir le robot.
        *right_speed /= 2;   // Réduit la vitesse de la roue droite pour ralentir le robot.

        if (pose->x < zone->x_min) {   // Si le robot est trop à gauche de la zone.
            *right_speed += 20;   // Augmente la roue droite pour favoriser une correction vers la droite.
        } else if (pose->x > zone->x_max) {   // Sinon, si le robot est trop à droite de la zone.
            *left_speed += 20;   // Augmente la roue gauche pour favoriser une correction vers la gauche.
        }   // Fin de la correction selon la position en X.

        if (pose->y < zone->y_min) {   // Si le robot est trop bas par rapport à la zone.
            *right_speed += 20;   // Applique une correction supplémentaire via la roue droite.
        } else if (pose->y > zone->y_max) {   // Sinon, si le robot est trop haut par rapport à la zone.
            *left_speed += 20;   // Applique une correction supplémentaire via la roue gauche.
        }   // Fin de la correction selon la position en Y.
    }   // Fin du traitement hors zone.
}   // Fin de la fonction d'ajustement de mouvement.
