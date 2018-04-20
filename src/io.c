#include "io.h"

void io_render_ascii(const grid *g, FILE *out)
{
    if (g == NULL || out == NULL) {
        return;
    }

    for (int y = 0; y < g->height; ++y) {
        for (int x = 0; x < g->width; ++x) {
            fputc(grid_get(g, x, y) ? '#' : '.', out);
        }
        fputc('\n', out);
    }
}

