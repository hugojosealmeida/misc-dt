#ifndef LIFE_CLI_H
#define LIFE_CLI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    int width;
    int height;
    int generations;
    int delay_ms;
    int offset_x;
    int offset_y;
    unsigned int seed;
    bool wrap;
    bool ansi;
    bool stats;
    bool help;
    bool version;
    const char *pattern_path;
    const char *output_path;
} life_options;

void cli_options_init(life_options *opts);
bool cli_parse(int argc, char **argv, life_options *opts, char *error,
               size_t error_size);
void cli_print_usage(FILE *out, const char *program);
void cli_print_version(FILE *out);

#endif /* LIFE_CLI_H */
