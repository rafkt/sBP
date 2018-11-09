//
//  main_permute.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 20/08/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//


#include "similar_queries.h"
#include <vector>

int main()
{
    vector<int> x = {132, 297, 300};
    
    similar_queries* sq = new similar_queries(&x[0], x.size());
    
    int** queries = sq->queryArray;
    
    for (int i = 0; i < sq->getQueryArraySize(); i++) {
        for (int k = 0; k < x.size(); k++) {
            cout << queries[i][k] << " ";
        }
        cout << endl;
    }
    
    int** ranges = new int*[3];
    ranges[0] = new int[2];
    ranges[1] = new int[2];
    ranges[2] = new int[2];
    
    ranges[0][0] = 20;
    ranges[0][1] = 25;
    ranges[1][0] = 35;
    ranges[1][1] = 40;
    ranges[2][0] = 36;
    ranges[2][1] = 60;
    
    for (int i = 0; i < 3; i++) {
        cout << ranges[i][0] << " " << ranges[i][1] << endl;
    }
    
    
    sq->rangesUnion(ranges, 3);
    
    for (int i = 0; i < 3; i++) {
        cout << ranges[i][0] << " " << ranges[i][1] << endl;
    }
    delete sq;
    for (int i = 0; i < 3; i ++) {
        delete [] ranges[i];
    }
    delete [] ranges;
    x.clear();
    return 0;
}
