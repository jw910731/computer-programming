#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void err(const char *s) {
    fprintf(stderr, "%s\nThus program is going to terminate.\n", s);
    exit(1);
}

size_t fgets_n(char *s, int size, FILE *stream) {
    char *ptr = fgets(s, size, stream);
    if (ptr != NULL) {
        size_t ret = strlen(ptr);
        if (ptr[ret - 1] == '\n') {
            ptr[--ret] = 0;
        }
        return ret;
    }
    return 0;
}