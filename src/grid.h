#ifndef LIFE_GRID_H
#define LIFE_GRID_H

#include <stdbool.h>
#include <stddef.h>

/*
 * A dense, row-major grid of cells. Every cell is stored as one byte so the
 * buffer can be copied with memcpy and walked without pointer chasing.
 */
typedef struct {
    int width;
    int height;
    unsigned char *cells;
} grid;

grid *grid_create(int width, int height);
void grid_destroy(grid *g);
void grid_clear(grid *g);
bool grid_in_bounds(const grid *g, int x, int y);
bool grid_get(const grid *g, int x, int y);
bool grid_set(grid *g, int x, int y, bool alive);
int grid_count_alive(const grid *g);
bool grid_copy(grid *dst, const grid *src);
bool grid_equals(const grid *a, const grid *b);
unsigned long grid_hash(const grid *g);

#endif /* LIFE_GRID_H */
