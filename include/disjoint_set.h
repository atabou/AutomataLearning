
#ifndef DISJOINT_SET_H

    #define DISJOINT_SET_H

    typedef struct disjoint_set_t {

        int* parent; // parent[i] = j -> parent of i is j
        int* rank;
        int capacity;

    } disjoint_set_t;


    void make_disjoint_set(disjoint_set_t* dset, int capacity);
    
    int  dset_find(disjoint_set_t* dset, int x);
    void dset_union(disjoint_set_t* dset, int x, int y);
    
    void free_disjoint_set(disjoint_set_t* dset);

#endif
