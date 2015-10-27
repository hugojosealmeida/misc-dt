#ifndef LIFE_LIFE_H
#define LIFE_LIFE_H

#include <stdbool.h>

#include "grid.h"

/* Called once after every generation with the updated grid. */
typedef void (*life_observer)(const grid *g, int generation, void *context);

int life_count_neighbors(const grid *g, int x, int y, bool wrap);
bool life_next_state(bool alive, int neighbors);
void life_step(const grid *current, grid *next, bool wrap);
int life_run(grid *g, int generations, bool wrap, life_observer observer,
             void *context);

#endif /* LIFE_LIFE_H */
