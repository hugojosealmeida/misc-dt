#ifndef LIFE_TEST_H
#define LIFE_TEST_H

#include <stdio.h>

typedef void (*test_fn)(void);

void test_register(const char *name, test_fn fn);
void test_fail(const char *file, int line, const char *expression);

#if defined(__GNUC__) || defined(__clang__)
#define TEST(name)                                                            \
    static void name(void);                                                   \
    static void name##_register(void) __attribute__((constructor));           \
    static void name##_register(void) { test_register(#name, name); }         \
    static void name(void)
#else
#error "the test harness needs constructor attribute support"
#endif

#define CHECK(expression)                                                     \
    do {                                                                      \
        if (!(expression)) {                                                  \
            test_fail(__FILE__, __LINE__, #expression);                       \
        }                                                                     \
    } while (0)

#define CHECK_INT(actual, expected)                                           \
    do {                                                                      \
        long actual_value = (long)(actual);                                   \
        long expected_value = (long)(expected);                               \
        if (actual_value != expected_value) {                                 \
            char message[192];                                                \
            snprintf(message, sizeof message, "%s == %ld, expected %ld",      \
                     #actual, actual_value, expected_value);                  \
            test_fail(__FILE__, __LINE__, message);                           \
        }                                                                     \
    } while (0)

#endif /* LIFE_TEST_H */
