#include "life.h"

#include <stdlib.h>

static int wrap_index(int value, int limit)
{
    int result = value % limit;
    return result < 0 ? result + limit : result;
}

