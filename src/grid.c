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

