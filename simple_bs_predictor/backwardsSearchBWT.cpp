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


double backwardsSearchBWT::sizeInMegabytes(){
    return size_in_mega_bytes(L) +  (8 + (sigma_seperator + 1) * 8)* 0.000001/*+ size_in_mega_bytes(*LplusOne)*/;
}


backwardsSearchBWT::backwardsSearchBWT(const string filename){
    letterNode l;
    ofstream tmpOutputFile;
    int lastItemcounter = 0;
    seqNumber =0;
    stop = false;
    ifstream file(filename);
    sigma_seperator = initialise(file, 10);
    this->L = BWT2WT();
    for (int i = 0; i < this->L.size(); i++) {
        if (this->L[i] == sigma_seperator) seqNumber++;
        try {
            alphabet_tmp.at(this->L[i])->appears++; //could be avoided though a simple rank call
        } catch (out_of_range e) {
            l = new letternode();
            l->c = this->L[i];
            l->appears = 1;
            alphabet_tmp.insert({l->c, l});
        }
    }
    this->alphabetCounters = new uint64_t[sigma_seperator + 1];
    int_vector<> tmp_v(sigma_seperator + 1, 1, 0);
    for (int i = 0; i <= sigma_seperator; i++) {
        //myMap::iterator mapIt = alphabet_tmp.begin(); mapIt != alphabet_tmp.end(); mapIt++
        
        bool missing = false;

        try {
            l = alphabet_tmp.at(i);
        } catch (out_of_range e) {
            missing = true;
        }
        //cout << l->c << endl;

        if (i == 0){
            if (!missing) this->alphabetCounters[l->c] = l->appears;
            else this->alphabetCounters[i] = 0;
        }else{
            if (!missing) this->alphabetCounters[l->c] = l->appears + this->alphabetCounters[l->c - 1];
            else this->alphabetCounters[i] = this->alphabetCounters[i - 1];
        }

        // this->alphabetCounters[l->c] = l->c == 0 ? l->appears : l->appears + this->alphabetCounters[l->c - 1];
        // cout << this->alphabetCounters[vector_counter] << endl;
    }

    // for (int i = 0; i < tmp_v.size(); i++){
    // //if (i != tmp_v[i]) cout << "Alphabet array indexes error" << endl;
    //     cout << i << ": " << tmp_v[i] << ": " << alphabetCounters[i] << endl;
    // }
    // exit(0);
//    LplusOne = new int_vector<>(L.size(), 0, 64);
//    int newRangeStart = L.size();
//    for (int i = 0; i < L.size(); i++){
//        int return_val = fowawrdTraversal(i, newRangeStart);
//        assert(return_val != -1);
//        (*LplusOne)[i] = newRangeStart;
//    }

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
   // delete LplusOne;
    delete[] alphabetCounters;
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
        // int letterPos = alphabet.select(1, c);
        // if (letterPos != c) cout << "ERROR2 : " << letterPos << " : " << c << endl;

        int range2Add = c != 0 ? alphabetCounters[c - 1] : 0;
        newRangeStart = rankStart + range2Add;
        newRangeEnd = rankEnd + range2Add - 1;
        return 1;//valid input
    }else return 0; //no valid input
}

int backwardsSearchBWT::fowawrdTraversal(int index, int& newRangeStart){
    if (index < 0) return -1;
    // int letterPos = alphabet.select(1, L[index]);
    // if (letterPos != L[index]) cout << "ERROR1 : " << letterPos << " : " << L[index] << endl;
    int symbol = L[index];
    int range2Add = symbol != 0 ? alphabetCounters[symbol - 1] : 0;
    int rangeStart = L.rank(index + 1, symbol) + range2Add - 1;
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

    int low = 0, high = sigma_seperator + 1; // numElems is the size of the array i.e arr.size() 
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
        back_index =  L.select(index + 1, 0/*alphabet[f_index]*/);
    }else{
        //cout << "L posistion: " << L.select(alphabetCounters[f_index] - alphabetCounters[f_index - 1], alphabet[f_index]) << endl;
        back_index = L.select(alphabetCounters[f_index] - (index + 1) == 0 ? alphabetCounters[f_index] - alphabetCounters[f_index - 1] : alphabetCounters[f_index] - (index + 1), f_index/*alphabet[f_index]*/);

    }
    return f_index/*alphabet[f_index]*/;
}

void backwardsSearchBWT::getRange(int c, int& rangeStart, int& rangeEnd){

    //get the starting range in the F array

    rangeStart = -1; rangeEnd = -1;

    if (c > 0){
        if (alphabetCounters[c] - alphabetCounters[c - 1] == 0) 
            return;
    }
    else if (c == 0){
        if (alphabetCounters[0] == 0) 
            return;
    }

    rangeStart = c != 0 ? alphabetCounters[c - 1] : 0;
    rangeEnd = alphabetCounters[c] - 1;
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
        if (item2subst != sigma_seperator) substitutedItems.insert(item2subst);
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
            
            uint64_t quantity = 0;
            std::vector<uint64_t> cs(L.sigma);      // list of characters in the interval
            std::vector<uint64_t> rank_c_i(L.sigma);    // number of occurrence of character in [0 .. i-1]
            std::vector<uint64_t> rank_c_j(L.sigma);    // number of occurrence of character in [0 .. j-1]
            if (rangeStart >= 0 || rangeEnd >= 0) interval_symbols(L, rangeStart, rangeEnd + 1, quantity, cs, rank_c_i, rank_c_j);

            // for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            //     cout << mapIt->second << " ";
            // }
            // cout << endl;
            // cout << " cs: ";
            // for (int it = quantity - 1; it > -1; it--)cout << cs[it] << " ";
            // cout << endl;
            // cout << " map: ";
            // for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) cout << mapIt->first << " ";
            // cout << endl;

            //for (counterMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
            for (int it = quantity - 1; it > -1; it--){
                //if (mapIt->first == 99999) {/*cout << "No 99999 expansion " << endl;*/ continue;}
                if (cs[it] == sigma_seperator){ continue; }
                //cout << mapIt->second << endl;
                xy[index] = cs[it]; //mapIt->first;
                // for (int item : xy) cout << item << " ";
                // cout << ">" << endl;
                if (search(cs[it] /*mapIt->first*/, rangeStart, rangeEnd, newRangeStart, newRangeEnd) == -1) {/*cout << "NOT FOUND" << endl;*/ return;}
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
        if (d == sigma_seperator) {
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
            int symbol = L[i];
            search(symbol, i, i, newRangeStart, newRangeEnd);
            //search(mapIt->second, rangeStart, rangeEnd, newRangeStart, newRangeEnd);
            getConsequents(xy, index + 1, newRangeStart, newRangeEnd, length, /*mapIt->second*/ symbol, consequentList, predictionCount, consequentBits);
        }
    }
    return;
}


//void backwardsSearchBWT::getQuickConsequents(int rangeStart, int rangeEnd, vector<vector<int>>& consequentList, int& predictionCount, sdsl::bit_vector* consequentBits){//size should be over or equal to 2
//    for (int i = rangeStart; i <= rangeEnd; i++){
//        if ((*consequentBits)[i] == 1) continue;
//        (*consequentBits)[i] = 1;
//        vector<int> conseq;
//        int symbol = L[i];
//        if (symbol != sigma_seperator){
//            conseq.push_back(symbol);
//
//           (*consequentBits)[(*LplusOne)[i]] = 1;
//            symbol = L[(*LplusOne)[i]];
//            if (symbol != sigma_seperator) conseq.push_back(symbol);
//            consequentList.push_back(conseq);
//            predictionCount++;
//        }
//    }
//    return;
//}

void backwardsSearchBWT::getQuickConsequents_noLplus(int rangeStart, int rangeEnd, vector<vector<int>>& consequentList, int& predictionCount, sdsl::bit_vector* consequentBits){//size should be over or equal to 2
    for (int i = rangeStart; i <= rangeEnd; i++){
        if ((*consequentBits)[i] == 1) continue;
        (*consequentBits)[i] = 1;
        vector<int> conseq;
        int symbol = L[i];
        if (symbol != sigma_seperator){
            conseq.push_back(symbol);

            int LplusOneIndex = -1;
            fowawrdTraversal(i, LplusOneIndex);
            (*consequentBits)[LplusOneIndex] = 1;
            symbol = L[LplusOneIndex];
            if (symbol != sigma_seperator) conseq.push_back(symbol);
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
    double relay_value = overallRangeLength / (sigma_seperator + 1);
    if (relay_value <= threshold){ // this threshold was based on research that was done in my master thesis
        for (int i = rangeStart; i <= rangeEnd; i++){
            ret = mostFrequent.insert(std::pair<int, int>(L[i], 1));
            if (ret.second == false){
                ret.first->second = ret.first->second + 1;
            }
        }
    }else{
        for (size_t i = 0; i < (sigma_seperator + 1); i++){
            int counter = countRange(rangeStart, rangeEnd, i);
            if (counter > 0) mostFrequent.insert({i, counter});
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
