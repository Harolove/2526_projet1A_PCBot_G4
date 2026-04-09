#include "map.h"
#include <string.h>

void MAP_Init(MapGrid *grid) {
    memset(grid->grid, 0, sizeof(grid->grid));
}

void MAP_AddObstacle(MapGrid *grid, float x, float y) {
    int ix = (int)(x / MAP_RESOLUTION + MAP_WIDTH / 2);
    int iy = (int)(y / MAP_RESOLUTION + MAP_HEIGHT / 2);

    if (ix >= 0 && ix < MAP_WIDTH && iy >= 0 && iy < MAP_HEIGHT) {
        grid->grid[iy][ix] += 0.2f;

        if (grid->grid[iy][ix] > 1.0f) {
            grid->grid[iy][ix] = 1.0f;
        }
    }
}
