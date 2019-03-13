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
#include <vector>
#include <set>

#include <sdsl/wavelet_trees.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/wt_helper.hpp>

using namespace std;
using namespace sdsl;

typedef struct letternode{
    int c;
    int appears;
} *letterNode;

typedef map<int,letterNode> myMap;

typedef map<int, int> counterMap;

class backwardsSearchBWT{
public:
    wt_int<> L;
    //int_vector<>* LplusOne;
    uint64_t* alphabetCounters;
    bool stop;
    backwardsSearchBWT(const string);
    ~backwardsSearchBWT();
    //int countRange(const int &, const int &);
    int countRange(const int &, const int &, const int &);
    int searchQuery(int*, int, int&, int&);
    int search(int, int, int, int&, int&);
    int backwardError(int* xy, int, set<int>&);
    int fowawrdTraversal(int, int&);
    int backwardTraversal(int index, int&);
    void getRange(int, int&, int&);
    void neighborExpansion(vector<int>, int, int, int, vector<pair<int, int>>&);
    void getConsequents(vector<int> xy, int index, int rangeStart, int rangeEnd, int length, int d, vector<vector<int>>&, int&, sdsl::bit_vector*);
    void getQuickConsequents(int, int, vector<vector<int>>&, int&, sdsl::bit_vector*);
    void getQuickConsequents_noLplus(int, int, vector<vector<int>>&, int&, sdsl::bit_vector*);
    counterMap scan(int, int);
    //int* findRange(int*, int);
	int sigma_seperator;
    int seqNumber;
    //int countRange(int*, int, int = 0, int = 0);
    double sizeInMegabytes();
private:
    void deleteMap();
//    int rank(const char &, const int &);
//    int select(const char &, const int &);
    
    //int subStringOccurences( const string &, const string &);
    myMap alphabet_tmp;

    int finalStartIndex, finalEndIndex, finalStartIndexPermuted, finalEndIndexPermuted;
};
