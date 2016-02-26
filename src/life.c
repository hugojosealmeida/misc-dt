#include "life.h"

#include <stdlib.h>

static int wrap_index(int value, int limit)
{
    int result = value % limit;
    return result < 0 ? result + limit : result;
}

int life_count_neighbors(const grid *g, int x, int y, bool wrap)
{
    if (g == NULL) {
        return 0;
    }

    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int nx = x + dx;
            int ny = y + dy;
            if (wrap) {
                nx = wrap_index(nx, g->width);
                ny = wrap_index(ny, g->height);
            } else if (!grid_in_bounds(g, nx, ny)) {
                continue;
            }

            if (grid_get(g, nx, ny)) {
                ++count;
            }
        }
    }
    return count;
}

bool life_next_state(bool alive, int neighbors)
{
    if (alive) {
        return neighbors == 2 || neighbors == 3;
    }
    return neighbors == 3;
}

