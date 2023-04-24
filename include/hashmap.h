#ifndef HASHMAP_H

    #define HASHMAP_H

    #include "common.h"

    typedef struct hashmap_t {

        triple_t* map;
        long long n;

    } hashmap_t;


    void make_hashmap(hashmap_t* map);

    void put(hashmap_t* map, pair_t key, triple_t value);

    int get(hashmap_t* map, pair_t key, triple_t* value);

    void free_hashmap(hashmap_t* map);

#endif
