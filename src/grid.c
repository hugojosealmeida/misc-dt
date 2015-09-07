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

int grid_count_alive(const grid *g)
{
    if (g == NULL) {
        return 0;
    }

    int alive = 0;
    size_t count = (size_t)g->width * (size_t)g->height;
    for (size_t i = 0; i < count; ++i) {
        alive += g->cells[i] != 0;
    }
    return alive;
}

bool grid_copy(grid *dst, const grid *src)
{
    if (dst == NULL || src == NULL) {
        return false;
    }
    if (dst->width != src->width || dst->height != src->height) {
        return false;
    }

    memcpy(dst->cells, src->cells, (size_t)src->width * (size_t)src->height);
    return true;
}

bool grid_equals(const grid *a, const grid *b)
{
    if (a == b) {
        return true;
    }
    if (a == NULL || b == NULL) {
        return false;
    }
    if (a->width != b->width || a->height != b->height) {
        return false;
    }

    size_t count = (size_t)a->width * (size_t)a->height;
    return memcmp(a->cells, b->cells, count) == 0;
}

unsigned long grid_hash(const grid *g)
{
    if (g == NULL) {
        return 0;
    }

    unsigned long hash = 1469598103934665603UL;
    size_t count = (size_t)g->width * (size_t)g->height;
    for (size_t i = 0; i < count; ++i) {
        hash ^= g->cells[i];
        hash *= 1099511628211UL;
    }
    return hash;
}
