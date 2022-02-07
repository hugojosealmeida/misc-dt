# Rules

The simulation follows Conway's original rule set, usually written as B3/S23:

- A live cell with two or three live neighbours survives.
- A dead cell with exactly three live neighbours becomes alive.
- Every other cell becomes or stays dead.

Each cell looks at the eight cells around it. By default cells outside the
grid are treated as dead. Passing `--wrap` makes the grid toroidal: the top
edge connects to the bottom edge and the left edge connects to the right edge.
