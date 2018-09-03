//
//  similar_queries.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 20/08/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "similar_queries.h"


similar_queries::similar_queries(int* q, int s){
    current_size = 0;
    size = factorial(s) / 2;
    queryArray = new int*[size];
    for (int i = 0; i < size; i++) queryArray[i] = new int[s];
    permute(q, 0, s - 1);
}

similar_queries::~similar_queries(){
    for (int i = 0; i < size; i++) {
        delete [] queryArray[i];
    }
    delete [] queryArray;
}

void similar_queries::swap (int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Function to print permutations of string
 This function takes three parameters:
 1. String
 2. Starting index of the string
 3. Ending index of the string. */
void similar_queries::permute(int *a, int i, int n){
    int j;
    if (i == n - 1){
        for (int i = 0; i < n + 1; i++) queryArray[current_size][i] = a[i];
        current_size++;
    }
    else
    {
        for (j = i; j <= n; j++)
        {
            swap((a+i), (a+j));
            permute(a, i+1, n);
            swap((a+i), (a+j)); //backtrack
        }
    }
}

int similar_queries::getQueryArraySize(){
    return size;
}

int similar_queries::factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

void similar_queries::rangesUnion(int** ranges, int n){
    bool swapped = true;
    int j = 0;
    int tmp0, tmp1;
    while (swapped) {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++) {
            if (ranges[i][0] > ranges[i + 1][0]) {
                tmp0 = ranges[i][0];
                tmp1 = ranges[i][1];
                ranges[i][0] = ranges[i + 1][0];
                ranges[i][1] = ranges[i + 1][1];
                ranges[i + 1][0] = tmp0;
                ranges[i + 1][1] = tmp1;
                swapped = true;
            }
        }
    }
    int* top;
    top = ranges[0];
    for (int i = 1; i < n; i++) {
        if (ranges[i][0] <= top[1]) {
            if (ranges[i][1] > top[1]) top[1] = ranges[i][1];
            ranges[i][0] = -1;
            ranges[i][1] = -1;
        }else top = ranges[i];
    }
}













