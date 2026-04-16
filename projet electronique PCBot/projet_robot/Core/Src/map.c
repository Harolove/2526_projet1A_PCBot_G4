/*
 * Ce fichier implémente une carte 2D discrète du robot pour :
 * - initialiser la grille d'occupation,
 * - ajouter des obstacles mesurés dans la grille,
 * - borner la probabilité d'occupation entre 0 et 1.
 */
#include "map.h"   // Inclut les définitions du module de carte (types, constantes, prototypes).
#include <string.h>   // Inclut memset pour initialiser la grille rapidement.

void MAP_Init(MapGrid *grid) {   // Initialise la carte d'occupation.
    memset(grid->grid, 0, sizeof(grid->grid));   // Met toute la grille à 0 (cellules libres/inconnues).
}   // Fin de la fonction d'initialisation de la carte.

void MAP_AddObstacle(MapGrid *grid, float x, float y) {   // Ajoute un obstacle détecté à la position (x, y) en coordonnées réelles.
    int ix = (int)(x / MAP_RESOLUTION + MAP_WIDTH / 2);   // Convertit x (cm ou m selon projet) en indice de colonne de la grille.
    int iy = (int)(y / MAP_RESOLUTION + MAP_HEIGHT / 2);   // Convertit y en indice de ligne de la grille.

    if (ix >= 0 && ix < MAP_WIDTH && iy >= 0 && iy < MAP_HEIGHT) {   // Vérifie que la cellule calculée est bien dans les bornes de la grille.
        grid->grid[iy][ix] += 0.2f;   // Augmente le niveau d'occupation de la cellule visée.

        if (grid->grid[iy][ix] > 1.0f) {   // Vérifie si la valeur dépasse la borne maximale.
            grid->grid[iy][ix] = 1.0f;   // Limite la valeur d'occupation à 1.0.
        }   // Fin du bornage supérieur de la cellule.
    }   // Fin de la vérification des bornes de la grille.
}   // Fin de la fonction d'ajout d'obstacle.
