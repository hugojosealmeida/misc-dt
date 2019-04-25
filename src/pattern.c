#include "pattern.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_MAX_LINE 4096

static bool alive_char(int ch)
{
    return ch == 'O' || ch == 'o' || ch == '#' || ch == '*';
}

static bool dead_char(int ch)
{
    return ch == '.' || ch == ' ';
}

bool pattern_load_plain(grid *g, FILE *in, int offset_x, int offset_y)
{
    if (g == NULL || in == NULL) {
        return false;
    }

    char line[PATTERN_MAX_LINE];
    int row = 0;

    while (fgets(line, sizeof line, in) != NULL) {
        if (line[0] == '!' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        int column = 0;
        for (const char *cursor = line; *cursor != '\0'; ++cursor) {
            unsigned char ch = (unsigned char)*cursor;
            if (ch == '\n' || ch == '\r') {
                break;
            }
            if (alive_char(ch)) {
                grid_set(g, offset_x + column, offset_y + row, true);
            } else if (!dead_char(ch)) {
                return false;
            }
            ++column;
        }
        ++row;
    }

    return true;
}

bool pattern_save_plain(const grid *g, FILE *out)
{
    if (g == NULL || out == NULL) {
        return false;
    }

    fprintf(out, "!Game of Life pattern, %dx%d\n", g->width, g->height);
    for (int y = 0; y < g->height; ++y) {
        for (int x = 0; x < g->width; ++x) {
            fputc(grid_get(g, x, y) ? 'O' : '.', out);
        }
        fputc('\n', out);
    }

    return !ferror(out);
}

