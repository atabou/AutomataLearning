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

    int array_equality(int* arr1, int n1, int* arr2, int n2);

    int iselementof(int x, int* arr, int n);

#endif
