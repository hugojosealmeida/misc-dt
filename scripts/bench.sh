#!/bin/sh
set -eu

BIN=${BIN:-./build/life}

echo "200x100 grid, 200 generations"
time "$BIN" --ascii --width 200 --height 100 --generations 200 >/dev/null
