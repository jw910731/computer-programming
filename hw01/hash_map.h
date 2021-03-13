#define LAYER 8
#define LAY_SIZE 4
#define MASK(x, n) (((x)&((uint32_t)(0xf) << (LAY_SIZE*(n)))) >> (LAY_SIZE*(n)))

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct item{
    uint32_t backup_hash;
    char *data;
    struct item *next;
}Item;

typedef struct {
    // pointer
    void *t[1<<LAY_SIZE];
}Entry;

typedef struct {
    Entry *e;
}HashTable;


HashTable *table_create();

void table_free(HashTable **t);

uint32_t hash_str(const char *s);
uint32_t backup_hash(const char *s);

// Emplace a data to table
void table_emplace(HashTable *t, const char *key, char *s);

// Access a data from table
Item *table_query(HashTable *t, const char *key);

// Allocate Entry
void **entry_allocate(HashTable *t, uint32_t hash);

// Query Entry
void **entry_query(HashTable *t, uint32_t hash);