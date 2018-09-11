//
//  countBefore.h
//  countBefore
//
//  Created by Rafael Ktistakis on 20/06/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <map>

#include <sdsl/wavelet_trees.hpp>

using namespace std;
using namespace sdsl;

typedef struct letternode{
    int c;
    int appears;
} *letterNode;

typedef map<int,letterNode> myMap;

typedef multimap<float, int> counterMap;

class backwardsSearchBWT{
public:
    wt_int<> L, alphabet;
    int_vector<> alphabetCounters;
    bool stop;
    backwardsSearchBWT(const string, bool);
    ~backwardsSearchBWT();
    int countRange(const int &, const int &);
    int countRange(const int &, const int &, const int &);
    int search(int c, int, int );
    int fowawrdTraversal(int);
    int backwardTraversal(int index, int&);
    counterMap scan(int, int);
    int* findRange(int*, int);
	int seqNumber;
private:
    void deleteMap();
//    int rank(const char &, const int &);
//    int select(const char &, const int &);
    int countRange(int*, int, int = 0, int = 0);
    void countRangeLastTwoLettersPermuted(int, int, int*);
    int subStringOccurences( const string &, const string &);
    myMap alphabet_tmp;
    int finalStartIndex, finalEndIndex, finalStartIndexPermuted, finalEndIndexPermuted;
	bool similar;
};
