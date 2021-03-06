#include "hash_map.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static void entry_free(Entry *e, int d){
    for(int i = 0 ; i < (1<<LAY_SIZE) ; ++i){
        if(e->t[i] != NULL){
            if(d < LAYER-1) {
                entry_free((Entry *) e->t[i], d + 1);
            } else {
                Item *p = (Item*)(e->t[i]);
                while(p != NULL){
                    Item *tmp = p->next;
                    free(p->data);
                    free(p);
                    p = tmp;
                }
            }
        }
    }
    free(e);
}

uint32_t hash_str(const char *s){
    uint32_t ret = 0;
    uint64_t p = 1;
    while(*s){
        ret = (ret + (uint32_t)(((uint64_t)(*s) * p) % PRM2)) % PRM2;
        p = (p*PRM1) % PRM2;
        ++s;
    }
    return ret;
}

uint32_t backup_hash(const char *s){
    uint32_t ret = 0;
    uint64_t p = 1;
    while(*s){
        ret = (ret + (uint32_t)(((uint64_t)(*s) * p) % PRM1)) % PRM1;
        p = (p*PRM3) % PRM1;
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
    for(int i = 0 ; i < LAYER ; ++i){
        if(e->t[MASK(hash, LAYER-1-i)] == NULL && i < LAYER-1){
            e->t[MASK(hash, LAYER-1-i)] = calloc(1, sizeof(Entry));
        }
        if(i < LAYER-1){
            e = (Entry*)e->t[MASK(hash, LAYER-1-i)];
        }
        else{
            ret = &(e->t[MASK(hash, LAYER-1-i)]);
        }
    }
    return ret;
}

void **entry_query(HashTable *t, uint32_t hash) {
    if(t == NULL) return NULL;
    Entry *e = t->e;
    void **ret;
    for(int i = 0 ; i < LAYER ; ++i){
        if(e->t[MASK(hash, LAYER-1-i)] == NULL && i < LAYER - 1){
            return NULL;
        }
        if(i < LAYER - 1){
            e = (Entry*)e->t[MASK(hash, LAYER-1-i)];
        }
        else{
            ret = &(e->t[MASK(hash, LAYER-1-i)]);
        }
    }
    return ret;
}

void table_emplace(HashTable *t, const char *key, char *s){
    if(t == NULL) return ;
    uint32_t hash = hash_str(key);
    uint32_t backup = backup_hash(key);
    // points to array pointer
    void **p = entry_allocate(t, hash);
    Item *item;
    // special case to NULL item in hash table (No collision)
    if(*p == NULL){
        *p = calloc(1, sizeof(Item));
        item = (Item *) *p;
    }
    else{ // on collision
        // copied of pointer
        item = (Item*)(*p);
        // Pointer reference for create new node in next
        Item **ptr = &item;
        while(item != NULL){
            if(item->backup_hash == backup){
                break;
            }
            ptr = &item->next;
            item = item->next;
        }
        if(*ptr == NULL){
            // update prev -> next
            item = *ptr = calloc(1, sizeof(Item));
        }
    }
    if(item->data != NULL){
        free(item->data);
    }
    size_t size = strlen(s);
    item->data = calloc(size+1, sizeof(char));
    assert(item->data != NULL);
    strncpy(item->data, s, size);
    item->data[size] = 0;
    if(item->backup_hash != backup){
        size = strlen(key);
        item->backup_hash = backup;
    }
}

Item *table_query(HashTable *t, const char *key){
    if(t == NULL) return NULL;
    void **p = entry_allocate(t, hash_str(key));
    uint32_t backup = backup_hash(key);
    if(*p == NULL) return *p;
    Item *i = (Item*)*p;
    while(i->backup_hash != backup){
        if(i->next != NULL){
            i = i->next;
        }
        else{
            return NULL;
        }
    }
    return i;
}