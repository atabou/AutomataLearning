#include "dfa.h"
#include "disjoint_set.h"
#include "queue.h"
#include "hashmap.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG 1


int iselementof(int x, int* arr, int n) {

    for(int i=0; i<n; i++) {

        if(x == arr[i]) {

            return 1;

        }

    }

    return 0;

}


void make_string(string_t* s, int* symbols, int n) {

    if(symbols != NULL && n > 0) {

        s->symbols = (int*) malloc(sizeof(int) * n);

        s->length = n;

        for(int i=0; i<n; i++) {

            s->symbols[i] = symbols[i];

        }

    } else {

        s->symbols = NULL;

        s->length = 0;

    }



}

string_t empty_string() {

    string_t empty = {.symbols=NULL, .length=0};

    return empty;

}

string_t string_concat(string_t s1, string_t s2) {

    string_t s;

    if(s1.length + s2.length > 0) {

        s.symbols = (int*) malloc(sizeof(int) * (s1.length + s2.length));

        s.length  = s1.length + s2.length;

        for(int i=0; i<s1.length; i++) {

            s.symbols[i] = s1.symbols[i];

        }

        for(int i=s1.length; i<s1.length+s2.length; i++) {

            s.symbols[i] = s2.symbols[i];


        }


    } else {


        s.symbols = NULL;

        s.length  = 0;


    }

    return s;

}

void free_string(string_t s) {

    if(s.symbols != NULL) {

        free(s.symbols);

    }

    s.length = 0;

}



int membership(dfa* d, string_t* s) {

    state curr = d->starting;

    for(int i=0; i<s->length; i++) {

        curr = d->transition[curr][s->symbols[i]];

    }

    for(int i=0; i<d->naccept; i++) {

        if(curr == d->accepting[i]) {

            return 1;

        }

    }

    return 0;

}



int equivalent(dfa* d1, dfa* d2, string_t* witness) {

    

    disjoint_set_t dset;
    hashmap_t map;
    queue_t queue;
    
    pair_t   pair;
    triple_t triple;

    int q1_is_accepting;
    int q2_is_accepting;

    if(witness != NULL) {

        witness->symbols = NULL;
        witness->length  = 0;

    }

    /* Initialize the disjoint set on all states */

    make_disjoint_set(&dset, d1->nstate + d2->nstate);
    make_hashmap(&map);
    make_queue(&queue);

    /* Verify if  */

    q1_is_accepting = iselementof(d1->starting, d1->accepting, d1->naccept);
    q2_is_accepting = iselementof(d2->starting, d2->accepting, d2->naccept);

    if ((q1_is_accepting && q2_is_accepting) 
            || (!q1_is_accepting && !q2_is_accepting)) {

        dset_union(&dset, d1->starting, d1->nstate + d2->starting);
        
        pair.first  = d1->starting;
        pair.second = d2->starting;

        enqueue(&queue, pair);
   
    } else {

        

        return 0;

    }

    while (!empty(&queue)) {

        dequeue(&queue, &pair);

        int q1 = pair.first;
        int q2 = pair.second;
        
        for(int sym=0; sym < d1->nsym; sym++) {

            /******************************************************************
             * Get next states given unqueued states and the current symbol.
             *****************************************************************/

            int qn1 = d1->transition[q1][sym]; 
            int qn2 = d2->transition[q2][sym];

            /****************************************************************** 
             * Verify if transition was previously taken by looking at the 
             * successors of qn1 and qn2. 
             *****************************************************************/
            
            int r1 = dset_find(&dset, qn1);
            int r2 = dset_find(&dset, qn2 + d1->nstate);

            if(r1 != r2) {

                dset_union(&dset, r1, r2);

                pair.first  = qn1;
                pair.second = qn2;

                enqueue(&queue, pair);
                
                /**************************************************************
                 * Add (qn1, qn2) -> (q1, q2, sym) to the hashmap for the 
                 * backtracking step if we find that the two DFAs are not 
                 * eqivalent.
                 *************************************************************/

                pair.first  = qn1;
                pair.second = qn2;

                triple.first  = q1;
                triple.second = q2;
                triple.third  = sym;

                put(&map, pair, triple); 

                /**************************************************************
                 * Check if q1 and q2 are both accepting or both non-accepting.
                 *************************************************************/

                q1_is_accepting = iselementof(qn1, d1->accepting, d1->naccept);
                q2_is_accepting = iselementof(qn2, d2->accepting, d2->naccept);

                
                if (!((q1_is_accepting && q2_is_accepting) 
                        || (!q1_is_accepting && !q2_is_accepting))) {

                    /**********************************************************
                     * DFA Non-Equivalence Esatablished. 
                     *
                     * Backtracking step is needed to build counter example.
                     *********************************************************/

                    if(witness != NULL) {

                        pair.first  = qn1;
                        pair.second = qn2;

                        int cap=0;

                        while (!( (pair.first == d1->starting) 
                                    && (pair.second == d2->starting) )) {

                            /******************************************************
                             * Backtracking Step
                             *
                             * Increase the capacity of the counter-example array.
                             * Complexity: O(n)
                             * Amortized Cost: O(1)
                             *****************************************************/

                            if (witness->length == cap) {

                                witness->symbols = 
                                    (cap == 0) 
                                    ? (int*) malloc(sizeof(int)) 
                                    : (int*) realloc(witness->symbols, sizeof(int)*cap*2);


                                assert(witness != NULL);

                                cap = (cap == 0) ? 1 : cap * 2;

                            }

                            // Lookup the values

                            get(&map, pair, &triple);

                            pair.first  = triple.first;
                            pair.second = triple.second;

                            witness->symbols[witness->length] = triple.third;
                            witness->length++;

                        }


                        /**********************************************************
                         * Reverse the Counter-Example Array
                         *
                         * At this point we have generated a counter-example of
                         * size 
                         *********************************************************/

                    }


                    free_disjoint_set(&dset);
                    free_hashmap(&map);
                    free_queue(&queue);

                    return 0;
            
                }
            
            }

        }

    }

    free_disjoint_set(&dset);
    free_hashmap(&map);
    free_queue(&queue);

    return 1;

}



