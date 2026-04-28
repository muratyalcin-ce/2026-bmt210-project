#include "hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned int hash(const char* key)
{
    unsigned int h = 0;
    while (*key)
        h = h * 31 + *key++;
    return h % TABLE_SIZE;
}

void InitHashMap(HashMap* map)
{
    for (int i = 0; i < TABLE_SIZE; i++)
        map->table[i] = NULL;
}

void Insert(HashMap* map, const char* key, Tool value)
{
    unsigned int idx = hash(key);

    HashNode* node = (HashNode*) malloc(sizeof(HashNode));
    strcpy(node->key, key);
    node->value = value;
    node->next = map->table[idx];

    map->table[idx] = node;
}

Tool* Get(HashMap* map, const char* key)
{
    unsigned int idx = hash(key);
    HashNode* cur = map->table[idx];

    while (cur)
    {
        if (strcmp(cur->key, key) == 0)
            return &cur->value;
        cur = cur->next;
    }

    return NULL;
}
