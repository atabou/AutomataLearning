#ifndef LEARNING_H

    #define LEARNING_H

    #include "dfa.h"

    typedef struct hankel_matrix {

        int* A;
        int  nA;

        /* Non-empty finite prefix-closed set S. */
        string_t* S;
        int       nS;

        /* Non-empty finite suffix-closed set E. */
        string_t* E;
        int       nE;
        
        /* forall s in S, maps (s, a) -> row(s.a) in the table. */
        int**** mapper;

        int** St;
        int** SUSAt;

    } hankel_matrix;


    void make_hankel_matrix(hankel_matrix* H, int* alphabet, int n);

    void free_hankel_matrix(hankel_matrix* H);

#endif
