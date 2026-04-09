#ifndef INC_MAP_H_
#define INC_MAP_H_



/* Carte très simple: une grille de cellules occupées ou libres. */
#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define MAP_RESOLUTION 0.1f

typedef struct {
    float grid[MAP_HEIGHT][MAP_WIDTH];
} MapGrid;

void MAP_Init(MapGrid *grid);
void MAP_AddObstacle(MapGrid *grid, float x, float y);

#endif /* INC_MAP_H_ */