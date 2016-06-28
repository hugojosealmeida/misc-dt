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

void life_step(const grid *current, grid *next, bool wrap)
{
    for (int y = 0; y < current->height; ++y) {
        for (int x = 0; x < current->width; ++x) {
            int neighbors = life_count_neighbors(current, x, y, wrap);
            bool alive = grid_get(current, x, y);
            grid_set(next, x, y, life_next_state(alive, neighbors));
        }
    }
}

int life_run(grid *g, int generations, bool wrap, life_observer observer,
             void *context)
{
    if (g == NULL || generations < 0) {
        return -1;
    }
    if (generations == 0) {
        return 0;
    }

    grid *scratch = grid_create(g->width, g->height);
    if (scratch == NULL) {
        return -1;
    }

    for (int generation = 1; generation <= generations; ++generation) {
        life_step(g, scratch, wrap);
        if (!grid_copy(g, scratch)) {
            grid_destroy(scratch);
            return -1;
        }
        if (observer != NULL) {
            observer(g, generation, context);
        }
    }

    grid_destroy(scratch);
    return generations;
}
