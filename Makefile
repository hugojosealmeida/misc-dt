CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Isrc
LDFLAGS ?=

SRC := $(wildcard src/*.c)
TEST_SRC := $(filter-out src/main.c,$(SRC)) tests/test.c tests/test_cases.c
HEADERS := $(wildcard src/*.h) $(wildcard tests/*.h)

BIN := build/life
TEST_BIN := build/test_life

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRC) $(HEADERS)
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

$(TEST_BIN): $(TEST_SRC) $(HEADERS)
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(TEST_SRC) $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf build

SANITIZE_FLAGS := -std=c11 -g -O1 -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: sanitize
sanitize:
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(SANITIZE_FLAGS) -o build/test_san $(TEST_SRC) $(LDFLAGS)
	./build/test_san

