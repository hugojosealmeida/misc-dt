#include "grid.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

grid *grid_create(int width, int height)
{
    if (width <= 0 || height <= 0) {
        return NULL;
    }

    size_t count = (size_t)width * (size_t)height;
    if (count / (size_t)width != (size_t)height) {
        return NULL;
    }

    grid *g = malloc(sizeof(*g));
    if (g == NULL) {
        return NULL;
    }

    g->cells = calloc(count, sizeof(*g->cells));
    if (g->cells == NULL) {
        free(g);
        return NULL;
    }

    g->width = width;
    g->height = height;
    return g;
}

void grid_destroy(grid *g)
{
    if (g == NULL) {
        return;
    }
    free(g->cells);
    free(g);
}

void grid_clear(grid *g)
{
    if (g == NULL) {
        return;
    }
    memset(g->cells, 0, (size_t)g->width * (size_t)g->height);
}

bool grid_in_bounds(const grid *g, int x, int y)
{
    return g != NULL && x >= 0 && y >= 0 && x < g->width && y < g->height;
}

bool grid_get(const grid *g, int x, int y)
{
    if (!grid_in_bounds(g, x, y)) {
        return false;
    }
    return g->cells[(size_t)y * (size_t)g->width + (size_t)x] != 0;
}

bool grid_set(grid *g, int x, int y, bool alive)
{
    if (!grid_in_bounds(g, x, y)) {
        return false;
    }
    g->cells[(size_t)y * (size_t)g->width + (size_t)x] = alive ? 1 : 0;
    return true;
}

