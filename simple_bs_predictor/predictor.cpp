//
//  predictor.cpp
//  countBefore
//
//  Created by Rafael Ktistakis on 23/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "predictor.h"
//#include "similar_queries.h"
#include <fstream>
#include <sys/time.h>
#include <time.h>

predictor::predictor(const string filename, bool similarSearch, bool threashold): mostFrequentToKeepCounter(0), similarSearch(similarSearch), threashold(threashold){
    bSBWT = new backwardsSearchBWT(filename, similarSearch);
}

predictor::~predictor(){
    delete bSBWT;
}

int compare(const void* a, const void* b){
    return ( *(int*)a - *(int*)b );
}

double predictor::itemConfidence(int item){
	int frequency = bSBWT->L.rank(bSBWT->L.size() - 1, item);
	//cout << "Freq " << frequency << " Sequences " << bSBWT->seqNumber << endl;
	return ((double) frequency / bSBWT->seqNumber);
}

int predictor::datasetSeqNumber(){
	return bSBWT->seqNumber;
}

aMap predictor::predictWithRecursiveDivider(int* query, int size, int candidatesNumber, int minRecursion, int maxRecursion){
	
	aMap predictionResult;
	
	//good idea would be to remove unseen items from query..
	vector<int> queryWithOnlyKnownItems;
	for (int i = 0; i < size; i++) {
		if (bSBWT->alphabet.rank(bSBWT->alphabet.size() - 1, query[i]) != 0) queryWithOnlyKnownItems.push_back(query[i]);
	}
	
	if (queryWithOnlyKnownItems.size() == 0) return predictionResult;
	
	int initialTargetArraySize = size; // save it to calculate the weight later...
	maxRecursion = (maxRecursion > size) ? size : maxRecursion;
	
	
	for(int i = minRecursion ; i < maxRecursion && predictionResult.size() == 0; i++) {
		int minSize = size - i; //setting the minSize for the recursiveDivider
		
		//Dividing the target sequence into sub sequences
		RecursiveDivider(&queryWithOnlyKnownItems[0], queryWithOnlyKnownItems.size(), minSize, predictionResult, initialTargetArraySize, candidatesNumber);
	}
	//when an item appears equal times in the count table then confidence should be compared..
	float max = -1;
	double maxConfidence = -1;
	int maxTimesAppeared = -1;
	aMap::reverse_iterator maxIt = predictionResult.rend();
	for (aMap::reverse_iterator mapIt = predictionResult.rbegin(); mapIt != predictionResult.rend(); mapIt++){
		if (mapIt->first > max){
			max = mapIt->first;
			maxIt = mapIt;
		}else if (mapIt->first == max){
			//find the maximum confidence and keep that iterator
			int temp = bSBWT->L.rank(bSBWT->L.size() - 1, mapIt->second);
			if (temp > maxTimesAppeared){
				maxTimesAppeared = temp;
				maxIt = mapIt;
			}
		}
	}
	
	//place the item with maximum confidence first on the count table.
	if (predictionResult.rbegin() != maxIt){
		int item = maxIt->second;
		predictionResult.erase(--maxIt.base());
		predictionResult.insert({max * 2, item});
	}
	
	//for (aMap::reverse_iterator mapIt = predictionResult.rbegin(); mapIt != predictionResult.rend(); mapIt++) cout << mapIt->first << " : " << mapIt->second << endl;
	
	return predictionResult;
}

void predictor::RecursiveDivider(int* query, int qSize, int minSize, aMap& predictionResult, int initialTargetArraySize, int answersPerPrediction) {
	
	int size = qSize;
	
	//if the target is small enough or already too small
	if(size <= minSize) {
		return;
	}
	
	//Setting up the weight multiplier for the countTable
	float weight = (float)size / initialTargetArraySize;
	
	aMap res = predict(query, size, answersPerPrediction);
	//UpdateCountTable(targetArray, weight, countTable, hashSidVisited);
	//updating predictionResult Map (basically the counttable)
	//mapIt->first: #times, mapIt->second: wich item, mapIt->first == -1 then mapIt->second: overall times sequence matched
	//map sorted from -1 to maximum, so trasverse it reversely
	
	vector<aMap::iterator> elemetns;
	if(res.size() != 0 && predictionResult.size() != 0){
		for (aMap::iterator mapIt = res.begin(); mapIt != res.end(); mapIt++){
			bool found = false;
			float temp;
			for (aMap::iterator innerMapIt = predictionResult.begin(); innerMapIt != predictionResult.end(); innerMapIt++){
				if (mapIt->first == -1 && innerMapIt->first == -1) {
					temp = innerMapIt->second;
					predictionResult.erase(innerMapIt);
					predictionResult.insert({-1, temp + mapIt->second});
					found = true;
					break;
				}else if(mapIt->second == innerMapIt->second){
					temp = mapIt->first * weight;
					predictionResult.erase(innerMapIt);
					found = true;
					break;
				}
			}
			if (!found) elemetns.push_back(mapIt);
			else if(mapIt->first != -1) predictionResult.insert({temp, mapIt->second});
		}
		for (int i = 0; i < elemetns.size(); i++) predictionResult.insert({elemetns[i]->first * weight, elemetns[i]->second});
		//weight should be computed and multiplied with current value
	}else if (res.size() != 0 && predictionResult.size() == 0) predictionResult = res;
	
	
	//Hiding one item at the time from the target
	for(int toHide = 0; toHide < size; toHide++) {
		
		//Constructing a new sequence from the target without the "toHide" item
		vector<int> newSequence(size -1);
		int currentPosition =0;
		for(int toUse = 0; toUse < size; toUse++) {
			if(toUse != toHide) {
				newSequence[currentPosition++] = query[toUse];
			}
		}
		
		RecursiveDivider(&newSequence[0], size - 1, minSize, predictionResult, initialTargetArraySize, answersPerPrediction);
	}
}

aMap predictor::predict(int* query, int size, int candidatesNumber){
    bool similarSearchON = false;//similarSearch;
    bool all_empty;
    int** queries;
    int** overallRanges;
    int* ranges;
    int overallRangeLength;
    int rangeArraySize = 0;
   // similar_queries* sq;
    aMap mostFrequent;
	
	//mapIt->first: #times, mapIt->second: wich item, mapIt->first == -1 then mapIt->second: overall times sequence matched
	//map sorted from -1 to maximum, so trasverse it reversely
	
    mostFrequentToKeepCounter = candidatesNumber;
    
//    ofstream outputFile;
//    
//    int res = access("ranges.csv", R_OK);
//    if (res < 0) {
//        if (errno == ENOENT) {
//            // file does not exist
//            outputFile.open("ranges.csv");
//            outputFile << "Start Range,End Range\n";
//        }
//    }else{
//        outputFile.open("ranges.csv", ios_base::app);
//    }
    
    
    //sq = NULL;
    if (size == 1) {
        int letterPos = bSBWT->alphabet.select(1, query[0]);
        int rangeStart = letterPos != 0 ? bSBWT->alphabetCounters[letterPos - 1] : 0;
        int rangeEnd = bSBWT->alphabetCounters[letterPos] - 1;
        overallRanges = new int*[1];
        overallRanges[0] = new int[2];
        overallRanges[0][0] = rangeStart;
        overallRanges[0][1] = rangeEnd;
        rangeArraySize = 1;
    //}else if (similarSearchON){
        // sq = new similar_queries(query, size); // for query bigger than 2 items
        // all_empty = true;
        // queries = sq->queryArray;
        // rangeArraySize = sq->getQueryArraySize() * 2;
        // overallRanges = new int*[rangeArraySize];
        // int i_counter = 0;
        // for (int i = 0; i < sq->getQueryArraySize(); i++){
        //     ranges = bSBWT->findRange(queries[i], size);
        //     overallRanges[i_counter] = new int[2];
        //     overallRanges[i_counter + 1] = new int[2];;
        //     overallRanges[i_counter][0] = ranges[0];
        //     overallRanges[i_counter][1] = ranges[1];
        //     overallRanges[i_counter + 1][0] = ranges[2];
        //     overallRanges[i_counter + 1][1] = ranges[3];
        //     if (ranges[0] != -1 || ranges[2] != -1) all_empty = false;
        //     i_counter += 2;
        //     delete [] ranges;
        // }
        
        // if (all_empty) {
        //     for (int i = 0; i < rangeArraySize; i++){
        //         delete [] overallRanges[i];
        //     }
        //     delete [] overallRanges;
        //     delete sq;
        //     return mostFrequent;//-1;
        // }
        
        // sq->rangesUnion(overallRanges, rangeArraySize);
        // delete sq;
    }else{
        rangeArraySize = 1;
        overallRanges = new int*[rangeArraySize];
        overallRanges[0] = new int[2];
        ranges = bSBWT->findRange(query, size);
        overallRanges[0][0] = ranges[0];
        overallRanges[0][1] = ranges[1];
        delete [] ranges;
        if (overallRanges[0][0] == -1 || overallRanges[0][1]== -1 ){
            delete [] overallRanges[0];
            delete [] overallRanges;
            return mostFrequent;//-1;
        }
    }
    
    //return 1; // for evaluation purposes only - return before doing any prediction. This will calculate only the time spent for backward search
    
    overallRangeLength = 0;
    for (int i = 0; i < rangeArraySize; i++) {
        if (overallRanges[i][1] != -1) overallRangeLength += overallRanges[i][1] - overallRanges[i][0] + 1;
    }
    
    int max = -1;
    int pos = -1;
    int counter = 0;
    double threshold = 1.7;
    bool threshold_ON = threashold;
    double relay_value = overallRangeLength / (double)bSBWT->alphabet.size();
    if (relay_value <= threshold && threshold_ON){
        int* range_items = new int[overallRangeLength];
        int addingCounter = 0;
        for (int i = 0; i < rangeArraySize; i++) {
            if (overallRanges[i][1] != -1) {
                for (int k = 0; k < overallRanges[i][1] - overallRanges[i][0] + 1; k++) {
                    range_items[addingCounter++] = bSBWT->L[k + overallRanges[i][0]];
                }
            }
        }
        qsort (range_items, overallRangeLength, sizeof(int), compare);
        int previous_item = range_items[0];
        for (int i = 0; i < overallRangeLength; i++) {
            if (range_items[i] == previous_item){
                counter++;
            }
            else{
                /* for collecting top most frequent
                if (counter > max) {
                    max = counter;
                    pos = i - 1;
                }
                 */
                mostFrequent.insert({counter, range_items[i - 1]});
                counter = 1;
            }
            previous_item = range_items[i];
        }
        /*
        if (counter > max) {
            max = counter;
            pos = overallRangeLength - 1;
        }
        */
        if (counter > 0) {
            mostFrequent.insert({counter, range_items[overallRangeLength - 1]});
        }
        
        for (int i = 0; i < rangeArraySize; i++){
            delete [] overallRanges[i];
        }
        delete [] overallRanges;
        int return_value;
        /*
        if (max > 0 && range_items[pos] != 99999) return_value = range_items[pos];
        else return_value = -1;
        delete [] range_items;
        return return_value;
         */
        
        int c = 0;
        for (aMap::iterator mapIt = --mostFrequent.end(); mapIt != mostFrequent.begin(); mapIt--, c++) {
            if (c == mostFrequentToKeepCounter - 1) {
                mostFrequent.erase(mostFrequent.begin(), --mapIt);
                break;
            }
        }
        mostFrequent.insert({-1, overallRangeLength});
//        cout << mostFrequent.rbegin()->first << "/" << overallRangeLength << endl;
//        for (aMap::iterator mapIt = mostFrequent.begin(); mapIt != mostFrequent.end(); mapIt++) {
//            cout << mapIt->first << " " << mapIt->second << endl;
//        }
        return mostFrequent;
    }else{
        for (size_t i = 0; i < bSBWT->alphabet.size(); i++){
            for (int k = 0; k < rangeArraySize; k++) {
                if (overallRanges[k][1] != -1){
                    counter = bSBWT->countRange(overallRanges[k][0], overallRanges[k][1], bSBWT->alphabet[i]);
                    /*
                    if (counter > max){
                        max = counter;
                        pos = i;
                    }
                     */
                    mostFrequent.insert({counter, bSBWT->alphabet[i]});
                }
            }
        }
    }
    
//    //starting of threshold evaluation
//    
//    ofstream outputFile;
//    int res = access("AccessALLvsRanks.csv", R_OK);
//    if (res < 0) {
//        if (errno == ENOENT) {
//            // file does not exist
//            outputFile.open("AccessALLvsRanks.csv");
//            outputFile << "Range Length,Alphabet Size, Go through All in Range (ms),Entire Alphabet Rank (ms),Range / Alphabet ratio\n";
//        }
//    }else{
//        outputFile.open("AccessALLvsRanks.csv", ios_base::app);
//    }
//
//    int iterations = 1;
//    auto begin_ranges = clock();
//    
//    for (int it = 0; it < iterations; it++) {
//        int* range_items = new int[overallRangeLength];
//        int addingCounter = 0;
//        for (int i = 0; i < rangeArraySize; i++) {
//            if (overallRanges[i][1] != -1) {
//                for (int k = 0; k < overallRanges[i][1] - overallRanges[i][0] + 1; k++) {
//                    range_items[addingCounter++] = bSBWT->L[k + overallRanges[i][0]];
//                }
//            }
//        }
//        qsort (range_items, overallRangeLength, sizeof(int), compare);
//        int previous_item = range_items[0];
//        for (int i = 0; i < overallRangeLength; i++) {
//            if (range_items[i] == previous_item){
//                counter++;
//            }
//            else{
//                if (counter > max) {
//                    max = counter;
//                    pos = i - 1;
//                }
//                counter = 1;
//            }
//            previous_item = range_items[i];
//        }
//        if (counter > max) {
//            max = counter;
//            pos = overallRangeLength;
//        }
//    }
//    
//    
//    auto end_ranges = clock();
//    auto duration_ranges = ((double)(end_ranges - begin_ranges))/CLOCKS_PER_SEC;
//    
//    
//    auto begin_ranks = clock();
//    for (int it = 0; it < iterations; it++) {
//        for (size_t i = 0; i < bSBWT->alphabet.size(); i++){
//            for (int k = 0; k < rangeArraySize; k++) {
//                if (overallRanges[k][1] != -1){
//                    counter = bSBWT->countRange(overallRanges[k][0], overallRanges[k][1], bSBWT->alphabet[i]);
//                    if (counter > max){
//                        max = counter;
//                        pos = i;
//                    }
//                }
//            }
//        }
//    }
//    auto end_ranks = clock();
//    auto duration_ranks = ((double)(end_ranks - begin_ranks))/CLOCKS_PER_SEC;
//    
//    outputFile << ranges[1] - ranges[0] + 1 << "," << bSBWT->alphabet.size() << "," << duration_ranges / (double)iterations << "," << duration_ranks / (double)iterations << "," << relay_value <<"\n";
//    
//    return 0;
//    
//    //end of evaluation sector
    
    
//    outputFile << bSBWT->finalStartIndex << "," << bSBWT->finalEndIndex << "\n";
//    outputFile.close();
    
    for (int i = 0; i < rangeArraySize; i++){
        delete [] overallRanges[i];
    }
    delete [] overallRanges;
    
    int c = 0;
    for (aMap::iterator mapIt = --mostFrequent.end(); mapIt != mostFrequent.begin(); mapIt--, c++) {
        if (c == mostFrequentToKeepCounter - 1) {
            mostFrequent.erase(mostFrequent.begin(), --mapIt);
            break;
        }else if (mapIt->first == 0){
            mostFrequent.erase(mostFrequent.begin(), ++mapIt);
            break;
        }
    }
    mostFrequent.insert({-1, overallRangeLength});
//    cout << mostFrequent.rbegin()->first << "/" << overallRangeLength << endl;
//    for (aMap::iterator mapIt = mostFrequent.begin(); mapIt != mostFrequent.end(); mapIt--) {
//        cout << mapIt->first << " " << mapIt->second << endl;
//    }
    return mostFrequent;//(max < 0 ? -1 : bSBWT->alphabet[pos] != 99999 ? bSBWT->alphabet[pos] : -1);
}