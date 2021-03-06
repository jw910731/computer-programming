#include "hash_map.h"
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static void entry_free(Entry *e, int d){
    for(int i = 0 ; i < (1<<8) ; ++i){
        if(e->t[i] != NULL){
            if(d < 3) {
                entry_free((Entry *) e->t[i], d + 1);
            } else {
                free(e->t[i]);
            }
        }
    }
    free(e);
}

uint32_t hash_str(const char *s){
    uint32_t ret = 0;
    uint64_t p = 1;
    while(*s){
        ret = (ret + ((uint64_t)(*s) * p) % PRM2) % PRM2;
        p = (p*PRM1) % PRM2;
        ++s;
    }
    return ret;
}

HashTable *table_create(){
    HashTable *ret = calloc(1, sizeof(HashTable));
    ret->e = calloc(1, sizeof(Entry));
    return ret;
}

void table_free(HashTable **t){
    if(t == NULL || *t == NULL) return;
    entry_free((*t)->e, 0);
    free(*t);
    *t = NULL;
}


void **entry_allocate(HashTable *t, uint32_t hash) {
    if(t == NULL) return NULL;
    Entry *e = t->e;
    void **ret;
    for(int i = 0 ; i < 4 ; ++i){
        if(e->t[MASK(hash, 3-i)] == NULL && i < 3){
            e->t[MASK(hash, 3-i)] = calloc(1, sizeof(Entry));
        }
        if(i < 3){
            e = (Entry*)e->t[MASK(hash, 3-i)];
        }
        else{
            ret = &(e->t[MASK(hash, 3-i)]);
        }
    }
    return ret;
}

void **entry_query(HashTable *t, uint32_t hash) {
    if(t == NULL) return NULL;
    if(t->e == NULL) return NULL;
    Entry *e = t->e;
    void **ret;
    for(int i = 0 ; i < 4 ; ++i){
        if(e->t[MASK(hash, 3-i)] == NULL){
            return NULL;
        }
        if(i < 3){
            e = (Entry*)e->t[MASK(hash, 3-i)];
        }
        else{
            ret = &(e->t[MASK(hash, 3-i)]);
        }
    }
    return ret;
}

void table_emplace(HashTable *t, const char *key, char *s){
    if(t == NULL) return ;
    uint32_t hash = hash_str(key);
    void **p = entry_allocate(t, hash);
    assert(*p == NULL);
    size_t size = strlen(s);
    *p = calloc(size+1, sizeof(char));
    strncpy(*p, s, size);
    ((char*)*p)[size] = 0;
}

void *table_query(HashTable *t, const char *key){
    if(t == NULL) return NULL;
    return *entry_query(t, hash_str(key));
}