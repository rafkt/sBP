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
#include <set>

#define MAXPREDICTIONCOUNT 0

float subseqPredictor::get_memory(){
	return bSBWT->sizeInMegabytes();
}

subseqPredictor::subseqPredictor(const string filename){
    bSBWT = new backwardsSearchBWT(filename);
}

subseqPredictor::~subseqPredictor(){
    delete bSBWT;
}

int compare(const void* a, const void* b){
    return ( *(int*)a - *(int*)b );
}

double subseqPredictor::itemConfidence(int item){
	//int frequency = bSBWT->L.rank(bSBWT->L.size() - 1, item);
	int frequency;
	if (item == 0){
		frequency = bSBWT->alphabetCounters[0];
	}else{
		frequency = bSBWT->alphabetCounters[item] - bSBWT->alphabetCounters[item - 1];
	}
	//cout << "Freq " << frequency << " Sequences " << bSBWT->seqNumber << endl;
	return ((double) frequency / bSBWT->seqNumber);
}

int subseqPredictor::datasetSeqNumber(){
	return bSBWT->seqNumber;
}

void subseqPredictor::predict(vector<int> query, int size, int maxPredictionCount, int initialLength, int errors){
	// this will be called for different subqueries - and will return a prediction
			// for (uint64_t i = 0; i < bitstring->size(); i++) cout << (*bitstring)[i];
			// cout << endl;

	//constract a bit-vector of size L (from BWT)

	//find all the ranges with neighborExpansion

	int rangeStart = - 1, rangeEnd = -1;
	vector<pair<int, int>> bs_ranges;
	vector<vector<int>> consequentList;
	vector<int> consequentList_corresponding_errors;


	    	bSBWT->getRange(query[0], rangeStart, rangeEnd);
	    	if (rangeStart != -1 && rangeEnd != -1){
	    		bSBWT->treeExpansion(query, 1, errors, errors, rangeStart, rangeEnd, bs_ranges, consequentList, consequentList_corresponding_errors, predictionCount, consequentBits, MAXPREDICTIONCOUNT);
	    	}

	    //cout << ": Ranges : " << bs_ranges.size() << endl;
	    // for (pair<int, int> it : bs_ranges){
	    // 	vector<int> tmp;
	    // 	//bSBWT->getConsequents(tmp, 0, it.first, it.second, 2, -1, consequentList, predictionCount, consequentBits);
	    // 	bSBWT->getQuickConsequents_noLplus(it.first, it.second, consequentList, predictionCount, consequentBits);
	    // }
	    //put all ranges into CT
	    for (int i = 0; i < consequentList.size(); i++){

	    	push(consequentList[i], consequentList_corresponding_errors[i], initialLength, size);
	    }
	    
	    if (predictionCount >= MAXPREDICTIONCOUNT) {
	    	stop = true;
	    	return;
		}
	

	//for every range that I have found
		//get the consequent of length 2; this can be something like L[x], L[y]
		//set the bits on positions x & y. If they are already set then reject this consequent and continue with another range
		//put consequent to the countTable; I also need the parameters; current number of ? the current subquery has, size, and subquery length
		//consequent can be less than 2 if for example the sequence ended. So, I have to check whether a terminating character (99999) appears in the consequent length
		//if consequent length > 0 then increase the prediction count
	//if prediction count > maxPredictioncount then get the most frequent item from the countTable; I have to also set a flag for permanent stop
	//return he most frequent item from the countTable
    return;
}

//I need to provide functionality for the countTable. So I need to provide the C++ structure. a multiMap<item, count> seems suitable. Also I 
// need a put method that will assign the weight for the consequent items when I am inserting them

//I need a method that generates all the subqueries. I think that at least for now it will be better organised, this way.
void subseqPredictor::push(vector<int> consequent, int errors, int initialLength, int subLength){
	//cout << "About to push: " << endl;
	//for (int it : consequent) cout << it << " ";
	//cout << endl;
	pair<std::map<int, double>::iterator, bool> ret;
	double current_score = 0.0;
	for (int i = 0; i < consequent.size(); i++){
		double weightDistance = 1.0 / (i + 1);
		double newWeight = ((subLength / (double)initialLength)) + ((2 - errors) / (double)2) + (1.0) + (weightDistance);
		//cout << (subLength / (double)initialLength) << " " << ((2 - errors) / (double)2) << " " << ((double) weightDistance) << endl;
		ret = countTable.insert(std::pair<int, double>(consequent[i], newWeight));
	  	if (ret.second == false) {
	  		//item already in the countTable
	  		//cout << "Previous score: " << ret.first->second << endl;
	  		current_score = ret.first->second * newWeight;
		    ret.first->second =  current_score;
		    //cout << "Pushed: " << consequent[i] << " with score: " << current_score << endl;
		    if (current_score > score){
		    	score = current_score;
		    	prediction = consequent[i];
		    }
		}else{
			//cout << "Pushed: " << consequent[i] << " with score: " << newWeight << endl;
			if (newWeight > score){
		    	score = newWeight;
		    	prediction = consequent[i];
		    }
		}
	}
}

int subseqPredictor::getBest(){
	return prediction;
}

int subseqPredictor::start(vector<int> query, int size){ // this function will manage deletion of the first items.
	consequentBits = new bit_vector(bSBWT->L.size(), 0); // I have to delete this at the end of this function or before returning
	cashed_ranges.clear();
	stop = false;
	prediction = -1; score = -1.0;
	predictionCount = 0;
	countTable.clear();
	if (size < 2) return getBest();
	for (int k = 0; k < size - 1; k++) {
		predictionCount = 0;
		generateSubqueries(vector<int>(query.begin()+k, query.end()), size - k); 
		if (stop) {delete consequentBits; return getBest();}
	}
	delete consequentBits;
	return getBest();
}

void subseqPredictor::generateSubqueries(vector<int> query, int size){
	int initialLength = size;
	// for (int i = 0; i < size; i++){
 //        //cout << i << endl;
 //        int old_item = query[i];
 //        query[i] = -2;
 //        // for (int item : y) cout << item << " ";
 //        // cout << endl;
 //        //for (int q = 0; q < size; q++) cout << query[q] << " ";
 //        //cout << endl;
 //        predict(query, size, MAXPREDICTIONCOUNT, initialLength, 1);
 //        //predict; if permanent stop abbort and return answer
 //        query[i] = old_item;
 //        //if (stop) return;
 //    }
 //    if (stop) return;
 //    if (size > 2){
 //        for (int i = 0; i < size; i++){
 //            for (int j = i + 1; j < size; j++){
 //                //cout << i << j << endl;
 //                int old_item_1 = query[i];
 //                int old_item_2 = query[j];
 //                query[i] = -2;
 //                query[j] = -2;
 //                // for (int item : y) cout << item << " ";
 //                // cout << endl;
 //                //for (int q = 0; q < size; q++) cout << query[q] << " ";
 //        		//cout << endl;
 //                predict(query, size, MAXPREDICTIONCOUNT, initialLength, 2);
 //                //predict; if permanent stop abbort and return answer

 //                query[i] = old_item_1;
 //                query[j] = old_item_2;
 //                //if (stop) return;
 //            }
 //        }
 //    }
		// predict(query, size, MAXPREDICTIONCOUNT, initialLength, 1);
		// if (stop) return;
		predict(query, size, MAXPREDICTIONCOUNT, initialLength, 2);
}
