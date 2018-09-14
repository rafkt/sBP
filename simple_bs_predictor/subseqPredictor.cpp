//
//  predictor.cpp
//  countBefore
//
//  Created by Rafael Ktistakis on 23/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "subseqPredictor.h"
//#include "similar_queries.h"
#include <fstream>
#include <sys/time.h>
#include <time.h>

subseqPredictor::subseqPredictor(const string filename, bool similarSearch, bool threashold): mostFrequentToKeepCounter(0), similarSearch(similarSearch), threashold(threashold){
    bSBWT = new backwardsSearchBWT(filename, similarSearch);
}

subseqPredictor::~subseqPredictor(){
    delete bSBWT;
}

int compare(const void* a, const void* b){
    return ( *(int*)a - *(int*)b );
}

double subseqPredictor::itemConfidence(int item){
	int frequency = bSBWT->L.rank(bSBWT->L.size() - 1, item);
	//cout << "Freq " << frequency << " Sequences " << bSBWT->seqNumber << endl;
	return ((double) frequency / bSBWT->seqNumber);
}

int subseqPredictor::datasetSeqNumber(){
	return bSBWT->seqNumber;
}

int subseqPredictor::predict(int* query, int size, int maxPredictionCount){
	//contract a bit-vector of size L (from BWT)
	//generate subqueries
	//for every subquery that I have generated

		//find all the ranges with neighborExpansion
		//for every range that I have found
			//get the consequent of length 2; this can be something like L[x], L[y]
			//set the bits on positions x & y. If they are already set then reject this consequent and continue with another range
			//put consequent to the countTable; I also need the parameters; current number of ? the current subquery has, size, and subquery length
			//consequent can be less than 2 if for example the sequence ended. So, I have to check whether a terminating character (99999) appears in the consequent length
			//if consequent length > 0 then increase the prediction count
		//if prediction count > maxPredictioncount then get the most frequent item from the countTable; I have to also set a flag for permanent stop
	//return he most frequent item from the countTable
    return 1;
}

//I need to provide functionality for the countTable. So I need to provide the C++ structure. a multiMap<item, count> seems suitable. Also I 
// need a put method that will assign the weight for the consequent items when I am inserting them

//I need a method that generates all the subqueries. I think that at least for now it will be better organised, this way.

void subseqPredictor::start(int* query, size){ // this function will manage deletion of the first items.
	//for (k = 0; k < size - 2; k++)
		//create a copy of the query start at [k, size - k]
		//generateSubqueries(copy of query above, new size)	
}

void subseqPredictor::generateSubqueries(int* query, size){
	for (int i = 0; i < _size; i++){
        //cout << i << endl;
        int old_item = y[i];
        y[i] = -2;
        for (int item : y) cout << item << " ";
        cout << endl;
        y[i] = old_item;
        //predict; if permanent stop abbort and return answer
    }

    if (_size > 2){
        for (int i = 0; i < _size; i++){
            for (int j = i + 1; j < _size; j++){
                //cout << i << j << endl;
                int old_item_1 = y[i];
                int old_item_2 = y[j];
                y[i] = -2;
                y[j] = -2;
                for (int item : y) cout << item << " ";
                cout << endl;
                y[i] = old_item_1;
                y[j] = old_item_2;
            }
        }
        //predict; if permanent stop abbort and return answer
    }
}