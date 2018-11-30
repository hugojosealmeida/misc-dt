#ifndef LIFE_PATTERN_H
#define LIFE_PATTERN_H

#include <stdbool.h>
#include <stdio.h>

#include "grid.h"

bool pattern_load_plain(grid *g, FILE *in, int offset_x, int offset_y);
bool pattern_load_rle(grid *g, FILE *in, int offset_x, int offset_y);
bool pattern_load_file(grid *g, const char *path, int offset_x, int offset_y);
bool pattern_save_plain(const grid *g, FILE *out);

#endif /* LIFE_PATTERN_H */
