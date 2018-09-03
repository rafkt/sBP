//
//  suffixArray.cpp
//  suffixArray
//
//  Created by Rafael Ktistakis on 22/07/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "suffixArray.h"


vector<int> suffixArray::sa;
vector<int> suffixArray::T;

void suffixArray::clearALL(){
    deleteCreatedFiles();
    sa.erase(sa.begin(), sa.end());
    T.erase(T.begin(), T.end());
}

void suffixArray::clean_myfile() {
    remove( "sdb.sdsl" );
    remove( "tmp.txt" );
}

void suffixArray::deleteCreatedFiles(){
    ofstream output("sdb.sdsl", std::ios::binary | std::ios::trunc);
    atexit(clean_myfile);
}

//old compare function for suffix array sort - commented for debugging - delete if bug is found.
//int suffixArray::compare(const void* a, const void* b){
//    if (T[*(int*)a] < T[*(int*)b]) return -1;
//    else if (T[*(int*)a] > T[*(int*)b]) return 1;
//    else{
//        if (*(int*)a + 1 > T.size()) return 1;
//        else if (*(int*)b + 1 > T.size()) return -1;
//        else{
//            int new_a, new_b;
//            new_a = *(int*)a + 1;
//            new_b = *(int*)b + 1;
//            return compare(&new_a, &new_b);
//        }
//        
//    }
//    return 0;
//}

int suffixArray::compare(const void* a, const void* b){
    if (T[*(int*)a] < T[*(int*)b]) return -1;
    else if (T[*(int*)a] > T[*(int*)b]) return 1;
    else{
        if (*(int*)a + 1 >= T.size()){
            int new_a, new_b;
            new_a = T.size() - (*(int*)a) - 1;
            new_b = *(int*)b + 1;
            return compare(&new_a, &new_b);
        }
        else if (*(int*)b + 1 >= T.size()){
            int new_a, new_b;
            new_a = *(int*)a + 1;
            new_b = T.size() - (*(int*)b) - 1;
            return compare(&new_a, &new_b);
        }
        else{
            int new_a, new_b;
            new_a = *(int*)a + 1;
            new_b = *(int*)b + 1;
            return compare(&new_a, &new_b);
        }
        
    }
    return 0;
}


int suffixArray::initialise(ifstream &file, int k_fold){ // returns the number of sequences that was red
	int seqCounter(0);
	for(string word; file >> word;) {
		T.push_back(atoi(word.c_str()));
		if (atoi(word.c_str()) == 99999) seqCounter++;
	}
// code used for k-folding
//    int s_counter = 0;
//    for (int i = 0; i < T.size(); i++) if (T[i] == 99999) s_counter++;
//    if (k_fold != -1) {
//        int trainingSequences = 0;
//        trainingSequences = s_counter/k_fold;
//        trainingSequences *= (k_fold - 1);
//        s_counter = 0;
//        for (int i = 0; i < T.size(); i++) if (T[i] == 99999){
//            s_counter++;
//            if (s_counter == trainingSequences) {
//                T.erase(T.begin() + i + 1, T.end());
//            }
//        }
//    }
    //k-folding ends here ->incomplete though
    
    //reverse here T
    reverse(T.begin(), T.end());
    for (int i = 0; i < T.size(); i++) {
        sa.push_back(i);
    }
    qsort (&sa[0], sa.size(), sizeof(int), compare);
    file.close();
	return seqCounter;
}

int suffixArray::getBWT(int i){
    return sa[i] != 0 ? T[sa[i] - 1] : T[T.size() - 1];
}

wt_int<> suffixArray::BWT2WT(){
    wt_int<> wt;
    int_vector<> v(sa.size(), 1, 0);
    
    for (int i = 0; i < sa.size(); i++) {
        v[i] = getBWT(i);
    }
    store_to_file(v, "sdb.sdsl");
    util::clear(v);
    construct(wt, "sdb.sdsl", 0);
    return wt;
}