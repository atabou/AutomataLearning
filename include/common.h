#ifndef COMMON_H

    #define COMMON_H

    typedef struct pair_t {

        int first;
        int second;

    } pair_t;


    typedef struct triple_t {

        int first;
        int second;
        int third;

    } triple_t;


    int array_equality(int* arr1, int n1, int* arr2, int n2) {

        if(n1 != n2) {

            return 0;
        
        }

        for(int i=0; i<n1; i++) {

            if(arr1[i] != arr2[i]) {

                return 0;

            }

        }

        return 1;

    }

#endif
