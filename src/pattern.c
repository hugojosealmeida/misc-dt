#include "pattern.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN_MAX_LINE 4096

static bool alive_char(int ch)
{
    return ch == 'O' || ch == 'o' || ch == '#' || ch == '*';
}

static bool dead_char(int ch)
{
    return ch == '.' || ch == ' ';
}

