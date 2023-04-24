
#ifndef DFA_H

    #define DFA_H

    typedef int symbol;

    typedef int state;


    typedef struct string_t {

        int* symbols;
        int  length;

    } string_t;


    void make_string(string_t* s, int* symbols, int n);

    string_t empty_string();

    string_t string_concat(string_t s1, string_t s2);

    void free_string(string_t s);


    typedef struct dfa {

        state* states;
        int nstate;

        symbol* alphabet;
        int nsym;

        int** transition;

        state starting;

        state* accepting;
        int naccept;

    } dfa;


    int membership(dfa* d, string_t* s);


    /**************************************************************************
     * Functions that checks if two DFAs are equivalent.
     *
     * Takes as intput two DFAs, a pointer to array of integers to be 
     * initialized, as well as a pointer to an integer representing the length 
     * of the array.
     *
     * Returns 1 if the two DFAs are equivalent in which case the supplied 
     * pointer pointed to by example will be set to NULL and the value pointed 
     * to by n to 0. 
     *
     * Return 0 if the two DFAs are not equivalent in which case the pointer 
     * pointed to by example will contain a counter-example and the value 
     * pointed to by n the length of the counter-example.
     *
     * The format of the returned counter-example is reversed. That means that 
     * the first symbol is at position n-1 and the last at position 0.
     *
     * !! In the case where the function returns 0 and the counter-example !!
     * !! being the empty string, we choose to represent the empty string  !!
     * !!with a value of NULL and n = 0.                                   !!
     *
     **************************************************************************/
    int equivalent(dfa* d1, dfa* d2, string_t* witness);



#endif
