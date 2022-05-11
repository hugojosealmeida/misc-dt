# Design

The code is split into small modules with no global state:

- `src/grid.*` owns the cell buffer and bounds-checked access.
- `src/life.*` implements the rules and the generation loop. The loop reads
  from one grid and writes into a scratch grid, so updates never depend on the
  order of the cells.
- `src/io.*` renders a grid to a stream.
- `src/pattern.*` loads and saves patterns.
- `src/cli.*` parses the command line; `src/main.c` wires the modules together.

The grid stores one byte per cell. That keeps neighbour lookups cheap and lets
the grid be copied with a single `memcpy` per generation.
