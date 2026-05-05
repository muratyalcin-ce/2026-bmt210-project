#ifndef HASHMAP_H
#define HASHMAP_H

#include "tool.h"

#define TABLE_SIZE 10

typedef struct HashNode {
    char key[32];
    Tool value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* table[TABLE_SIZE];
} HashMap;

void InitHashMap(HashMap* map);
void Insert(HashMap* map, const char* key, Tool value);
Tool* Get(HashMap* map, const char* key);

#endif
