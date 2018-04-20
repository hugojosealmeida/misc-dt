#ifndef LIFE_IO_H
#define LIFE_IO_H

#include <stdio.h>

#include "grid.h"

void io_render_ascii(const grid *g, FILE *out);
void io_render_ansi(const grid *g, FILE *out);
void io_clear_screen(FILE *out);

#endif /* LIFE_IO_H */
