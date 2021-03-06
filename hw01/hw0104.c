#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basic.h"
#include "hash_map.h"

int main(){
    HashTable *t = table_create();
    table_emplace(t, "HELLO", "hello");
    char *s1 = (char *)*(entry_query(t, hash_str("HELLO")));
    printf("%s\n", s1);
    void **p = entry_query(t, hash_str("Hello"));
    printf("%p\n", p);
    table_free(&t);
    return 0;
}