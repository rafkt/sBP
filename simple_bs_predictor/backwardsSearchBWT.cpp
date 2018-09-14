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
#include <stack>
#include <algorithm>
#include <set>

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

int backwardsSearchBWT::countRange(int* xy, int size, int rangeStart, int rangeEnd){
    finalStartIndex = -1;
    finalEndIndex = -1;
    finalStartIndexPermuted = -1;
    finalEndIndexPermuted = -1;

    // int backtrav = -1;
    // cout << fowawrdTraversal(11) << endl;
    // cout << backwardTraversal(11, backtrav) << endl;
    // cout << "L position of backward Traversal: " << backtrav << endl;

    // cout << "Printing a range " << endl;

    // counterMap res = scan(0, 11);
    // for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
    //     cout << mapIt->first << " " << mapIt->second << endl;
    // }

    // cout << "Printing END" << endl;

    // int newRangeStart, newRangeEnd;
    // search(4, 9, 9, newRangeStart, newRangeEnd);
    // cout << "New range: " <<newRangeStart << " " << newRangeEnd << endl;

    // cout << "search with errors testing" << endl;
    // vector<int> x = {-2, 2, 10};
    // backwardError(&x[0], x.size());
    // cout << "END-----" << endl;

    //int start, end;
    vector<int> y = {3, 4};
    //getRange(y[0], start, end);
    //neighborExpansion(y, 1, start, end, 1, 1);
    //getConsequents(y, 0, 4, 6, 3, -1);
    
    // getRange(4, start, end);
    // cout << start << " " << end << endl;


    cout << "trying to test subquery generator" << endl;
        int _size = 2;
        for (int i = 0; i < _size; i++){
            //cout << i << endl;
            int old_item = y[i];
            y[i] = -2;
            for (int item : y) cout << item << " ";
            cout << endl;
            y[i] = old_item;
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
        }
    cout << "end of this test" << endl;

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
            if (!flag && rangeStart == 0 && rangeEnd == 0) {
                rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                rangeEnd = alphabetCounters[letterPos] - 1;
                flag = true;
           }else if (rankEndValue - rankStartValue != 0){
                int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
                rangeStart = rankStartValue + range2Add;
                rangeEnd = rankEndValue + range2Add - 1;
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
    

    return 1;
}


int backwardsSearchBWT::search(int c, int rangeStart, int rangeEnd, int& newRangeStart, int& newRangeEnd){
    if (rangeEnd - rangeStart >= 0){
        int rankStart = L.rank(rangeStart, c);
        int rankEnd = L.rank(rangeEnd + 1, c);
        if (rankEnd - rankStart == 0) return -1; //patern not found
        int letterPos = alphabet.select(1, c);
        int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
        newRangeStart = rankStart + range2Add;
        newRangeEnd = rankEnd + range2Add - 1;
        return 1;//valid input
    }else return 0; //no valid input
}

int backwardsSearchBWT::fowawrdTraversal(int index){
    if (index < 0) return -1;
    int letterPos = alphabet.select(1, L[index]);
    int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
    int rangeStart = L.rank(index + 1, L[index]) + range2Add - 1;
    return L[rangeStart]; 
}

int backwardsSearchBWT::backwardTraversal(int index, int& back_index){
    if (index < 0) return -1;
    int f_index = -1;
    for(int i = 0; i < alphabetCounters.size(); i++){
        if (alphabetCounters[i] >= index + 1){
            f_index = i;
            break;
        }
    }
   // cout << "dif: " << alphabetCounters[f_index] - alphabetCounters[f_index - 1] << endl;
    if (f_index == 0){
        back_index =  L.select(index + 1, alphabet[f_index]);
    }else{
        //cout << "L posistion: " << L.select(alphabetCounters[f_index] - alphabetCounters[f_index - 1], alphabet[f_index]) << endl;
        back_index = L.select(alphabetCounters[f_index] - (index + 1) == 0 ? alphabetCounters[f_index] - alphabetCounters[f_index - 1] : alphabetCounters[f_index] - (index + 1), alphabet[f_index]);

    }
    return alphabet[f_index];
}

void backwardsSearchBWT::getRange(int c, int& rangeStart, int& rangeEnd){
    int letterPos = alphabet.select(1, c);
    rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
    rangeEnd = alphabetCounters[letterPos] - 1;
}

int backwardsSearchBWT::backwardError(int* xy, int size){
    //for now, xy will note an error position with -2
    if (size < 2) return -1;
    //std::vector<int> patternVector(size);
    //copy(xy, xy + size, patternVector.begin());
    //pattern_stack.push(patternVector);
    //patternVector = pattern_stack.top();
    
    set<int> substitutedItems;
    int rankItems = L.rank(L.size(), xy[1]);
    for (int j = 0; j < rankItems; j++){
        int index = L.select(j + 1, xy[1]);
        int backtrav_index;
        int item2subst = backwardTraversal(index, backtrav_index);
        substitutedItems.insert(item2subst);
        
    }
    for (auto i = substitutedItems.begin(); i != substitutedItems.end(); ++i)
        cout << *i << ' ';
    cout << endl;

    return 1;
}


int backwardsSearchBWT::neighborExpansion(vector<int> xy, int index, int rangeStart, int rangeEnd, int subst, int call){//size should be over or equal to 2
    int newRangeStart, newRangeEnd;
    if (index == xy.size()){
        //cout << "-range: " << rangeStart << "," << rangeEnd  << " size: " << endl;
        counterMap res = scan(rangeStart, rangeEnd);
        for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            cout << mapIt->second << " ";
        }
        cout << endl;
        return 1;
    }else{
        if (xy[index] == -2){
            counterMap res = scan(rangeStart, rangeEnd);
            for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
                //cout << mapIt->first << " " << mapIt->second << endl;
                xy[index] = mapIt->second;
                search(mapIt->second, rangeStart, rangeEnd, newRangeStart, newRangeEnd);
                //rangeStart = newRangeStart;
                //rangeEnd = newRangeEnd;
                // if (index + 1 == xy.size() - 1) {
                //     cout << "-range: " << newRangeStart << "," << newRangeEnd  << " size: " << res.size() << endl;
                // } else 
                neighborExpansion(xy, index + 1, newRangeStart, newRangeEnd, subst, ++call);
            }
        }else{
            search(xy[index], rangeStart, rangeEnd, newRangeStart, newRangeEnd);
            rangeStart = newRangeStart;
            rangeEnd = newRangeEnd;
            if (index == xy.size() - 1)  {
                //cout << "--range: " << newRangeStart << "," << newRangeEnd  << endl;
                counterMap res = scan(newRangeStart, newRangeEnd);
                for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
                    cout << mapIt->second << " --";
                }
                cout << endl;
            }
            else neighborExpansion(xy, index + 1, newRangeStart, newRangeEnd, subst, ++call);
        }
    }
    return 1;
    
}

int backwardsSearchBWT::getConsequents(vector<int> xy, int index, int rangeStart, int rangeEnd, int length, int d){//size should be over or equal to 2
    int newRangeStart, newRangeEnd;
    if (d > 0) xy.push_back(d);
    if (index == length){
        //cout << "-range: " << rangeStart << "," << rangeEnd  << " size: " << endl;
        for (int i : xy) {
            cout << i << " ";
        }
        cout << endl;
        return 1;
    }else{
        counterMap res = scan(rangeStart, rangeEnd);
        for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            search(mapIt->second, rangeStart, rangeEnd, newRangeStart, newRangeEnd);
            getConsequents(xy, index + 1, newRangeStart, newRangeEnd, length, mapIt->second);
        }
    }
    return 1;
}

counterMap backwardsSearchBWT::scan(int rangeStart, int rangeEnd){
    counterMap mostFrequent;
    if (rangeStart < 0 || rangeEnd < 0) return mostFrequent;
    int counter = 0;
    double threshold = 1.7;
    int overallRangeLength = rangeEnd - rangeStart + 1;
    double relay_value = overallRangeLength / (double)alphabet.size();
    if (relay_value <= threshold){
        int* range_items = new int[overallRangeLength];
        int addingCounter = 0;
        for (int k = 0; k < overallRangeLength; k++) {
            range_items[addingCounter++] = L[k + rangeStart];
        }
        qsort (range_items, overallRangeLength, sizeof(int), compare);
        int previous_item = range_items[0];
        for (int i = 0; i < overallRangeLength; i++) {
            if (range_items[i] == previous_item){
                counter++;
            }
            else{

                mostFrequent.insert({counter, range_items[i - 1]});
                counter = 1;
            }
            previous_item = range_items[i];
        }
        if (counter > 0) {
            mostFrequent.insert({counter, range_items[overallRangeLength - 1]});
        }
    }else{
        for (size_t i = 0; i < alphabet.size(); i++){
            counter = countRange(rangeStart, rangeEnd, alphabet[i]);
            mostFrequent.insert({counter, alphabet[i]});
        }
    }
    //mostFrequent.insert({-1, overallRangeLength});
    return mostFrequent;

}


// a simple function that simulates COUNTRANGE
int backwardsSearchBWT::countRange(const int & j, const int & k, const int & c){
    return L.rank(k + 1, c) - L.rank(j, c);
}

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