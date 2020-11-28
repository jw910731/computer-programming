#include <stdio.h>
#include <stdlib.h>

void err(const char *s){
	fprintf(stderr, "%s\nThus program is going to terminate.\n", s);
	exit(1);
}