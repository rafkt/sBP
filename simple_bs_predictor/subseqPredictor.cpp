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

#define MAXPREDICTIONCOUNT 10

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
	int frequency = bSBWT->L.rank(bSBWT->L.size() - 1, item);
	//cout << "Freq " << frequency << " Sequences " << bSBWT->seqNumber << endl;
	return ((double) frequency / bSBWT->seqNumber);
}

int subseqPredictor::datasetSeqNumber(){
	return bSBWT->seqNumber;
}

void subseqPredictor::predict(int* query, int size, int maxPredictionCount, int initialLength, int errors){
	// this will be called for different subqueries - and will return a prediction
			// for (uint64_t i = 0; i < bitstring->size(); i++) cout << (*bitstring)[i];
			// cout << endl;

	//constract a bit-vector of size L (from BWT)

	//find all the ranges with neighborExpansion

	

	int rangeStart = - 1, rangeEnd = -1;
	set<int> firstItemPossibleReplacements, secondItemPossibleReplacements;
	vector<pair<int, int>> bs_ranges;
	vector<pair<int, int>> exclude_bs_ranges;
	if (query[0] == -2){
		
		vector<int> exclude_query_vector(size);
		copy(query, query + size, exclude_query_vector.begin());

		if (errors > 1 && query[1] == -2){
		// 	bSBWT->backwardError(query, size, secondItemPossibleReplacements);
		// 	for (int item : secondItemPossibleReplacements){
		// 		//cout << item << endl;
		// 		query[1] = item;
		// 		bSBWT->backwardError(query, size, firstItemPossibleReplacements);
		// 		for (int Fitem : firstItemPossibleReplacements){
		// 			//cout << Fitem << endl;
		// 			query[0] = Fitem;

		// 			// cout << ".Current query: ";
		// 			// for (int i = 0; i < size; i++){
		// 			// 	cout << query[i] << " ";
		// 			// }
		// 			// cout << endl;

					exclude_query_vector[1] = 99999;
					if (bSBWT->searchQuery(&(exclude_query_vector[1]), size - 1, rangeStart, rangeEnd) != -1){
			    		exclude_bs_ranges.push_back(make_pair(rangeStart, rangeEnd));
			    	}

					
					exclude_query_vector[0] = 99999;
					exclude_query_vector[1] = -2;
					bSBWT->getRange(exclude_query_vector[0], rangeStart, rangeEnd);
					bSBWT->neighborExpansion(exclude_query_vector, 1, rangeStart, rangeEnd, exclude_bs_ranges);



					int finalStartRange, finalEndRange;
			    	if (bSBWT->searchQuery(&(query[2]), size - 2, finalStartRange, finalEndRange) != -1){
			    		bs_ranges.push_back(make_pair(finalStartRange, finalEndRange));
			    	}
		// 		}
		// 	}
		// 	firstItemPossibleReplacements.clear(); secondItemPossibleReplacements.clear();
		}else{ //bSBWT->backwardError(query, size, firstItemPossibleReplacements);
		//for (int item : firstItemPossibleReplacements){
			//cout << item << endl;
			// query[0] = item;

			exclude_query_vector[0] = 99999;

			//repeated code here - should improve it
			vector<int> query_vector(size);
			copy(&(query[1]), query + size, query_vector.begin());
		    if (errors > 1){

		    	bSBWT->getRange(exclude_query_vector[0], rangeStart, rangeEnd);
				bSBWT->neighborExpansion(exclude_query_vector, 1, rangeStart, rangeEnd, exclude_bs_ranges);
				

		    	bSBWT->getRange(query[1], rangeStart, rangeEnd);
		    	bSBWT->neighborExpansion(query_vector, 1, rangeStart, rangeEnd, bs_ranges);
		    }else{
				if (bSBWT->searchQuery(&(exclude_query_vector[0]), size, rangeStart, rangeEnd) != -1){
		    		exclude_bs_ranges.push_back(make_pair(rangeStart, rangeEnd));
		    	}

		  //   	cout << "..Current query: ";
				// for (int i = 0; i < size; i++){
				// 	cout << query[i] << " ";
				// }
				// cout << endl;

		    	//should do a search; if search returns anything then I should add it in the bs_ranges
		    	int finalStartRange, finalEndRange;
		    	if (bSBWT->searchQuery(&(query[1]), size - 1, finalStartRange, finalEndRange) != -1){
		    		bs_ranges.push_back(make_pair(finalStartRange, finalEndRange));
		    	}
		    }
			//end of repeated code
		}

		//excluding ranges
		for (pair<int, int> range : exclude_bs_ranges)
			for (int r = range.first; r <= range.second; r++) (*consequentBits)[r] = 1;
		//end of excluding ranges


		vector<vector<int>> consequentList;
	    //cout << "Ranges: " << bs_ranges.size() << endl;
	    for (pair<int, int> it : bs_ranges){
	    	vector<int> tmp;
	    	bSBWT->getConsequents(tmp, 0, it.first, it.second, 2, -1, consequentList, predictionCount, consequentBits);
	    }
	    //put all ranges into CT
	    for (vector<int> consequent : consequentList) push(consequent, errors, initialLength, size);
	    
	    if (predictionCount > MAXPREDICTIONCOUNT) {
	    	stop = true;
	    	return;
		}
	}else{
		vector<int> query_vector(size);
		copy(query, query + size, query_vector.begin());
	    if (errors > 0){
	    	bSBWT->getRange(query[0], rangeStart, rangeEnd);
	    	if (rangeStart != -1 && rangeEnd != -1) bSBWT->neighborExpansion(query_vector, 1, rangeStart, rangeEnd, bs_ranges);
	    }else{

	  //   	cout << "...Current query: ";
			// for (int i = 0; i < size; i++){
			// 	cout << query[i] << " ";
			// }
			// cout << endl;


	    	//should do a search; if search returns anything then I should add it in the bs_ranges
	    	int finalStartRange, finalEndRange;
	    	if (bSBWT->searchQuery(query, size, finalStartRange, finalEndRange) != -1){
	    		bs_ranges.push_back(make_pair(finalStartRange, finalEndRange));
	    	}
	    }
	    //cout << "Ranges: " << bs_ranges.size() << endl;
	    vector<vector<int>> consequentList;
	    for (pair<int, int> it : bs_ranges){
	    	vector<int> tmp;
	    	bSBWT->getConsequents(tmp, 0, it.first, it.second, 2, -1, consequentList, predictionCount, consequentBits);
	    }
	    //put all ranges into CT
	    for (vector<int> consequent : consequentList) push(consequent, errors, initialLength, size);
	    
	    if (predictionCount > MAXPREDICTIONCOUNT) {
	    	stop = true;
	    	return;
		}
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

int subseqPredictor::start(int* query, int size){ // this function will manage deletion of the first items.
	consequentBits = new bit_vector(bSBWT->L.size(), 0); // I have to delete this at the end of this function or before returning
	stop = false;
	prediction = -1; score = -1.0;
	predictionCount = 0;
	countTable.clear();
	if (size == 2) {
		predict(query, size, MAXPREDICTIONCOUNT, size, 0);
		if (!stop) generateSubqueries(query, size);
	} else if (size < 2) return getBest();
	for (int k = 0; k < size - 1; k++) {
		predictionCount = 0;
		predict(&query[k], size - k, MAXPREDICTIONCOUNT, size, 0);
		if (stop) {delete consequentBits; return getBest();}
		generateSubqueries(&query[k], size - k); 
		if (stop) {delete consequentBits; return getBest();}
	}
	delete consequentBits;
	return getBest();
}

void subseqPredictor::generateSubqueries(int* query, int size){
	int initialLength = size;
	for (int i = 0; i < size; i++){
        //cout << i << endl;
        int old_item = query[i];
        query[i] = -2;
        // for (int item : y) cout << item << " ";
        // cout << endl;
        predict(query, size, MAXPREDICTIONCOUNT, initialLength, 1);
        //predict; if permanent stop abbort and return answer
        query[i] = old_item;
        //if (stop) return;
    }
    if (stop) return;
    if (size > 2){
        for (int i = 0; i < size; i++){
            for (int j = i + 1; j < size; j++){
                //cout << i << j << endl;
                int old_item_1 = query[i];
                int old_item_2 = query[j];
                query[i] = -2;
                query[j] = -2;
                // for (int item : y) cout << item << " ";
                // cout << endl;

                predict(query, size, MAXPREDICTIONCOUNT, initialLength, 2);
                //predict; if permanent stop abbort and return answer

                query[i] = old_item_1;
                query[j] = old_item_2;
                //if (stop) return;
            }
        }
    }
}