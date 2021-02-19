#include "test.h"

#include "grid.h"
#include "life.h"

#include <stdio.h>
#include <string.h>

TEST(grid_create_reports_dimensions)
{
    grid *g = grid_create(4, 3);
    CHECK(g != NULL);
    CHECK_INT(g->width, 4);
    CHECK_INT(g->height, 3);
    CHECK_INT(grid_count_alive(g), 0);
    grid_destroy(g);
}

TEST(grid_create_rejects_invalid_dimensions)
{
    CHECK(grid_create(0, 4) == NULL);
    CHECK(grid_create(4, 0) == NULL);
    CHECK(grid_create(-1, 4) == NULL);
}

TEST(grid_clear_removes_every_cell)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);
    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 2, 2, true));
    grid_clear(g);
    CHECK_INT(grid_count_alive(g), 0);
    grid_destroy(g);
}

TEST(grid_set_and_get_round_trip)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);

    CHECK(grid_set(g, 1, 2, true));
    CHECK(grid_get(g, 1, 2));
    CHECK(!grid_get(g, 2, 1));

    CHECK(grid_set(g, 1, 2, false));
    CHECK(!grid_get(g, 1, 2));

    grid_destroy(g);
}

TEST(grid_access_rejects_out_of_bounds)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);

    CHECK(!grid_get(g, -1, 0));
    CHECK(!grid_get(g, 0, -1));
    CHECK(!grid_get(g, 3, 0));
    CHECK(!grid_get(g, 0, 3));
    CHECK(!grid_set(g, 3, 3, true));

    CHECK(!grid_in_bounds(NULL, 0, 0));
    CHECK(!grid_in_bounds(g, 3, 2));
    CHECK(grid_in_bounds(g, 2, 2));

    grid_destroy(g);
}

TEST(grid_count_alive_counts_every_live_cell)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);

    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 1, 1, true));
    CHECK(grid_set(g, 2, 2, true));
    CHECK_INT(grid_count_alive(g), 3);
    CHECK_INT(grid_count_alive(NULL), 0);

    grid_destroy(g);
}

TEST(grid_copy_duplicates_the_buffer)
{
    grid *source = grid_create(2, 2);
    grid *target = grid_create(2, 2);
    CHECK(source != NULL);
    CHECK(target != NULL);

    CHECK(grid_set(source, 0, 1, true));
    CHECK(grid_copy(target, source));
    CHECK(grid_equals(source, target));
    CHECK_INT(grid_hash(source), grid_hash(target));

    grid_destroy(source);
    grid_destroy(target);
}

TEST(grid_equals_compares_dimensions_and_cells)
{
    grid *a = grid_create(2, 2);
    grid *b = grid_create(2, 2);
    grid *c = grid_create(3, 2);
    CHECK(a != NULL);
    CHECK(b != NULL);
    CHECK(c != NULL);

    CHECK(grid_equals(a, b));
    CHECK(!grid_equals(a, c));
    CHECK(grid_set(a, 1, 0, true));
    CHECK(!grid_equals(a, b));

    grid_destroy(a);
    grid_destroy(b);
    grid_destroy(c);
}

TEST(life_counts_eight_neighbors_in_a_block)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);

    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 3; ++x) {
            CHECK(grid_set(g, x, y, true));
        }
    }

    CHECK_INT(life_count_neighbors(g, 1, 1, false), 8);
    CHECK_INT(life_count_neighbors(g, 1, 1, true), 8);

    grid_destroy(g);
}

TEST(life_ignores_missing_neighbors_without_wrapping)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);
    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 1, 0, true));

    CHECK_INT(life_count_neighbors(g, 0, 0, false), 1);
    CHECK_INT(life_count_neighbors(g, 2, 2, false), 0);

    grid_destroy(g);
}

TEST(life_wraps_neighbors_around_the_edges)
{
    grid *g = grid_create(4, 4);
    CHECK(g != NULL);
    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 3, 3, true));

    CHECK_INT(life_count_neighbors(g, 0, 0, true), 1);
    CHECK_INT(life_count_neighbors(g, 0, 0, false), 0);

    grid_destroy(g);
}

TEST(life_applies_the_b3s23_rule_table)
{
    CHECK(life_next_state(true, 0) == false);
    CHECK(life_next_state(true, 1) == false);
    CHECK(life_next_state(true, 2) == true);
    CHECK(life_next_state(true, 3) == true);
    CHECK(life_next_state(true, 4) == false);
    CHECK(life_next_state(true, 8) == false);

    CHECK(life_next_state(false, 0) == false);
    CHECK(life_next_state(false, 2) == false);
    CHECK(life_next_state(false, 3) == true);
    CHECK(life_next_state(false, 4) == false);
}

TEST(life_step_turns_a_blinker_into_a_vertical_bar)
{
    grid *current = grid_create(5, 5);
    grid *next = grid_create(5, 5);
    CHECK(current != NULL);
    CHECK(next != NULL);

    CHECK(grid_set(current, 1, 2, true));
    CHECK(grid_set(current, 2, 2, true));
    CHECK(grid_set(current, 3, 2, true));

    life_step(current, next, false);

    CHECK(grid_get(next, 2, 1));
    CHECK(grid_get(next, 2, 2));
    CHECK(grid_get(next, 2, 3));
    CHECK_INT(grid_count_alive(next), 3);

    grid_destroy(current);
    grid_destroy(next);
}

TEST(life_step_keeps_a_block_alive)
{
    grid *current = grid_create(4, 4);
    grid *next = grid_create(4, 4);
    CHECK(current != NULL);
    CHECK(next != NULL);

    CHECK(grid_set(current, 1, 1, true));
    CHECK(grid_set(current, 2, 1, true));
    CHECK(grid_set(current, 1, 2, true));
    CHECK(grid_set(current, 2, 2, true));

    life_step(current, next, false);

    CHECK(grid_equals(current, next));

    grid_destroy(current);
    grid_destroy(next);
}

static int observed_generations;

static void count_generation(const grid *g, int generation, void *context)
{
    (void)g;
    (void)context;
    observed_generations = generation;
}

TEST(life_run_advances_the_requested_number_of_generations)
{
    grid *g = grid_create(5, 5);
    CHECK(g != NULL);
    CHECK(grid_set(g, 1, 2, true));
    CHECK(grid_set(g, 2, 2, true));
    CHECK(grid_set(g, 3, 2, true));

    observed_generations = 0;
    CHECK_INT(life_run(g, 2, false, count_generation, NULL), 2);
    CHECK_INT(observed_generations, 2);
    CHECK_INT(grid_count_alive(g), 3);

    grid_destroy(g);
}

TEST(life_run_handles_zero_and_invalid_input)
{
    grid *g = grid_create(3, 3);
    CHECK(g != NULL);

    CHECK_INT(life_run(g, 0, false, NULL, NULL), 0);
    CHECK_INT(life_run(NULL, 1, false, NULL, NULL), -1);
    CHECK_INT(life_run(g, -1, false, NULL, NULL), -1);

    grid_destroy(g);
}

#include "io.h"

TEST(io_render_ascii_writes_the_expected_rows)
{
    grid *g = grid_create(3, 2);
    CHECK(g != NULL);
    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 2, 0, true));
    CHECK(grid_set(g, 1, 1, true));

    FILE *out = tmpfile();
    CHECK(out != NULL);
    io_render_ascii(g, out);
    rewind(out);

    char buffer[32];
    CHECK(fgets(buffer, sizeof buffer, out) != NULL);
    CHECK(strcmp(buffer, "#.#\n") == 0);
    CHECK(fgets(buffer, sizeof buffer, out) != NULL);
    CHECK(strcmp(buffer, ".#.\n") == 0);

    fclose(out);
    grid_destroy(g);
}

#include "pattern.h"

TEST(pattern_load_plain_parses_cells_and_comments)
{
    grid *g = grid_create(5, 5);
    CHECK(g != NULL);

    FILE *in = tmpfile();
    CHECK(in != NULL);
    fputs("!glider\n.O.\n..O\nOOO\n", in);
    rewind(in);

    CHECK(pattern_load_plain(g, in, 1, 1));
    CHECK_INT(grid_count_alive(g), 5);
    CHECK(grid_get(g, 2, 1));
    CHECK(grid_get(g, 3, 2));
    CHECK(grid_get(g, 1, 3));

    fclose(in);
    grid_destroy(g);
}

TEST(pattern_load_rle_expands_runs)
{
    grid *g = grid_create(6, 4);
    CHECK(g != NULL);

    FILE *in = tmpfile();
    CHECK(in != NULL);
    fputs("#N glider\nx = 3, y = 3, rule = B3/S23\nbo$2bo$3o!\n", in);
    rewind(in);

    CHECK(pattern_load_rle(g, in, 1, 0));
    CHECK_INT(grid_count_alive(g), 5);
    CHECK(grid_get(g, 2, 0));
    CHECK(grid_get(g, 3, 1));
    CHECK(grid_get(g, 1, 2));

    fclose(in);
    grid_destroy(g);
}

TEST(pattern_rle_rejects_a_missing_header)
{
    grid *g = grid_create(4, 4);
    CHECK(g != NULL);

    FILE *in = tmpfile();
    CHECK(in != NULL);
    fputs("bo$2bo!\n", in);
    rewind(in);

    CHECK(!pattern_load_rle(g, in, 0, 0));

    fclose(in);
    grid_destroy(g);
}

TEST(pattern_save_plain_writes_a_readable_grid)
{
    grid *g = grid_create(2, 2);
    CHECK(g != NULL);
    CHECK(grid_set(g, 0, 0, true));
    CHECK(grid_set(g, 1, 1, true));

    FILE *out = tmpfile();
    CHECK(out != NULL);
    CHECK(pattern_save_plain(g, out));
    rewind(out);

    char buffer[64];
    CHECK(fgets(buffer, sizeof buffer, out) != NULL);
    CHECK(strncmp(buffer, "!Game of Life", 13) == 0);
    CHECK(fgets(buffer, sizeof buffer, out) != NULL);
    CHECK(strcmp(buffer, "O.\n") == 0);
    CHECK(fgets(buffer, sizeof buffer, out) != NULL);
    CHECK(strcmp(buffer, ".O\n") == 0);

    fclose(out);
    grid_destroy(g);
}

#include "cli.h"

TEST(cli_parse_keeps_the_documented_defaults)
{
    life_options options;
    cli_options_init(&options);
    CHECK_INT(options.width, 40);
    CHECK_INT(options.height, 20);
    CHECK_INT(options.generations, 100);
    CHECK(options.ansi);
    CHECK(!options.wrap);
}

TEST(cli_parse_reads_flags_and_values)
{
    char *argv[] = {"life",     "--width", "12", "-H",   "8",
                    "--wrap",   "--seed",  "42", "-g",   "5",
                    "--ascii",  "--stats", "glider.txt"};
    life_options options;
    cli_options_init(&options);

    char error[128] = "";
    CHECK(cli_parse(13, argv, &options, error, sizeof error));
    CHECK_INT(options.width, 12);
    CHECK_INT(options.height, 8);
    CHECK_INT(options.generations, 5);
    CHECK_INT(options.seed, 42);
    CHECK(options.wrap);
    CHECK(!options.ansi);
    CHECK(options.stats);
    CHECK(strcmp(options.pattern_path, "glider.txt") == 0);
}

TEST(cli_parse_rejects_bad_input)
{
    char *unknown[] = {"life", "--bogus"};
    char *missing[] = {"life", "--width"};
    char *invalid[] = {"life", "--width", "wide"};
    life_options options;
    char error[128];

    cli_options_init(&options);
    CHECK(!cli_parse(2, unknown, &options, error, sizeof error));
    cli_options_init(&options);
    CHECK(!cli_parse(2, missing, &options, error, sizeof error));
    cli_options_init(&options);
    CHECK(!cli_parse(3, invalid, &options, error, sizeof error));
}
