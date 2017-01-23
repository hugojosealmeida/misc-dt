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

