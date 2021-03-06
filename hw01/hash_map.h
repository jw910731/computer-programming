#define PRM1 1000000007U
#define PRM2 1000000009U
#define MASK(x, n) (((x)&((uint32_t)(0xff) << (8*(n)))) >> (8*(n)))

#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    // pointer
    void *t[1<<8];
}Entry;

typedef struct {
    Entry *e;
}HashTable;


HashTable *table_create();

void table_free(HashTable **t);

uint32_t hash_str(const char *s);

// Emplace a data to table
void table_emplace(HashTable *t, const char *key, char *s);

// Access a data from table
void *table_query(HashTable *t, const char *key);

// Allocate Entry
void **entry_allocate(HashTable *t, uint32_t hash);

// Query Entry
void **entry_query(HashTable *t, uint32_t hash);