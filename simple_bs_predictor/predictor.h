//
//  predictor.h
//  countBefore
//
//  Created by Rafael Ktistakis on 23/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//


#include <iostream>
#include <vector>
#include "backwardsSearchBWT.h"
#include <map>

using namespace std;

typedef multimap<float, int> aMap;

class predictor{
public:
    backwardsSearchBWT* bSBWT;
    predictor(const string, bool, bool);
    ~predictor();
    aMap predict(int*, int, int);
	aMap predictWithRecursiveDivider(int*, int, int, int, int);
	double itemConfidence(int item);
	int datasetSeqNumber();
private:
    int mostFrequentToKeepCounter;
	bool similarSearch, threashold;
	void RecursiveDivider(int*, int, int, aMap&, int, int);
};
