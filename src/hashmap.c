#include "hashmap.h"

#include <stdlib.h>
#include <stdio.h>

void make_hashmap(hashmap_t* map) {

    map->map = (triple_t*) malloc(sizeof(triple_t));
    map->n = 0;

}


long long hash(pair_t pair) {

    long long x = pair.first;
    long long y = pair.second;

    return (x >= y) ? x*x + x + y : y*y + x;

}


void put(hashmap_t* map, pair_t key, triple_t value) {

    long long position = hash(key);

    if (position >= map->n) {

        // Increase the size to the next power of 2.
        // Essential to get amortized O(1)
        
        long long size = 1ul << ((64 - __builtin_clzll(position)) + 1);

        map->map = realloc(map->map, sizeof(triple_t*) * size);

        for(int i=map->n; i<size; i++) {

            triple_t t = {.first=-1, .second=-1, .third=-1};

            map->map[i] = t;

        }

        map->n = size; 

        if(map->map == NULL) {

            printf("Rehash was not successful, may have run out of memory.\n");
            exit(1);

        }
    
    }

    map->map[position] = value;

}


int get(hashmap_t* map, pair_t key, triple_t* value) {

    long long position = hash(key);

    if(position >= map->n) {

        return 0;

    }

    *value = map->map[position];

    if (value->first == -1 && value->second == -1 && value->third == -1) {

        return 0;

    } else {

        return 1;

    }

}


void free_hashmap(hashmap_t* map) {

    free(map->map);
    map->n = 0;


}

