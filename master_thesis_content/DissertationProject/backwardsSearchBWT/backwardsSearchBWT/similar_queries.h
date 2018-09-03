//
//  similar_queries.h
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 20/08/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class similar_queries{
public:
    similar_queries(int* q, int s);
    ~similar_queries();
    int** queryArray;
    int getQueryArraySize();
    void rangesUnion(int**, int);
private:
    void permute(int *, int , int );
    void swap (int *, int *);
    int factorial(int);
    int size;
    int current_size;
};