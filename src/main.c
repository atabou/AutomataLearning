#include "dfa.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void test_membership(dfa d, string_t string, int expected) {

    int ismember = membership(&d, &string);

    printf("String: ");

    for(int i=0; i<string.length; i++) {

        printf("%d", string.symbols[i]);

    }

    printf("\n\tEXPECTED: %d, ACTUAL: %d\n\t[SUCCESS]\n\n", expected, ismember);

    assert(ismember == expected);

}


int main() {

    state states[2] = {0, 1};
    symbol alphabet[2] = {0, 1};
    int* transition[2] = {};

    transition[0] = (int*) malloc(sizeof(int) * 2);
    transition[1] = (int*) malloc(sizeof(int) * 2);

    transition[0][0] = 1;
    transition[0][1] = 0;
    transition[1][0] = 0;
    transition[1][1] = 1;

    state starting = 0;
    state accepting[1] = {0};

    dfa d1 = {
        .states=states, 
        .nstate=2, 
        .alphabet=alphabet, 
        .nsym=2, 
        .transition=transition,
        .accepting=accepting,
        .naccept=1,
        .starting=starting 
    };


    // TESTS
    
    int arr[80];
    string_t string;

    string.symbols = arr;

    string.length = 3;
    string.symbols[0] = 0;
    string.symbols[1] = 1;
    string.symbols[2] = 0;

    test_membership(d1, string, 1);

    string.length = 3;
    string.symbols[0] = 1;
    string.symbols[1] = 1;
    string.symbols[2] = 1;

    test_membership(d1, string, 1);

    string.length = 3;
    string.symbols[0] = 0;
    string.symbols[1] = 0;
    string.symbols[2] = 0;

    test_membership(d1, string, 0);

    string.length = 3;
    string.symbols[0] = 1;
    string.symbols[1] = 0;
    string.symbols[2] = 1;

    test_membership(d1, string, 0);

    state eqstates[3] = {0, 1, 2};
    int* eqtransition[3];

    eqtransition[0] = (int*) malloc(sizeof(int) * 2);
    eqtransition[1] = (int*) malloc(sizeof(int) * 2);
    eqtransition[2] = (int*) malloc(sizeof(int) * 2);

    eqtransition[0][0] = 1;
    eqtransition[0][1] = 0;
    eqtransition[1][0] = 0;
    eqtransition[1][1] = 1;
    eqtransition[2][0] = 0;
    eqtransition[2][1] = 1;

    state eqstarting = 2;
    state eqaccepting[2] = {0, 2};

    dfa d2 = {
        .states=eqstates, 
        .nstate=3, 
        .alphabet=alphabet, 
        .nsym=2, 
        .transition=eqtransition,
        .accepting=eqaccepting,
        .naccept=2,
        .starting=eqstarting 
    };

    int eq = equivalent(&d1, &d2, &string);

    printf("equivalence: %d\n", eq);

    if(!eq) {


        printf("counter-example: ");
        
        for(int i=string.length-1; i>=0; i--) {

            printf("%d\n", string.symbols[i]);

        }

        printf("\n");

        free_string(string);

    }

    free(transition[0]);
    free(transition[1]);

    return 0;

}

