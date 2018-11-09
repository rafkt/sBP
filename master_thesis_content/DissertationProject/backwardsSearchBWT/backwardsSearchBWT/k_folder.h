//
//  k-folder.h
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 19/02/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>


using namespace std;

typedef map<int, vector<vector<int>>> foldList;

class k_folder{
public:
	k_folder(string, int, int, int, int);
	vector<vector<vector<int>>> getQueries();
	vector<vector<vector<int>>> getSuffixes();
	void evaluateFormula(vector<vector<int>>, vector<vector<int>>, int, int, vector<int>&);
	void gameWithCachedPredictions(vector<vector<int>>, vector<vector<int>>, int, int, int, vector<double>&);
	void gameWithCachedPredictionsV2(vector<vector<int>>, vector<vector<int>>, int, int, int, vector<int>&);
	void pureCacheMechanismGame(vector<vector<int>>, int, int, vector<int>&);
	vector<int> evaluator(vector<vector<int>>, vector<int> (*)(vector<vector<int>>, vector<vector<int>>, int, int));
	vector<string> getTrainingPaths();
	string split();
	~k_folder();
private:
	string datasetFile, trainingFile;
	vector<vector<int>> queriesSuffixes, datasetSequences;
	vector<int> foldedDataset, restDataset, dataset;
	ifstream dataseFileStream, trainingFileStream;
	
	vector<vector<vector<int>>> queries, suffixes;
	vector<string> trainingPaths;
	foldList folds;
	int k, prefixSize, suffixSize, maxSequences;
};