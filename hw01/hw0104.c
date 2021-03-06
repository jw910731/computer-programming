#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include "basic.h"
#include "hash_map.h"

void rand_str(char *s, size_t size){
    static const char pool[]="1234567890/*-.,/;'[]{}zxcvbnmasdfghjklqpwoeirutyZMXNCBVALSKDJFHGQPWOEIRUTY";
    for(int i = 0 ; i < (int)(size)-1 ; ++i){
        s[i] = pool[rand()%(sizeof(pool)/ sizeof(*pool))];
    }
    s[size-1] = 0;
}

int main(){
    srand(time(NULL));
    HashTable *t = table_create();
    int epoch = 100000;
    char s1[1000], s2[1000];
    for(int i = 0 ; i < epoch ; ++i){
        rand_str(s1, 1000);
        rand_str(s2, 1000);
        table_emplace(t, s1, s2);
        Item *item = table_query(t, s1);
        assert(strcmp(item->data, s2) == 0);
    }
    table_free(&t);
    return 0;
}