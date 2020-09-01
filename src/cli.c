#include "cli.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static bool parse_int(const char *text, int *out)
{
    if (text == NULL || *text == '\0') {
        return false;
    }

    char *end = NULL;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0') {
        return false;
    }
    if (value < INT_MIN || value > INT_MAX) {
        return false;
    }

    *out = (int)value;
    return true;
}

static bool parse_seed(const char *text, unsigned int *out)
{
    if (text == NULL || *text == '\0') {
        return false;
    }

    char *end = NULL;
    errno = 0;
    unsigned long value = strtoul(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0' || value > UINT_MAX) {
        return false;
    }

    *out = (unsigned int)value;
    return true;
}

static bool need_value(int argc, int *index, char *error,
                       size_t error_size, const char *option)
{
    if (*index + 1 >= argc) {
        snprintf(error, error_size, "option %s requires a value", option);
        return false;
    }
    ++*index;
    return true;
}

void cli_options_init(life_options *opts)
{
    memset(opts, 0, sizeof(*opts));
    opts->width = 40;
    opts->height = 20;
    opts->generations = 100;
    opts->delay_ms = 90;
    opts->seed = 1;
    opts->ansi = true;
}

bool cli_parse(int argc, char **argv, life_options *opts, char *error,
               size_t error_size)
{
    for (int i = 1; i < argc; ++i) {
        const char *arg = argv[i];

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            opts->help = true;
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            opts->version = true;
        } else if (strcmp(arg, "--wrap") == 0) {
            opts->wrap = true;
        } else if (strcmp(arg, "--ascii") == 0) {
            opts->ansi = false;
        } else if (strcmp(arg, "--stats") == 0) {
            opts->stats = true;
        } else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--width") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->width)) {
                snprintf(error, error_size, "invalid width '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-H") == 0 || strcmp(arg, "--height") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->height)) {
                snprintf(error, error_size, "invalid height '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-g") == 0 || strcmp(arg, "--generations") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->generations)) {
                snprintf(error, error_size, "invalid generation count '%s'",
                         argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-d") == 0 || strcmp(arg, "--delay") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->delay_ms)) {
                snprintf(error, error_size, "invalid delay '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-x") == 0 || strcmp(arg, "--offset-x") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->offset_x)) {
                snprintf(error, error_size, "invalid offset '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-y") == 0 || strcmp(arg, "--offset-y") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_int(argv[i], &opts->offset_y)) {
                snprintf(error, error_size, "invalid offset '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "--seed") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)
                || !parse_seed(argv[i], &opts->seed)) {
                snprintf(error, error_size, "invalid seed '%s'", argv[i]);
                return false;
            }
        } else if (strcmp(arg, "-p") == 0 || strcmp(arg, "--pattern") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)) {
                return false;
            }
            opts->pattern_path = argv[i];
        } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--output") == 0) {
            if (!need_value(argc, &i, error, error_size, arg)) {
                return false;
            }
            opts->output_path = argv[i];
        } else if (arg[0] == '-') {
            snprintf(error, error_size, "unknown option '%s'", arg);
            return false;
        } else if (opts->pattern_path == NULL) {
            opts->pattern_path = arg;
        } else {
            snprintf(error, error_size, "unexpected argument '%s'", arg);
            return false;
        }
    }

    if (opts->width <= 0 || opts->height <= 0) {
        snprintf(error, error_size, "grid dimensions must be positive");
        return false;
    }
    if (opts->width > 1000 || opts->height > 1000) {
        snprintf(error, error_size, "grid dimensions must be 1000 or less");
        return false;
    }
    if (opts->generations < 0) {
        snprintf(error, error_size, "generation count cannot be negative");
        return false;
    }
    if (opts->delay_ms < 0) {
        snprintf(error, error_size, "delay cannot be negative");
        return false;
    }

    return true;
}

