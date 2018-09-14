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

class subseqPredictor{
public:
    backwardsSearchBWT* bSBWT;
    subseqPredictor(const string, bool, bool);
    ~subseqPredictor();
    int predict(int*, int, int);
	double itemConfidence(int item);
	int datasetSeqNumber();
private:
    int mostFrequentToKeepCounter;
	bool similarSearch, threashold;
};
