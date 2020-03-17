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

static bool parse_rle_header(const char *line, int *width, int *height)
{
    int parsed_width = 0;
    int parsed_height = 0;

    if (sscanf(line, "x = %d , y = %d", &parsed_width, &parsed_height) != 2
        && sscanf(line, "x=%d,y=%d", &parsed_width, &parsed_height) != 2) {
        return false;
    }

    if (width != NULL) {
        *width = parsed_width;
    }
    if (height != NULL) {
        *height = parsed_height;
    }
    return true;
}

bool pattern_load_rle(grid *g, FILE *in, int offset_x, int offset_y)
{
    if (g == NULL || in == NULL) {
        return false;
    }

    char line[PATTERN_MAX_LINE];
    bool header_seen = false;

    while (fgets(line, sizeof line, in) != NULL) {
        if (line[0] == '#') {
            continue;
        }
        if (!header_seen) {
            if (!parse_rle_header(line, NULL, NULL)) {
                return false;
            }
            header_seen = true;
            break;
        }
    }

    if (!header_seen) {
        return false;
    }

    long column = 0;
    long row = 0;
    long run = 0;
    int ch;

    while ((ch = fgetc(in)) != EOF) {
        if (ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t') {
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            run = run * 10 + (ch - '0');
            continue;
        }
        if (run == 0) {
            run = 1;
        }

        switch (ch) {
        case 'b':
            column += run;
            break;
        case 'o':
            for (long i = 0; i < run; ++i) {
                grid_set(g, offset_x + (int)column + (int)i,
                         offset_y + (int)row, true);
            }
            column += run;
            break;
        case '$':
            row += run;
            column = 0;
            break;
        case '!':
            return true;
        default:
            return false;
        }
        run = 0;
    }

    return true;
}

static bool has_rle_extension(const char *path)
{
    size_t length = strlen(path);
    return length >= 4 && strcmp(path + length - 4, ".rle") == 0;
}

bool pattern_load_file(grid *g, const char *path, int offset_x, int offset_y)
{
    if (g == NULL || path == NULL) {
        return false;
    }

    FILE *in = fopen(path, "r");
    if (in == NULL) {
        return false;
    }

    bool ok = has_rle_extension(path)
                  ? pattern_load_rle(g, in, offset_x, offset_y)
                  : pattern_load_plain(g, in, offset_x, offset_y);
    fclose(in);
    return ok;
}
