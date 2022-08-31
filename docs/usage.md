# Usage

```
life [options] [pattern]
```

`pattern` is an optional path to a plain text or RLE pattern. Without a
pattern the grid is filled with a reproducible random soup.

| Option | Description |
| --- | --- |
| `-w, --width N` | grid width, default 40 |
| `-H, --height N` | grid height, default 20 |
| `-g, --generations N` | number of generations, default 100 |
| `-d, --delay MS` | delay between frames in milliseconds |
| `-x, --offset-x N` | horizontal offset for the loaded pattern |
| `-y, --offset-y N` | vertical offset for the loaded pattern |
| `--seed N` | seed for the random soup |
| `--wrap` | connect the grid edges |
| `--ascii` | print one plain text frame instead of animating |
| `--stats` | print generation statistics to stderr |
| `-p, --pattern PATH` | load a pattern from a file |
| `-o, --output PATH` | write the final grid to a file |
| `-h, --help` | show the help text |
| `-v, --version` | show the version |

## Examples

```sh
# Animate a glider on a wrapping grid.
./build/life --wrap --pattern examples/glider.txt

# Run the Gosper gun for 300 generations and save the final state.
./build/life -w 80 -H 40 -g 300 -p examples/gosper_glider_gun.rle -o gun.txt

# Render a fixed number of generations without animation.
./build/life --ascii -g 20 -p examples/pulsar.rle
```
