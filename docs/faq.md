# FAQ

## Does the project depend on ncurses?

No. The renderer only writes ANSI escape sequences, so the binary has no
runtime dependencies beyond the C standard library.

## Why is the grid size fixed?

Patterns in the Game of Life can grow without bound. A fixed grid keeps the
simulation bounded and makes the rules easy to reason about. Use `--wrap` if
you want the simulation to continue at the edges.

## How do I add a pattern?

Drop a plain text or `.rle` file into `examples/` and load it with
`--pattern`. Plain text uses `.` for dead cells and `O` for live cells.
