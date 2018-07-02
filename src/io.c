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

void io_render_ansi(const grid *g, FILE *out)
{
    if (g == NULL || out == NULL) {
        return;
    }

    for (int y = 0; y < g->height; ++y) {
        for (int x = 0; x < g->width; ++x) {
            if (grid_get(g, x, y)) {
                fputs("\033[1;32m#\033[0m", out);
            } else {
                fputs("\033[2m.\033[0m", out);
            }
        }
        fputc('\n', out);
    }
}

void io_clear_screen(FILE *out)
{
    if (out == NULL) {
        return;
    }
    fputs("\033[H\033[2J", out);
}
