#include "learning.h"

#include "common.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void make_hankel_matrix(hankel_matrix* H) {

    /**************************************************************************
     * Initialize S and E to the sets that contain only the empty string.
     *************************************************************************/

    H->S = (string_t*) malloc(sizeof(string_t));
    H->E = (string_t*) malloc(sizeof(string_t));

    H->S[0] = empty_string();
    H->E[0] = empty_string();

    H->nS = 1;
    H->nE = 1;

    /**************************************************************************
     * Initialize the S and S U S.A tables.
     *************************************************************************/

    H->St = (int**) malloc(sizeof(int*) * H->nS);
    H->St[0] = (int*) malloc(sizeof(int) * H->nE);

    H->SUSAt = (int**) malloc(sizeof(int*) * H->nS * H->nA);
    
    for(int sa=0; sa<H->nS*H->nA; sa++) {

        H->SUSAt[sa] = (int*) malloc(sizeof(int) * H->nE);

    }

    /**************************************************************************
     * Initialize the S->A->S.A row mapper.
     *************************************************************************/

    H->mapper = (int****) malloc(sizeof(int***) * H->nS);
    H->mapper[0] = (int***) malloc(sizeof(int**) * H->nA);

    H->mapper[0][0] = H->SUSAt;
    H->mapper[0][1] = H->SUSAt + 1;

}


typedef struct inconsistency_t {

    int s1;
    int s2;
    int a;
    int e;

} inconsistency_t;


/**************************************************************************
 * Verifies that:
 * 
 * forall s1,s2 in S, row(s1)=row(s2) => forall a in A, row(s1.a)=row(s2.a)
 *
 * Complexity: O(A*E*S^2)
 *
 * Possible Optimization: 
 *      - Only check the newly added rows.
 *      - This would yield an amortized O(A*E*S)
 *************************************************************************/
int hankel_matrix_isconsistent(hankel_matrix* H, inconsistency_t* icstcy) {


    for (int s1=0; s1 < H->nS - 1; s1++) { // O(|S|)

        for (int s2=s1+1; s2 < H->nS; s2++) { // O(|S|)

            if (array_equality(H->St[s1], H->nE, H->St[s2], H->nE)) { // O(|E|)

                for (int a=0; a<H->nA; a++) {

                    int* s1a = *(H->mapper[s1][a]);
                    int* s2a = *(H->mapper[s2][a]);

                    for(int e=0; e < H->nE; e++) {

                        if(s1a[e] != s2a[e]) {

                            icstcy->s1 = s1;
                            icstcy->s2 = s2;
                            icstcy->a = a;
                            icstcy->e = e;

                            return 0;

                        }

                    }

                }

            }

        }

    }

    return 1;


}


typedef struct unclosed_t {

    int s;
    int a;

    int mappedby;

} unclosed_t;

/**************************************************************************
 * Verifies that:
 *      
 * forall t in S U A, there exists s in S such that row(t)=row(s) 
 *************************************************************************/
int hankel_matrix_isclosed(hankel_matrix* H, unclosed_t* unclosed) {

    for(int t=0; t<H->nS * H->nA; t++) {

        int exists = 0;

        for(int s=0; s<H->nS; s++) {

            exists = exists 
                || array_equality(H->SUSAt[t], H->nE, H->St[s], H->nE);

        }

        if(!exists && unclosed != NULL) {

            /******************************************************************
             * Get the prefix that maps to t.
             *****************************************************************/

            for(int s=0; s<H->nS; s++) {

                for(int a=0; a<H->nA; a++) {

                    if(*(H->mapper[s][a]) == H->SUSAt[t]) {

                        unclosed->s = s;
                        unclosed->a = a;

                        return 0;

                    }
                    
                }

            }

            printf("[ERROR] Hankel Matrix is not correctly built.\n");
            printf("[ERROR] Found element s.a in S U S.A such that s not in S.\n");

            assert(0);

        }

    }

    return 1;

}


void hankel_matrix_conformize(hankel_matrix* H, dfa* d) {

    inconsistency_t inconsistency;
    unclosed_t unclosed;

    int consistant = 0;
    int closed     = 0;

    while (!consistant || !closed) {

        consistant = hankel_matrix_isconsistent(H, &inconsistency);

        if (!consistant) {

            int s1 = inconsistency.s1;
            int s2 = inconsistency.s2;
            int a  = inconsistency.a; 
            int e  = inconsistency.e;

            /* Create string ae from the inconsistency */
            
            int tmp[1] = { a };

            string_t a_s = {.symbols=tmp, .length=1};

            string_t ae_s = string_concat(a_s, H->E[e]);

            /*****************************************************************
             * Insert a.e in E 
             *****************************************************************/

            H->E = (string_t*) realloc(H->E, sizeof(string_t) * (H->nE + 1));

            assert(H->E != NULL);

            int ae = H->nE;
            
            H->nE = H->nE + 1;

            H->E[ae] = ae_s;


            /******************************************************************
             * Insert ae in table. 
             ******************************************************************/

            for (int s=0; s<H->nS; s++) {
 
                /**************************************************************
                 * Make positions (s, a.e) = (s.a, e)
                 *
                 * We know (s.a, e) exists since s.a in S U S.a and e in E.
                 *************************************************************/
                
                H->St[s] = (int*) realloc(H->St[s], sizeof(int) * H->nE);

                assert(H->St[s] != NULL);

                int sae = (*(H->mapper[s][a]))[e];

                H->St[s][ae] = sae;
                
                /**************************************************************
                 * Update the SUSA table with membership queries
                 *************************************************************/

                for (int sym=0; sym<H->nA; sym++) {


                    /**********************************************************
                     * Get the  address of the s.sym row in the SUSA table from
                     * the mapper.
                     *********************************************************/

                    int** ssym_row = H->mapper[s][sym];

                    /**********************************************************
                     * Add column to S U S.A table.
                     *********************************************************/
                    
                    *ssym_row = (int*) realloc(*ssym_row, sizeof(int) * H->nE);

                    assert(*ssym_row != NULL);

                    /**********************************************************
                     * Do membership queries on all s.a1 in S.a1 for (s.a1).a.e
                     *********************************************************/
                    
                    string_t sa_s = string_concat(H->S[s], ae_s);

                    (*ssym_row)[ae] = membership(d, &sa_s);

                    free_string(sa_s);    

                } 
            }            
        }

        closed = hankel_matrix_isclosed(H, &unclosed);

        if(!closed) {
            
            int s  = unclosed.s;
            int a  = unclosed.a;

            /****************************************************************** 
             * Create string s.a from the unclosed row. 
             *****************************************************************/

            int tmp[1] = { a };

            string_t a_s = {.symbols=tmp, .length=1};

            string_t sa_s = string_concat(H->S[s], a_s);

            /****************************************************************** 
             * Add s.a to S 
             *****************************************************************/

            H->S = (string_t*) realloc(H->S, sizeof(string_t) * (H->nS + 1));

            assert(H->S != NULL);

            int sa = H->nS;

            H->nS = H->nS + 1;

            H->S[sa] = sa_s;

            /******************************************************************
             * Add s.a row to the S table.
             *****************************************************************/

            H->St = (int**) realloc(H->St, sizeof(int*) * H->nS);

            assert(H->St != NULL);

            H->St[sa] = *(H->mapper[s][a]);

            /****************************************************************** 
             * Remove the s.a row from the S U S.A table. 
             *
             * The s.a row is now exlusively part of the S table.
             *****************************************************************/

             *(H->mapper[s][a]) = NULL;

            // TODO: Don't forget to add the position that was nulled in a stack for later reuse.
            
            /*****************************************************************
             * Update the mapping table to add (s, a) -> new s.a row location. 
             *****************************************************************/

            H->mapper[s][a] = H->St + sa;
            
            /******************************************************************
             * Add rows to S U S.A table for the new added s in S.
             * Map those new rows to the mapper.
             *****************************************************************/
            
            H->SUSAt = (int**) realloc(H->SUSAt, sizeof(int*) * (H->nS*H->nA));
            
            H->mapper = (int****) realloc(H->mapper, sizeof(int***) * H->nS);
            
            H->mapper[sa] = (int***) malloc(sizeof(int***) * H->nA);

            for (int sym; sym < H->nA; sym++) {

                /**************************************************************
                 * Allocate memory for S U S.A strings that were added.
                 *************************************************************/

                H->SUSAt[sa * H->nA + sym] = malloc(sizeof(int)*H->nE);

                /**************************************************************
                 * Fill the added memory with membership queries.
                 *************************************************************/

                int sym_arr[1] = {sym};

                string_t sym_s = {.symbols=sym_arr, .length=1}; 

                string_t sasym = string_concat(sa_s, sym_s);

                for(int e=0; e<H->nE; e++) {

                    H->SUSAt[sa * H->nA + sym][e] = membership(d, &sasym);

                }

                free_string(sasym);

                /**************************************************************
                 * Add mapping for the newly added rows.
                 *************************************************************/

                H->mapper[sa][sym] = H->SUSAt + (sa*H->nA + sym); 

            }

        }

    }

}

void convert_hankel_to_dfa(hankel_matrix* H, DFA* M);


void learn_automata(dfa teacher) {

    hankel_matrix H;

    make_hankel_matrix(&H);

    do {

        dfa M;

        convert_hankel_to_dfa(H, M);

        string_t witness;

        if(equivalent(&M, &teacher, &(witness.symbols), &(witness.length))) {

                        


        }


    

    } while();


    free_hankel_matrix(&H);


}


void free_hankel_matrix(hankel_matrix* H);

