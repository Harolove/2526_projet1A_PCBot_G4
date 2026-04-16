/*
 * Ce fichier d'en-tête déclare l'interface du module de cartographie :
 * - dimensions de la grille de carte,
 * - type de données de la carte,
 * - fonctions d'initialisation et d'ajout d'obstacles.
 */
#ifndef INC_MAP_H_   // Vérifie si la garde d'inclusion n'a pas encore été définie.
#define INC_MAP_H_   // Définit la garde d'inclusion pour éviter les inclusions multiples.

/* Carte très simple: une grille de cellules occupées ou libres. */   // Résume le principe de la carte.
#define MAP_WIDTH 20   // Définit le nombre de colonnes de la grille de carte.
#define MAP_HEIGHT 20   // Définit le nombre de lignes de la grille de carte.
#define MAP_RESOLUTION 0.1f   // Définit la taille d'une cellule en unités de distance du projet.

typedef struct {   // Déclare la structure de données représentant une carte.
    float grid[MAP_HEIGHT][MAP_WIDTH];   // Stocke la valeur d'occupation de chaque cellule de la grille.
} MapGrid;   // Nomme le type de structure de carte "MapGrid".

void MAP_Init(MapGrid *grid);   // Déclare la fonction qui remet toute la carte à zéro.
void MAP_AddObstacle(MapGrid *grid, float x, float y);   // Déclare la fonction qui ajoute un obstacle à partir d'une position réelle.

#endif /* INC_MAP_H_ */   // Termine la garde d'inclusion de ce fichier d'en-tête.