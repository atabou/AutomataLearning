#include "disjoint_set.h"

#include <stdlib.h>


void make_disjoint_set(disjoint_set_t* dset, int capacity) {

    dset->parent = (int*) malloc(sizeof(int) * capacity);
    dset->rank = (int*) malloc(sizeof(int) * capacity);
    
    for(int i=0; i<capacity; i++) {

        dset->parent[i] = i;
        dset->rank[i] = 0;

    }

    dset->capacity = capacity;

}


int dset_find(disjoint_set_t* dset, int x) {

    int root = x;

    while (root != dset->parent[root]) {

        root = dset->parent[root];

    }

    return root;

}

void dset_union(disjoint_set_t* dset, int x, int y) {

    int xr = dset_find(dset, x);
    int yr = dset_find(dset, y);

    if(xr == yr) {

        return;
    
    } 
    
    if (dset->rank[xr] == dset->rank[yr]) {

        dset->parent[xr] = yr;
        dset->rank[yr] = yr + 1;

        return;

    } else if (dset->rank[xr] < dset->rank[yr]) {

        dset->parent[xr] = yr; 

        return;

    } else {

        dset->parent[yr] = xr;

        return;

    }

}

void free_disjoint_set(disjoint_set_t* dset) {

    if(dset->parent != NULL) {
        
        free(dset->parent);
        dset->parent = NULL;
    
    }

    if(dset->rank) {

        free(dset->rank);
        dset->rank = NULL;

    }

    dset->capacity = 0;

}
