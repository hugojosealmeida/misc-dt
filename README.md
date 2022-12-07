# Game of Life

A small, dependency-free implementation of Conway's Game of Life written in
C11.

## Build

```sh
make
```

The binary is written to `build/life`.

## Run

```sh
./build/life --width 40 --height 20 --generations 200 --wrap
```

Load a pattern and render a single plain text frame:

```sh
./build/life --ascii --pattern examples/glider.txt --generations 16
```

## Test

```sh
make test
```

## License

MIT
