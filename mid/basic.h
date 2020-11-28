#ifndef BASIC_H
#define BASIC_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t i32;
typedef int64_t i64;

void err(const char *s){
	fprintf(stderr, "%s\nThus program is going to terminate.", s);
	exit(1);
}

#endif