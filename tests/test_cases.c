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

