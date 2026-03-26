/*
 * map.h
 *
 *  Created on: Mar 26, 2026
 *      Author: dig94
 *
 * Ce fichier définit les structures et prototypes pour la cartographie.
 * Utilise une grille probabiliste pour représenter l'environnement et fusionner les données de plusieurs robots.
 */

#ifndef INC_MAP_H_
#define INC_MAP_H_

#include <stdint.h>
#include <stdbool.h>

// Dimensions de la grille de cartographie
#define MAP_WIDTH 20       // Nombre de cellules en largeur
#define MAP_HEIGHT 20      // Nombre de cellules en hauteur
#define MAP_RESOLUTION 0.1f  // Résolution : 10cm par cellule

// Structure pour la grille de cartographie (probabilités d'occupation)
typedef struct {
    float grid[MAP_HEIGHT][MAP_WIDTH];  // Valeur entre 0 (libre) et 1 (occupé)
} MapGrid;

// Structure pour un obstacle après fusion
typedef struct {
    float x, y;         // Position de l'obstacle
    float confidence;   // Confiance (0-1)
} FusedObstacle;

// Prototypes des fonctions
void MAP_Init(MapGrid *grid);                                    // Initialisation de la grille
void MAP_AddObstacle(MapGrid *grid, float x, float y);           // Ajout d'un obstacle à la grille
void MAP_FuseData(MapGrid *global, const MapGrid *remote);       // Fusion de données de cartes
void MAP_SerializeForSend(uint8_t *buffer, size_t *len, const MapGrid *grid);  // Sérialisation pour envoi

#endif /* INC_MAP_H_ */