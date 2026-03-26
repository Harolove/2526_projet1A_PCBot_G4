/*
 * map.c
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Implémentation des fonctions de cartographie.
 * Gère la création et la fusion de cartes probabilistes.
 */

#include "map.h"
#include <string.h>
#include <math.h>

// Initialisation de la grille : tout est libre (probabilité 0)
void MAP_Init(MapGrid *grid) {
    memset(grid->grid, 0, sizeof(grid->grid));
}

// Ajout d'un obstacle à la position (x, y) : conversion en indices de grille et incrémentation de la probabilité
void MAP_AddObstacle(MapGrid *grid, float x, float y) {
    // Conversion des coordonnées réelles en indices de grille
    int ix = (int)(x / MAP_RESOLUTION + MAP_WIDTH / 2);
    int iy = (int)(y / MAP_RESOLUTION + MAP_HEIGHT / 2);
    // Vérifier les limites
    if (ix >= 0 && ix < MAP_WIDTH && iy >= 0 && iy < MAP_HEIGHT) {
        // Incrémenter la probabilité d'occupation (max 1.0)
        grid->grid[iy][ix] = fminf(1.0f, grid->grid[iy][ix] + 0.2f);
    }
}

// Fusion de données : moyenne des probabilités entre carte globale et carte distante
void MAP_FuseData(MapGrid *global, const MapGrid *remote) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            // Moyenne simple des probabilités
            global->grid[y][x] = (global->grid[y][x] + remote->grid[y][x]) / 2.0f;
        }
    }
}

// Sérialisation de la grille pour envoi (copie binaire)
void MAP_SerializeForSend(uint8_t *buffer, size_t *len, const MapGrid *grid) {
    *len = sizeof(MapGrid);
    memcpy(buffer, grid, *len);
}