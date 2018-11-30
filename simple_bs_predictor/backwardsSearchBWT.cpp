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

using namespace suffixArray;

float backwardsSearchBWT::sizeInMegabytes(){
    return size_in_mega_bytes(L) + size_in_mega_bytes(alphabet) + size_in_mega_bytes(alphabetCounters) + size_in_mega_bytes(*LplusOne);
}


backwardsSearchBWT::backwardsSearchBWT(const string filename){
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

    LplusOne = new int_vector<>(L.size(), 0, L.size());
    int newRangeStart = -1;
    for (int i = 0; i < L.size(); i++){
        int return_val = fowawrdTraversal(i, newRangeStart);
        assert(return_val != -1);
        (*LplusOne)[i] = newRangeStart;
    }

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
    delete LplusOne;
    deleteMap();
}

// int* backwardsSearchBWT::findRange(int* query, int size){
//     countRange(query, size);
//     int* ranges = new int[4];
//     ranges[0] = finalStartIndex;
//     ranges[1] = finalEndIndex;
//     ranges[2] = finalStartIndexPermuted;
//     ranges[3] = finalEndIndexPermuted;
//     return ranges;
// }

// int backwardsSearchBWT::countRange(const int & c, const int & y){
//     int letterPos = alphabet.select(1, y);
//     int rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
//     int rangeEnd = alphabetCounters[letterPos] - 1;
//     return countRange(rangeStart, rangeEnd, c);
// }

// int backwardsSearchBWT::countRange(int* xy, int size, int rangeStart, int rangeEnd){
//     finalStartIndex = -1;
//     finalEndIndex = -1;
//     finalStartIndexPermuted = -1;
//     finalEndIndexPermuted = -1;

    

//     int rankStartValue, rankEndValue;
//     bool flag = false;
//     try {
//         for (int i = 0; i < size; i++) { //red not reversly because we want countAfter, not countBefore
//             int letterPos = alphabet.select(1, xy[i]);
//             if (i != 0){
//                 rankStartValue = L.rank(rangeStart, xy[i]);
//                 rankEndValue = L.rank(rangeEnd + 1, xy[i]);
//             }
//             if (xy[i] == 99999) {
//                 stop = true;
//                 return 0; // not a normal sequence item
//             }
//             if (!flag && rangeStart == 0 && rangeEnd == 0) {
//                 rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
//                 rangeEnd = alphabetCounters[letterPos] - 1;
//                 flag = true;
//            }else if (rankEndValue - rankStartValue != 0){
//                 int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
//                 rangeStart = rankStartValue + range2Add;
//                 rangeEnd = rankEndValue + range2Add - 1;
//             }else {
//                 stop = true;
//                 return 0;
//             }
//             if (i == size - 1) {
//                 finalStartIndex = rangeStart;
//                 finalEndIndex = rangeEnd;
//                 return 1;
//             }
//         }
//     } catch (logic_error e) {
//         stop = true;
//         return 0;
//     }
    

//     return 1;
// }

int backwardsSearchBWT::searchQuery(int* xy, int size, int& finalStartRange, int& finalEndRange){
    if (size < 1) return -1;
    int startRange, endRange;
    getRange(xy[0], startRange, endRange);
    finalStartRange = startRange;
    finalEndRange = endRange;
    if (startRange == -1 || endRange == -1) {/*cout << "NOT FOUND" << endl;*/ return -1;}
    for (int i = 1; i < size; i++){
        if (search(xy[i], startRange, endRange, finalStartRange, finalEndRange) == -1) {/*cout << "NOT FOUND" << endl;*/ return -1;}
        startRange = finalStartRange;
        endRange = finalEndRange;
    }
    //cout << "FOUND" << endl;
    return 1;
}

int backwardsSearchBWT::search(int c, int rangeStart, int rangeEnd, int& newRangeStart, int& newRangeEnd){
    if (rangeEnd - rangeStart >= 0 && rangeEnd != -1 && rangeStart != -1){
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

int backwardsSearchBWT::fowawrdTraversal(int index, int& newRangeStart){
    if (index < 0) return -1;
    int letterPos = alphabet.select(1, L[index]);
    int range2Add = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
    int rangeStart = L.rank(index + 1, L[index]) + range2Add - 1;
    newRangeStart = rangeStart;
    return L[rangeStart]; 
}

int backwardsSearchBWT::backwardTraversal(int index, int& back_index){
    if (index < 0) return -1;
    int f_index = -1;
    //will replace the for loop below <linear search> with binary search
    // for(int i = 0; i < alphabetCounters.size(); i++){
    //     if (alphabetCounters[i] >= index + 1){
    //         f_index = i;
    //         break;
    //     }
    // }
    //binary search below

    int low = 0, high = alphabetCounters.size(); // numElems is the size of the array i.e arr.size() 
    while (low != high) {
        int mid = (low + high) / 2; // Or a fancy way to avoid int overflow
        if (alphabetCounters[mid] < index + 1) {
            /* This index, and everything below it, must not be the first element
             * greater than what we're looking for because this element is no greater
             * than the element.
             */
            low = mid + 1;
        }
        else {
            /* This element is at least as large as the element, so anything after it can't
             * be the first element that's at least as large.
             */
            high = mid;
        }
    }
    //Now, low and high both point to the element in question.
    f_index = low;

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
    int letterPos;
    try {
        letterPos = alphabet.select(1, c);
    } catch( std::logic_error ){
        rangeStart = -1; rangeEnd = -1;
        return;
    }
    rangeStart = letterPos != 0 ? alphabetCounters[letterPos - 1] : 0;
    rangeEnd = alphabetCounters[letterPos] - 1;
}

int backwardsSearchBWT::backwardError(int* xy, int size, set<int>& substitutedItems){
    //for now, xy will note an error position with -2
    if (size < 2) return -1;
    //std::vector<int> patternVector(size);
    //copy(xy, xy + size, patternVector.begin());
    //pattern_stack.push(patternVector);
    //patternVector = pattern_stack.top();
    
    //set<int> substitutedItems;
    int rankItems, whichIndex;
    if (xy[1] != -2) {rankItems = L.rank(L.size(), xy[1]); whichIndex = 1;}
    else {rankItems = L.rank(L.size(), xy[2]); whichIndex = 2;}
    for (int j = 0; j < rankItems; j++){
        int index = L.select(j + 1, xy[whichIndex]);
        int backtrav_index;
        int item2subst = backwardTraversal(index, backtrav_index);
        if (item2subst != 99999) substitutedItems.insert(item2subst);
        //else cout << "<-- No 99999 expansion " << endl;
        
    }
    // for (auto i = substitutedItems.begin(); i != substitutedItems.end(); ++i)
    //     cout << *i << ' ';
    // cout << endl;

    return 1;
}


void backwardsSearchBWT::neighborExpansion(vector<int> xy, int index, int rangeStart, int rangeEnd, vector<pair<int, int>>& ranges){//size should be over or equal to 2
    int newRangeStart, newRangeEnd;
    if (index == xy.size()){
        //cout << "-range: " << rangeStart << "," << rangeEnd  << " size: " << endl;
        // for (int item : xy) cout << item << " ";
        // cout << "." << endl;
        //counterMap res = scan(rangeStart, rangeEnd);
        ranges.push_back(make_pair(rangeStart, rangeEnd));
        // for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
        //     cout << mapIt->second << " ";
        // }
        // cout << endl;
        return;
    }else{
        if (xy[index] == -2){
            //counterMap res = scan(rangeStart, rangeEnd);
            
            uint64_t quantity;
            std::vector<uint64_t> cs(L.sigma);      // list of characters in the interval
            std::vector<uint64_t> rank_c_i(L.sigma);    // number of occurrence of character in [0 .. i-1]
            std::vector<uint64_t> rank_c_j(L.sigma);    // number of occurrence of character in [0 .. j-1]
            interval_symbols(L, rangeStart, rangeEnd + 1, quantity, cs, rank_c_i, rank_c_j);

            // for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            //     cout << mapIt->second << " ";
            // }
            // cout << endl;
            //for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            for (uint64_t it = 0; it < quantity; it++)
                //if (mapIt->first == 99999) {/*cout << "No 99999 expansion " << endl;*/ continue;}
                if (cs[it] == 99999){
                //cout << mapIt->second << endl;
                xy[index] = cs[it];//mapIt->first;
                // for (int item : xy) cout << item << " ";
                // cout << ">" << endl;
                if (search(cs[it]/*mapIt->first*/, rangeStart, rangeEnd, newRangeStart, newRangeEnd) == -1) {/*cout << "NOT FOUND" << endl;*/ return;}
                //rangeStart = newRangeStart;
                //rangeEnd = newRangeEnd;
                // if (index + 1 == xy.size() - 1) {
                //     cout << "-range: " << newRangeStart << "," << newRangeEnd  << " size: " << res.size() << endl;
                // } else 
                neighborExpansion(xy, index + 1, newRangeStart, newRangeEnd, ranges);
            }
        }else{
            if (search(xy[index], rangeStart, rangeEnd, newRangeStart, newRangeEnd) == -1) {/*cout << "NOT FOUND" << endl;*/ return;}
            rangeStart = newRangeStart;
            rangeEnd = newRangeEnd;
            if (index == xy.size() - 1)  {
                //cout << "--range: " << newRangeStart << "," << newRangeEnd  << endl;
                ranges.push_back(make_pair(newRangeStart, newRangeEnd));
                // for (int item : xy) cout << item << " ";
                // cout << ".." << endl;
            }
            else neighborExpansion(xy, index + 1, newRangeStart, newRangeEnd, ranges);
        }
    }
    return;
    
}

void backwardsSearchBWT::getConsequents(vector<int> xy, int index, int rangeStart, int rangeEnd, int length, int d, vector<vector<int>>& consequentList, int& predictionCount, sdsl::bit_vector* consequentBits){//size should be over or equal to 2
    int newRangeStart, newRangeEnd;
    if (d > 0) {
        if (d == 99999) {
            //cout << "Consequent contains 99999; terminating getting more consequents items" << endl;
            if (xy.size() > 0){consequentList.push_back(xy); predictionCount++;}
            // cout << "getConsequent returns: ";
            // for (int i : xy) {
            //     cout << i << " ";
            // }
            // cout << endl;
            return;
        }

        xy.push_back(d);
    }
    if (index == length){
        // cout << "-range: " << rangeStart << "," << rangeEnd  << " size: " << endl;
        // cout << "getConsequent returns: ";
        // for (int i : xy) {
        //     cout << i << " ";
        // }
        // cout << endl;
        consequentList.push_back(xy);
        predictionCount++;
        return;
    }else{
        // for (int bit_index = rangeStart; bit_index <= rangeEnd; bit_index++) {
        //     if ((*consequentBits)[bit_index]) {/*cout << "Bit Index signaled a return" << endl;*/ return;}
        //     (*consequentBits)[bit_index] = 1;
        // }
        //counterMap res = scan(rangeStart, rangeEnd);
        //for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
        for (int i = rangeStart; i <= rangeEnd; i++){
            if ((*consequentBits)[i] == 1) continue;
            (*consequentBits)[i] = 1;
            search(L[i], i, i, newRangeStart, newRangeEnd);
            //search(mapIt->second, rangeStart, rangeEnd, newRangeStart, newRangeEnd);
            getConsequents(xy, index + 1, newRangeStart, newRangeEnd, length, /*mapIt->second*/ L[i], consequentList, predictionCount, consequentBits);
        }
    }
    return;
}


void backwardsSearchBWT::getQuickConsequents(int rangeStart, int rangeEnd, vector<vector<int>>& consequentList, int& predictionCount, sdsl::bit_vector* consequentBits){//size should be over or equal to 2
    for (int i = rangeStart; i <= rangeEnd; i++){
        if ((*consequentBits)[i] == 1) continue;
        (*consequentBits)[i] = 1;
        vector<int> conseq;
        if (L[i] != 99999){
            conseq.push_back(L[i]);
            if (L[(*LplusOne)[i]] != 99999) conseq.push_back(L[(*LplusOne)[i]]);
            consequentList.push_back(conseq);
            predictionCount++;
        }
    }
    return;
}

counterMap backwardsSearchBWT::scan(int rangeStart, int rangeEnd){
    counterMap mostFrequent;
    if (rangeStart < 0 || rangeEnd < 0) return mostFrequent;
    double threshold = 1.7;
    pair<counterMap::iterator, bool> ret;
    int overallRangeLength = rangeEnd - rangeStart + 1;
    double relay_value = overallRangeLength / (double)alphabet.size();
    if (relay_value <= threshold){ // this threshold was based on research that was done in my master thesis
        for (int i = rangeStart; i <= rangeEnd; i++){
            ret = mostFrequent.insert(std::pair<int, int>(L[i], 1));
            if (ret.second == false){
                ret.first->second = ret.first->second + 1;
            }
        }
    }else{
        for (size_t i = 0; i < alphabet.size(); i++){
            int counter = countRange(rangeStart, rangeEnd, alphabet[i]);
            if (counter > 0) mostFrequent.insert({alphabet[i], counter});
        }
   }
    //mostFrequent.insert({-1, overallRangeLength});
    return mostFrequent;

}


// a simple function that simulates COUNTRANGE
int backwardsSearchBWT::countRange(const int & j, const int & k, const int & c){
    return L.rank(k + 1, c) - L.rank(j, c);
}

// int backwardsSearchBWT::subStringOccurences( const std::string & str, const std::string & obj ) {
//     int n = 0;
//     std::string ::size_type pos = 0;
//     while( (pos = obj.find( str, pos ))
//           != std::string::npos ) {
//     	n++;
//     	pos += str.size();
//     }
//     return n;
// }