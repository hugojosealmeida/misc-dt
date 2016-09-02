#include "test.h"

#include <stdlib.h>

typedef struct test_case {
    const char *name;
    test_fn fn;
    struct test_case *next;
} test_case;

static test_case *head;
static test_case *tail;
static int failures;

void test_register(const char *name, test_fn fn)
{
    test_case *item = malloc(sizeof(*item));
    if (item == NULL) {
        fprintf(stderr, "out of memory while registering '%s'\n", name);
        exit(2);
    }

    item->name = name;
    item->fn = fn;
    item->next = NULL;

    if (tail == NULL) {
        head = item;
    } else {
        tail->next = item;
    }
    tail = item;
}

void test_fail(const char *file, int line, const char *expression)
{
    ++failures;
    fprintf(stderr, "FAIL %s:%d: %s\n", file, line, expression);
}

int main(void)
{
    int total = 0;

    for (test_case *item = head; item != NULL; item = item->next) {
        int before = failures;
        ++total;
        item->fn();
        printf("%s %s\n", failures == before ? "ok  " : "FAIL", item->name);
    }

    printf("\n%d test(s), %d assertion(s) failed\n", total, failures);
    return failures == 0 ? 0 : 1;
}
