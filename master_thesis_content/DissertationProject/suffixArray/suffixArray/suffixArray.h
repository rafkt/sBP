//
//  suffixArray.h
//  suffixArray
//
//  Created by Rafael Ktistakis on 22/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#ifndef __suffixArray__suffixArray__
#define __suffixArray__suffixArray__

#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>


using namespace std;
using namespace sdsl;

namespace suffixArray {
    extern vector<int> sa;
    extern vector<int> T;
    
    int compare(const void*, const void*);
    void initialise(ifstream&);
    int getBWT(int);
    void clean_myfile();
    void deleteCreatedFiles();
    wt_int<> BWT2WT();
    void clearALL();
};


#endif /* defined(__suffixArray__suffixArray__) */
