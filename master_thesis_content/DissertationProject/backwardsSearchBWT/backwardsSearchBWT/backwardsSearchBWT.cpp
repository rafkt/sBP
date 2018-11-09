//
//  countBefore.cpp
//  countBefore
//
//  Created by Rafael Ktistakis on 20/06/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//
// Since now sdsl-lite library is included, the compile should be done thorough the
// provided Makefile

#include "backwardsSearchBWT.h"
#include <assert.h>
#include <fstream>
#include "suffixArray.h"

using namespace suffixArray;


backwardsSearchBWT::backwardsSearchBWT(const string filename, bool similar): similar(similar){
    letterNode l;
    int counter = 0;
    size_t vector_counter = 0;
    ofstream tmpOutputFile;
    int lastItemcounter = 0;
    stop = false;
    ifstream file(filename);
    seqNumber = initialise(file, 10);
    this->L = BWT2WT();
    for (int i = 0; i < this->L.size(); i++) {
        try {
            alphabet_tmp.at(this->L[i])->appears++; //could be avoided though a simple rank call
        } catch (out_of_range e) {
            l = new letternode();
            l->c = this->L[i];
            l->appears = 1;
            alphabet_tmp.insert({l->c, l});
        }
    }
    this->alphabetCounters = int_vector<>(L.sigma, 1, 0);
    int_vector<> tmp_v(alphabet_tmp.size(), 1, 0);
    for (myMap::iterator mapIt = alphabet_tmp.begin(); mapIt != alphabet_tmp.end(); mapIt++) {
        l = mapIt->second;
        tmp_v[counter++] = l->c;
        this->alphabetCounters[vector_counter] = vector_counter == 0 ? l->appears : l->appears + this->alphabetCounters[vector_counter - 1];
        vector_counter++;
    }
    store_to_file(tmp_v, "tmp.txt");
    construct(this->alphabet, "tmp.txt", 0);
    ofstream output("tmp.txt", std::ios::binary | std::ios::trunc);
    util::bit_compress(this->alphabetCounters);
    assert(this->alphabetCounters.size() == this->L.sigma);
    assert(this->L.sigma == this->alphabet.size());
    clearALL();
    deleteMap();
}

void backwardsSearchBWT::deleteMap(){
    letterNode l;
    for (myMap::iterator mapIt = alphabet_tmp.begin(); mapIt != alphabet_tmp.end(); mapIt++) {
        l = mapIt->second;
        delete l;
    }
    alphabet_tmp.erase(alphabet_tmp.begin(), alphabet_tmp.end());
}

backwardsSearchBWT::~backwardsSearchBWT(){
    deleteMap();
}

int* backwardsSearchBWT::findRange(int* query, int size){
    countRange(query, size);
    int* ranges = new int[4];
    ranges[0] = finalStartIndex;
    ranges[1] = finalEndIndex;
    ranges[2] = finalStartIndexPermuted;
    ranges[3] = finalEndIndexPermuted;
    return ranges;
}

int backwardsSearchBWT::countRange(const int & c, const int & y){
    int letterPos = alphabet.select(1, y);
    int rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
    int rangeEnd = alphabetCounters[letterPos] - 1;
    return countRange(rangeStart, rangeEnd, c);
}

int backwardsSearchBWT::countRange(int* xy, int size){
    bool similarSearchON = true;
    finalStartIndex = -1;
    finalEndIndex = -1;
    finalStartIndexPermuted = -1;
    finalEndIndexPermuted = -1;
    int rangeStart = 0;
    int rangeEnd = 0;
    int rankStartValue, rankEndValue;
    bool flag = false;
    try {
        for (int i = 0; i < size; i++) { //red not reversly because we want countAfter, not countBefore
            int letterPos = alphabet.select(1, xy[i]);
            if (i != 0){
                rankStartValue = L.rank(rangeStart, xy[i]);
                rankEndValue = L.rank(rangeEnd + 1, xy[i]);
            }
            if (xy[i] == 99999) {
                stop = true;
                return 0; // not a normal sequence item
            }
            if (!flag) {
                rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                rangeEnd = alphabetCounters[letterPos] - 1;
                flag = true;
                if (size == 3 && xy[i + 2] != 99999 && xy[i + 1] != 99999 && similarSearchON) {
                    try {
                        countRangeLastTwoLettersPermuted(rangeStart, rangeEnd, &xy[i]); //pointer to ith position of array .. 
                    } catch (logic_error e) {
                    }
                }else if (size == 2 && xy[i + 1] != 99999 && similarSearchON){
                    letterPos = alphabet.select(1, xy[i + 1]);
                    rankStartValue = L.rank(rangeStart, xy[i + 1]);
                    rankEndValue = L.rank(rangeEnd + 1, xy[i + 1]);
                    if (rankEndValue - rankStartValue != 0){
                        int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                        rangeStart = rankStartValue + range2Add;
                        rangeEnd = rankEndValue + range2Add - 1;
                        finalStartIndex = rangeStart;
                        finalEndIndex = rangeEnd;
                    }
                    letterPos = alphabet.select(1, xy[i + 1]);
                    rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                    rangeEnd = alphabetCounters[letterPos] - 1;
                    letterPos = alphabet.select(1, xy[i]);
                    rankStartValue = L.rank(rangeStart, xy[i]);
                    rankEndValue = L.rank(rangeEnd + 1, xy[i]);
                    if (rankEndValue - rankStartValue != 0){
                        int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                        rangeStart = rankStartValue + range2Add;
                        rangeEnd = rankEndValue + range2Add - 1;
                        finalStartIndexPermuted = rangeStart;
                        finalEndIndexPermuted = rangeEnd;
                    }
                    return 1;
                }
            }else if (rankEndValue - rankStartValue != 0){
                int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                rangeStart = rankStartValue + range2Add;
                rangeEnd = rankEndValue + range2Add - 1;
                if (size > 2 && i == size - 3 && rankEndValue - rankStartValue != 0 && xy[i + 2] != 99999 && xy[i + 1] != 99999 && similarSearchON) {
                    try {
                        countRangeLastTwoLettersPermuted(rangeStart, rangeEnd, &xy[i]);
                    } catch (logic_error e) {
                    }
                }
            }else {
                stop = true;
                return 0;
            }
            if (i == size - 1) {
                finalStartIndex = rangeStart;
                finalEndIndex = rangeEnd;
                return 1;
            }
        }
    } catch (logic_error e) {
        stop = true;
        return 0;
    }
    //cout << "assertion to check: " << subStringOccurences(c + xy, this->original) << endl;
    //assert(subStringOccurences(c + xy, this->original) == counter);
    return 1;
}

void backwardsSearchBWT::countRangeLastTwoLettersPermuted(int rangeStart, int rangeEnd, int* xy){
    int letterPosPermuted = alphabet.select(1, xy[2]);
    int range2AddPermuted = letterPosPermuted != 0 ? alphabetCounters[letterPosPermuted - 1] : 0;
    int rankStartValuePermuted = L.rank(rangeStart, xy[2]);
    int rankEndValuePermuted = L.rank(rangeEnd + 1, xy[2]);
    int rangeStartPermuted = rankStartValuePermuted + range2AddPermuted;
    int rangeEndPermuted = rankEndValuePermuted + range2AddPermuted - 1;
    if (rankEndValuePermuted - rankStartValuePermuted != 0) {
        letterPosPermuted = alphabet.select(1, xy[1]);
        range2AddPermuted = letterPosPermuted != 0 ? alphabetCounters[letterPosPermuted - 1] : 0;
        rankStartValuePermuted = L.rank(rangeStartPermuted, xy[1]);
        rankEndValuePermuted = L.rank(rangeEndPermuted + 1, xy[1]);
        rangeStartPermuted = rankStartValuePermuted + range2AddPermuted;
        rangeEndPermuted = rankEndValuePermuted + range2AddPermuted - 1;
        if (rankEndValuePermuted - rankStartValuePermuted != 0){
            finalStartIndexPermuted = rangeStartPermuted;
            finalEndIndexPermuted = rangeEndPermuted;
        }
    }
}

// a simple function that simulates COUNTRANGE
int backwardsSearchBWT::countRange(const int & j, const int & k, const int & c){
    return L.rank(k + 1, c) - L.rank(j, c);
}

////trivial rank function
//int backwardsSearchBWT::rank(const char & c, const int & i){
//    int counter = 0;
//    for (int k = 0; k < L.size() && k <= i; k++) {
//        if (L[k] == c) counter++;
//    }
//    return counter;
//}
//
//// trivial select function
//int countBefore::select(const char & c, const int & i){
//    int counter = 0;
//    for (int k = 0; k < L.size(); k++) {
//        if (L[k] == c){
//            counter++;
//            if (counter == i) return k;
//        }
//    }
//    return -1;
//}

int backwardsSearchBWT::subStringOccurences( const std::string & str, const std::string & obj ) {
    int n = 0;
    std::string ::size_type pos = 0;
    while( (pos = obj.find( str, pos ))
          != std::string::npos ) {
    	n++;
    	pos += str.size();
    }
    return n;
}