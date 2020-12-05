#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef DEBUG
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...) (void)0
#endif

typedef uint8_t byte;
typedef int32_t i32;
typedef int64_t i64;
typedef double fp;

void err(const char *s);