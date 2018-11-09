//
//  main.cpp
//  suffixArray
//
//  Created by Rafael Ktistakis on 22/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "suffixArray.h"

using namespace suffixArray;



int main ()
{
    
    ifstream file("input.txt");
    initialise(file);
    
    
    wt_int<> wt = BWT2WT();
    for (size_t i = 0; i < sa.size(); i++){
        cout << getBWT(i) << " ";
    }
    cout << endl;
    
    clearALL();
    
    
    
//    qsort (&myVector[0], myVector.size(), sizeof(int), compare);
//    for (int i = 0; i < myVector.size(); i++)
//        printf ("%d ",myVector[i]);
//    cout << endl;
    return 0;

}