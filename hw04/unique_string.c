#include "unique_string.h"

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "basic.h"
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char *unique_str(char *inp) {
    static unsigned int cnt = 0;
    if (cnt == 0) {
        cnt = time(0);
    }
    unsigned int cnt2 = cnt;
    // static char my_rand_s;
    // static u64 my_rand = ((u64)(&my_rand_s));
    for (int i = 0; i < 16; i++) {
        if (rand() % 2 && i) {
            if (rand() % 3 < 2) {
                inp[i] = '_';
            } else {
                inp[i] = cnt2 % 26 + 'A';
            }

        } else {
            inp[i] = cnt2 % 26 + 'a';
            cnt2 /= 26;
        }
    }
    inp[17] = '\0';
    cnt += rand() % 100;
    return inp;
}