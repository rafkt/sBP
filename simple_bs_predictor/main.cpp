//
//  main.cpp
//  countBefore
//
//  Created by Rafael Ktistakis on 20/06/2014.
//  Copyright (c) 2014 Rafael Ktistakis. All rights reserved.
//

#include "subseqPredictor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <time.h>
#include <assert.h>

#include <sys/time.h>
#include <time.h>

using namespace std;

vector<vector<int>> testQueries;
vector<int> testAnswer;

typedef unsigned long long timestamp_t;

static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

void createTestQueries(string filename){
    
    ofstream outputFile;
    
    outputFile.open(filename + "_Ran_queries.txt",ios::out);
    
    vector<int> T, query;
    ifstream file(filename);
    srand(time(NULL));
    int r1;
    for(string word; file >> word; ) T.push_back(atoi(word.c_str()));
    for (int i = 0; i < 1000; i++) {
        bool flag = false;
        int answer = -1;
        do{
            r1 = rand() % (T.size() - 11);
        }while (T[r1] == 99999 || T[r1 + 1] == 99999 || T[r1 + 2] == 99999 || T[r1 + 3] == 99999); //|| T[r1 + 4] == 99999 || T[r1 + 5] == 99999|| T[r1 + 6] == 99999 || T[r1 + 7] == 99999);
        int r2 = rand() % 5 + 3;
        for (int j = r1; j <= r1 + r2 ; j++) {
            if (T[j] != 99999 && j != 8) query.push_back(T[j]);
            else{
                int tmp = query[query.size() - 1];
                query.pop_back();
                testQueries.push_back(query);
                //cout << query << endl;
                outputFile << query << endl;
                testAnswer.push_back(tmp);
                query.clear();
                flag = true;
                break;
            }
            answer = T[j + 1];
        }
        if (!flag){
            if (answer != 99999){
                testAnswer.push_back(answer);
            }else{
                testAnswer.push_back(-1);
            }
            //cout << query << endl;
            testQueries.push_back(query);
            outputFile << query << endl;
            query.clear();
        }
    }
    outputFile.close();
    file.close();
}
vector<vector<int>> readTestQueries(string filename){
    ifstream infile(filename);
    vector<int> query;
    vector<vector<int>> sequenceCollection;
    for(string line; getline(infile, line ); ){
        stringstream ss(line);
        string result;
        while (getline(ss, result, ' ')) {
            // if (atoi(result.c_str()) == 99999){
            //     // for (int i : query) cout << i << " ";
            //     // cout << endl;
            //     sequenceCollection.push_back(query);
            //     query.clear();
            //     continue;
            // }
            query.push_back(atoi(result.c_str()));
        }
        sequenceCollection.push_back(query);
        query.clear();
    }
    return sequenceCollection;
}

int main(int argc, const char * argv[])
{
//    countBefore c(argv[1]);
//    cout << c.countBeforeChar(61, 58) << endl;
//    vector<int> x = {105, 7};
//    reverse(x.begin(), x.end()); //query should be given reversed since we want to count after.
//    cout << c.countBeforeString(1000, &x[0], x.size()) << endl;

    
    vector<vector<int>> testQueries = readTestQueries(argv[2]);
    subseqPredictor* pr = new subseqPredictor(argv[1]);

    //auto begin = clock();
    for(vector<int> query : testQueries){
       // if (query.size() < 2) {cout << "short query; do we except this?" << endl; continue;}
        // int size = query.size() - 1;// our offset is 1 for now
        // vector<int> finalQuery;
        // if (5 > size){ //our query length should be 5 for now
        //     vector<int> q(query.begin(), query.begin() + size);
        //     finalQuery = q;
        // }else{
        //     vector<int> q(query.begin() + (size - 5), query.begin() + size);
        //     finalQuery = q;
        // }
        // for (int i : finalQuery) cout << i << " ";
        // cout << endl;
        cout << pr->start(&query[0], query.size()) << endl;
        //break;
    }
    // auto end = clock();
    // auto duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    // cout << argv[1] << endl;
    // cout << "Memory: " << pr->get_memory() << endl;
    // cout << "Duration: " << duration << endl;

   //vector<int> x = {1, 2};//FIFA: 114 133 148 212 256 300 582 610 - 114 133 148 212 256 300 582 610 30 626 647 99999
   //Answer 30
   
   
   // for (aMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
   //     cout << mapIt->first << " " << mapIt->second << endl;
   // }
	
    
//    vector<int> x = { 131, 70  };//FIFA: 114 133 148 212 256 300 582 610 - 114 133 148 212 256 300 582 610 30 626 647 99999
//    //Answer 30
//    predictor* pr = new predictor(argv[1]);
//    int res = pr->predict(&x[0], x.size());
//    cout << res << endl;
    
//    x = { 30, 10, 147, 131, 135 };//FIFA: 114 133 148 212 256 300 582 610 - 114 133 148 212 256 300 582 610 30 626 647 99999
//    //Answer 30
//    pr = new predictor(argv[1]);
//    res = pr->predict(&x[0], x.size());
//    cout << res << endl;
    
    //createTestQueries(argv[1]);
 //    readTestQueries(argv[1]);
    
 //    int queries_length = 0;
 //    for (int i = 0; i < testQueries.size(); i++) queries_length += testQueries[i].size();
    
 //    double currentProb = 0, maxProb = 0, averageProb = 0;
 //    int overallResults = 0;
    
 //    predictor* pr = new predictor(argv[1], false, true);
	
	// //k_folder* evaluator = new k_folder(argv[1], 10);
    
 //    ofstream outputFile;
    
 //    int res = access("Timing_results.txt", R_OK);
 //    if (res < 0) {
 //        if (errno == ENOENT) {
 //            // file does not exist
 //            outputFile.open("Timing_results.txt");
 //        }
 //    }else{
 //        outputFile.open("Timing_results.txt", ios_base::app);
 //    }
    
    
 //    int queriesNum = 0;
 //    double average_query_length = queries_length / testQueries.size();
 //    auto begin = clock();
 //    uint32_t iterations = 1;
 //    for(uint32_t i = 0; i < iterations; ++i)
 //    {
 //        //if (i % 25 == 0) cout << "iteration: " << i << endl;
 //        for (int i = 0; i < testQueries.size(); i++) {
 //            aMap res = pr->predict(&testQueries[i][0], testQueries[i].size(), 10);
 //            if (res.empty()) {
 //                //cout << testQueries[i] << endl;
 //                continue;
 //            }
 //            overallResults = res.begin()->second;
 //            //for (aMap::iterator mapIt = res.begin(); mapIt != res.end(); mapIt++) {
 //                //cout << mapIt->first << " " << mapIt->second << endl;
 //                currentProb = (--(res.end()))->first;
	// 		cout << currentProb << endl;
 //            //}
 //            //cout << currentProb << " " << res.begin()->second << endl;
 //            currentProb = currentProb / (double) overallResults;
 //            averageProb += currentProb;
 //            maxProb = currentProb > maxProb ? currentProb : maxProb;
 //            res.erase(res.begin(), res.end());
 //            queriesNum++;
 //        }
 //    }
 //    auto end = clock();
 //    auto duration = ((double)(end - begin))/CLOCKS_PER_SEC;
    
 //    outputFile << "Filename: " << argv[1] << endl;
 //    outputFile << "m: " << pr->bSBWT->L.size() << endl;
 //    outputFile << "σ: " << pr->bSBWT->alphabet.size() << endl;
 //    outputFile << "Memory Size: " << size_in_mega_bytes(pr->bSBWT->L) + size_in_mega_bytes(pr->bSBWT->alphabetCounters) + size_in_mega_bytes(pr->bSBWT->alphabet) << endl;
 //    outputFile << "max Propability: " << maxProb << ", average Propability: " << averageProb/ (double) queriesNum << endl;
 //    outputFile << duration << "s total, average: " << duration / iterations << "s. Average per query: " << (duration / (double)iterations)/testQueries.size() <<  "s. Average query length: " << average_query_length << endl;
    
 //    outputFile.close();
 //   cout << "Done!\n";
    
    delete pr;
	//delete evaluator;
    return 0;


}