#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cli.h"
#include "grid.h"
#include "io.h"
#include "life.h"
#include "pattern.h"

typedef struct {
    bool ansi;
    int delay_ms;
} render_context;

static void sleep_milliseconds(int milliseconds)
{
    if (milliseconds <= 0) {
        return;
    }

    struct timespec request;
    request.tv_sec = milliseconds / 1000;
    request.tv_nsec = (long)(milliseconds % 1000) * 1000000L;
    nanosleep(&request, NULL);
}

static void render_frame(const grid *g, int generation, void *context)
{
    render_context *render = context;

    if (render->ansi) {
        io_clear_screen(stdout);
        printf("generation %d  alive %d\n\n", generation, grid_count_alive(g));
        io_render_ansi(g, stdout);
    } else {
        io_render_ascii(g, stdout);
    }

    fflush(stdout);
    sleep_milliseconds(render->delay_ms);
}

static void seed_soup(grid *g, unsigned int seed)
{
    srand(seed);
    for (int y = 0; y < g->height; ++y) {
        for (int x = 0; x < g->width; ++x) {
            grid_set(g, x, y, rand() % 100 < 28);
        }
    }
}

int main(int argc, char **argv)
{
    life_options options;
    cli_options_init(&options);

    char error[256];
    if (!cli_parse(argc, argv, &options, error, sizeof error)) {
        fprintf(stderr, "%s: %s\n", argv[0], error);
        fprintf(stderr, "Try '%s --help' for usage.\n", argv[0]);
        return 2;
    }

    if (options.help) {
        cli_print_usage(stdout, argv[0]);
        return 0;
    }
    if (options.version) {
        cli_print_version(stdout);
        return 0;
    }

    grid *board = grid_create(options.width, options.height);
    if (board == NULL) {
        fprintf(stderr, "cannot allocate a %dx%d grid\n", options.width,
                options.height);
        return 1;
    }

    if (options.pattern_path != NULL) {
        if (!pattern_load_file(board, options.pattern_path, options.offset_x,
                               options.offset_y)) {
            fprintf(stderr, "cannot load pattern '%s'\n", options.pattern_path);
            grid_destroy(board);
            return 1;
        }
    } else {
        seed_soup(board, options.seed);
    }

    bool animate = options.ansi && options.output_path == NULL;
    render_context render = {animate, options.delay_ms};

    if (animate) {
        render_frame(board, 0, &render);
    }

    int completed = life_run(board, options.generations, options.wrap,
                             animate ? render_frame : NULL,
                             animate ? &render : NULL);
    if (completed < 0) {
        fprintf(stderr, "simulation failed\n");
        grid_destroy(board);
        return 1;
    }

    if (options.output_path != NULL) {
        FILE *out = fopen(options.output_path, "w");
        if (out == NULL) {
            fprintf(stderr, "cannot write '%s'\n", options.output_path);
            grid_destroy(board);
            return 1;
        }

        bool saved = pattern_save_plain(board, out);
        fclose(out);
        if (!saved) {
            fprintf(stderr, "cannot write '%s'\n", options.output_path);
            grid_destroy(board);
            return 1;
        }
    } else if (!animate) {
        io_render_ascii(board, stdout);
    }

    if (options.stats) {
        fprintf(stderr, "generations: %d\ncells: %d\nalive: %d\n", completed,
                options.width * options.height, grid_count_alive(board));
    }

    grid_destroy(board);
    return 0;
}
