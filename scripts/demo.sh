#!/bin/sh
set -eu

BIN=${BIN:-./build/life}
PATTERN=${1:-examples/glider.txt}

"$BIN" --wrap --generations 200 --pattern "$PATTERN"
