//
//  rankEvaluation.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 19/10/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "rankEvaluation.h"


using namespace evaluator;

int main (){
    srand (time(NULL));
    
    for (int i = 0; i < 16; i++) set_16distinct.push_back(i);
    for (int i = 0; i < 32; i++) set_20distinct.push_back(i);
    for (int i = 0; i < 64; i++) set_24distinct.push_back(i);
    for (int i = 0; i < 128; i++) set_28distinct.push_back(i);
    for (int i = 0; i < 256; i++) set_32distinct.push_back(i);
    
    int_vector<> set_16distinct_2_16((int)pow(2,16), 1, 0);
    int_vector<> set_20distinct_2_16((int)pow(2,16), 1, 0);
    int_vector<> set_24distinct_2_16((int)pow(2,16), 1, 0);
    int_vector<> set_28distinct_2_16((int)pow(2,16), 1, 0);
    int_vector<> set_32distinct_2_16((int)pow(2,16), 1, 0);
    
    int_vector<> set_16distinct_2_20((int)pow(2,20), 1, 0);
    int_vector<> set_20distinct_2_20((int)pow(2,20), 1, 0);
    int_vector<> set_24distinct_2_20((int)pow(2,20), 1, 0);
    int_vector<> set_28distinct_2_20((int)pow(2,20), 1, 0);
    int_vector<> set_32distinct_2_20((int)pow(2,20), 1, 0);
    
    int_vector<> set_16distinct_2_24((int)pow(2,24), 1, 0);
    int_vector<> set_20distinct_2_24((int)pow(2,24), 1, 0);
    int_vector<> set_24distinct_2_24((int)pow(2,24), 1, 0);
    int_vector<> set_28distinct_2_24((int)pow(2,24), 1, 0);
    int_vector<> set_32distinct_2_24((int)pow(2,24), 1, 0);
    
    
    wt_int<> wt_16distinct_2_16;
    wt_int<> wt_32distinct_2_16;
    wt_int<> wt_64distinct_2_16;
    wt_int<> wt_128distinct_2_16;
    wt_int<> wt_256distinct_2_16;
    
    wt_int<> wt_16distinct_2_20;
    wt_int<> wt_32distinct_2_20;
    wt_int<> wt_64distinct_2_20;
    wt_int<> wt_128distinct_2_20;
    wt_int<> wt_256distinct_2_20;
    
    wt_int<> wt_16distinct_2_24;
    wt_int<> wt_32distinct_2_24;
    wt_int<> wt_64distinct_2_24;
    wt_int<> wt_128distinct_2_24;
    wt_int<> wt_256distinct_2_24;
    
    
    for (int i = 0; i < (int)pow(2,16); i++) {
        set_16distinct_2_16[i] = set_16distinct[rand() % 16];
        set_20distinct_2_16[i] = set_20distinct[rand() % 32];
        set_24distinct_2_16[i] = set_24distinct[rand() % 64];
        set_28distinct_2_16[i] = set_28distinct[rand() % 128];
        set_32distinct_2_16[i] = set_32distinct[rand() % 256];
    }
    
    for (int i = 0; i < (int)pow(2,20); i++) {
        set_16distinct_2_20[i] = set_16distinct[rand() % 16];
        set_20distinct_2_20[i] = set_20distinct[rand() % 32];
        set_24distinct_2_20[i] = set_24distinct[rand() % 64];
        set_28distinct_2_20[i] = set_28distinct[rand() % 128];
        set_32distinct_2_20[i] = set_32distinct[rand() % 256];
    }
    
    for (int i = 0; i < (int)pow(2,24); i++) {
        set_16distinct_2_24[i] = set_16distinct[rand() % 16];
        set_20distinct_2_24[i] = set_20distinct[rand() % 32];
        set_24distinct_2_24[i] = set_24distinct[rand() % 64];
        set_28distinct_2_24[i] = set_28distinct[rand() % 128];
        set_32distinct_2_24[i] = set_32distinct[rand() % 256];
    }
    
    
    store_to_file(set_16distinct_2_16, "sdb.sdsl");
    construct(wt_16distinct_2_16, "sdb.sdsl", 0);
    store_to_file(set_20distinct_2_16, "sdb.sdsl");
    construct(wt_32distinct_2_16, "sdb.sdsl", 0);
    store_to_file(set_24distinct_2_16, "sdb.sdsl");
    construct(wt_64distinct_2_16, "sdb.sdsl", 0);
    store_to_file(set_28distinct_2_16, "sdb.sdsl");
    construct(wt_128distinct_2_16, "sdb.sdsl", 0);
    store_to_file(set_32distinct_2_16, "sdb.sdsl");
    construct(wt_256distinct_2_16, "sdb.sdsl", 0);
    
    
    store_to_file(set_16distinct_2_20, "sdb.sdsl");
    construct(wt_16distinct_2_20, "sdb.sdsl", 0);
    store_to_file(set_20distinct_2_20, "sdb.sdsl");
    construct(wt_32distinct_2_20, "sdb.sdsl", 0);
    store_to_file(set_24distinct_2_20, "sdb.sdsl");
    construct(wt_64distinct_2_20, "sdb.sdsl", 0);
    store_to_file(set_28distinct_2_20, "sdb.sdsl");
    construct(wt_128distinct_2_20, "sdb.sdsl", 0);
    store_to_file(set_32distinct_2_20, "sdb.sdsl");
    construct(wt_256distinct_2_20, "sdb.sdsl", 0);
    
    store_to_file(set_16distinct_2_24, "sdb.sdsl");
    construct(wt_16distinct_2_24, "sdb.sdsl", 0);
    store_to_file(set_20distinct_2_24, "sdb.sdsl");
    construct(wt_32distinct_2_24, "sdb.sdsl", 0);
    store_to_file(set_24distinct_2_24, "sdb.sdsl");
    construct(wt_64distinct_2_24, "sdb.sdsl", 0);
    store_to_file(set_28distinct_2_24, "sdb.sdsl");
    construct(wt_128distinct_2_24, "sdb.sdsl", 0);
    store_to_file(set_32distinct_2_24, "sdb.sdsl");
    construct(wt_256distinct_2_24, "sdb.sdsl", 0);
    
    ofstream outputFile;
    
    int res = access("Rank_Timing_results.csv", R_OK);
    if (res < 0) {
        if (errno == ENOENT) {
            // file does not exist
            outputFile.open("Rank_Timing_results.csv");
            outputFile << "WT size,1000 random rank accesses (s)" << endl;
        }
    }else{
        outputFile.open("Rank_Timing_results.csv", ios_base::app);
    }
    
    auto begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_16distinct_2_16.rank(rand() % (int)pow(2, 16), rand() % 16);
        }
    }
     auto end = clock();
     auto duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_16distinct_2_16," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_32distinct_2_16.rank(rand() % (int)pow(2, 16), rand() % 32);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_32distinct_2_16," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_64distinct_2_16.rank(rand() % (int)pow(2, 16), rand() % 64);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_64distinct_2_16," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_128distinct_2_16.rank(rand() % (int)pow(2, 16), rand() % 128);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_128distinct_2_16," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_256distinct_2_16.rank(rand() % (int)pow(2, 16), rand() % 256);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_256distinct_2_16," <<duration/1000 << endl;
    
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_16distinct_2_20.rank(rand() % (int)pow(2, 20), rand() % 16);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_16distinct_2_20," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_32distinct_2_20.rank(rand() % (int)pow(2, 20), rand() % 32);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_32distinct_2_20," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_64distinct_2_20.rank(rand() % (int)pow(2, 20), rand() % 64);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_64distinct_2_20," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_128distinct_2_20.rank(rand() % (int)pow(2, 20), rand() % 128);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_128distinct_2_20," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_256distinct_2_20.rank(rand() % (int)pow(2, 20), rand() % 256);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_256distinct_2_20," <<duration/1000 << endl;
    
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_16distinct_2_24.rank(rand() % (int)pow(2, 24), rand() % 16);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_16distinct_2_24," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_32distinct_2_24.rank(rand() % (int)pow(2, 24), rand() % 32);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_32distinct_2_24," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_64distinct_2_24.rank(rand() % (int)pow(2, 24), rand() % 64);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_64distinct_2_24," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_128distinct_2_24.rank(rand() % (int)pow(2, 24), rand() % 128);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_128distinct_2_24," <<duration/1000 << endl;
    
     begin = clock();
    for (int i = 0; i <1000; i++) {
        for (int j = 0; j < 1000; j++) {
            wt_256distinct_2_24.rank(rand() % (int)pow(2, 24), rand() % 256);
        }
    }
     end = clock();
     duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
    outputFile << "wt_256distinct_2_24," <<duration/1000 << endl;
    
    outputFile.close();
    
    
    
    return 0;
}