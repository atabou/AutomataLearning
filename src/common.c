#include "common.h"

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


int iselementof(int x, int* arr, int n) {

    for(int i=0; i<n; i++) {

        if(x == arr[i]) {

            return 1;

        }

    }

    return 0;

}


