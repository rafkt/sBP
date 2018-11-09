//
//  k-folder.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 19/02/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include "k_folder.h"
#include <assert.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "LRUcache.h"

#define noSplitSeq true
#define writeQueriesToFile true



k_folder::k_folder(string filename, int maxSequences, int k, int prefixSize, int suffixSize): trainingFile("splitDataset.txt"), k(k), prefixSize(prefixSize), suffixSize(suffixSize), maxSequences(maxSequences){
	ifstream file(filename);
	
	for(string word; file >> word;) dataset.push_back(atoi(word.c_str()));
	split();
	file.close();
}

string k_folder::split(){
	srand (time(NULL));
	//ofstream file(trainingFile);
	
	
//	for (int i = 0; i < foldedDataset.size(); i++) {
//		file << foldedDataset[i] << (i == foldedDataset.size() - 1 ? "" : " ");
//	}
	int counter = 0;
	
	datasetSequences.push_back(vector<int>());
	for (int i = 0; i < dataset.size(); i++) {
		datasetSequences[counter].push_back(dataset[i]);
		if (dataset[i] == 99999) {
			datasetSequences.push_back(vector<int>());
			counter++;
		}
		if (counter >= maxSequences) break;
	}
	
	for (int i = 0; i < k; i++) folds.insert({i, vector<vector<int>>()});
	
	int foldsCounter = 0, queriesSeen = 1;
	int queriesPerFold = datasetSequences.size() / k;
	bool increaseFolds = false;
	while (!datasetSequences.empty()) {
		int randomIndex = rand() % datasetSequences.size();
		folds.at(foldsCounter).push_back(datasetSequences[randomIndex]);
		datasetSequences.erase(datasetSequences.begin() + randomIndex);
		if (queriesSeen == queriesPerFold){
			foldsCounter++;
			queriesSeen = 0;
		}
		if (foldsCounter == k) {
			foldsCounter = 0;
			increaseFolds = true;
			queriesSeen++;
			continue;
		}
		queriesSeen++;
		if (increaseFolds) foldsCounter++;
	}
	
	
	for (foldList::iterator it = folds.begin(); it != folds.end(); it++) {
		stringstream sstm;
		sstm << "trainingFile" << it->first << ".txt";
		trainingPaths.push_back(sstm.str());
		sstm.str("");
	}

	for(int i = 0; i < trainingPaths.size(); i++){ //for every fold - left this fold out - write the rest to a training file
		//open file stream
		ofstream file(trainingPaths[i]);
		for(foldList::iterator it = folds.begin(); it != folds.end(); it++){
			//write this fold to the open file - if fold is i, don't write it
			if (it->first != i){
				for (int j = 0; j < it->second.size(); j++) {
					for (int l = 0; l < it->second[j].size(); l++) {
						file << it->second[j][l] << " ";
					}
				}
			}
		}
		//close stream
		file.close();
	}
	exit(0);
	//training set k should contain all folds except k-th fold which will be used for queries
	
	//setting up queries & suffixes for each k-training set.
	vector<int> temp_q, temp_s;
	for (foldList::iterator it = folds.begin(); it != folds.end(); it++) {
		vector<vector<int>> temp_arr_q, temp_arr_s;
		for (int i = 0; i < it->second.size(); i++) {
			if (noSplitSeq) {
				temp_arr_q.push_back(vector<int>(it->second[i].begin(), it->second[i].end()));
				continue;
			}
			if (it->second[i].size() < 3 || prefixSize + suffixSize > it->second[i].size()) continue;
			else{
				//we keep that sequence discaring the first size - prefixSize - suffixSize items (Context)
				temp_q = it->second[i];
				temp_q.erase(temp_q.begin(), temp_q.begin() + temp_q.size() - prefixSize - suffixSize);//get query and discard context
				temp_s = vector<int>(temp_q.begin() + prefixSize, temp_q.end());//copy suffix to another array
				temp_q.erase(temp_q.begin() + prefixSize, temp_q.end());//discard that suffix from query body
				temp_arr_q.push_back(temp_q);
				temp_arr_s.push_back(temp_s);
				temp_q.erase(temp_q.begin(), temp_q.end());
				temp_s.erase(temp_s.begin(), temp_s.end());
			}
		}
		if (noSplitSeq){
			queries.push_back(vector<vector<int>>(temp_arr_q.begin(), temp_arr_q.end()));
			temp_arr_q.erase(temp_arr_q.begin(), temp_arr_q.end());
			if (writeQueriesToFile){
				for(int i = 0; i < queries.size(); i++){
					stringstream sstm;
					sstm << "queries_fold_" << i << ".txt";
					ofstream file(sstm.str());
					for (int j = 0; j < queries[i].size(); j++) {
						for (int l = 0; l < queries[i][j].size(); l++) {
							file << queries[i][j][l] << " ";
						}
					}
					//close stream
					file.close();
				}
			}
			continue;
		}
		queries.push_back(temp_arr_q);
		suffixes.push_back(temp_arr_s);
		temp_arr_q.erase(temp_arr_q.begin(), temp_arr_q.end());
		temp_arr_s.erase(temp_arr_s.begin(), temp_arr_s.end());
	}
	
	//file.close();
	return trainingFile;
}

vector<vector<vector<int>>> k_folder::getQueries(){
	return queries;
}

vector<vector<vector<int>>> k_folder::getSuffixes(){
	return suffixes;
}

vector<string> k_folder::getTrainingPaths(){
	return trainingPaths;
}

void k_folder::evaluateFormula(vector<vector<int>> predictions, vector<vector<int>> suffixes, int order, int use_predictions, vector<int>& scores){
	int tmp_order = order;
	assert(predictions.size() == suffixes.size());
	for (int i = 0; i < predictions.size(); i++) {
		order = tmp_order;
		bool flag = false;
		while (order + 1 > suffixes[i].size()) order--;
		for (int j = 0; j < predictions[i].size(); j++) {
			if (j >= use_predictions) break;
			if (find(suffixes[i].begin(), suffixes[i].begin() + order, predictions[i][j]) != suffixes[i].begin() + order) {
				if (!flag){
					flag = true;
					scores.push_back(1);
				}else scores[scores.size() - 1] += 1;
			}else {
				if (!flag){
					flag = true;
					scores.push_back(0);
				}
			}
		}
	}
	assert(scores.size() == predictions.size()); //this assertion actually fails since this function is called for all the available folds passing
	//as a reference the same score table...
}

/*
 ---------- Rules of the game --------
 
 The game is based on the web server-client communication. When a client asks for a page, the server either has to retrieve the page from a disk and pay a latency of 10ms (for example) to access the disk or has to retrieve the page from a cached list stored to a RAM ! The second is considered much faster.
 
 So, for every page that is asked by the client, server keeps track of clients logs and asks a predictor for a prediction. This prediction would be a possible next page that will be asked from the client. So, every prediction is kept in a cache of a specific size (say N pages) by the server.
 
 If a prediction is successful (meaning that this prediction actually used as a next page to be visited by the client) then the predictor gets 10 points (representing the ms that the server didn't pay for accessing its hard disk).
 So for a given prediction, if the current prediction or any old prediction contained in the cache is representing the actual outcome then the predictor always gets 10 points. If there is not such a prediction in the cache or currently then the predictor loses 5 points.
 Every prediction that is contained in the cache is marked as paid or not paid. A paid/unpaid prediction represents if at current time the predictor lost 5 points or was awarded points for its prediction at current/past time.
 
 For any given prediction to the server, if this prediction is used immediately then predictor gets 10 points and this prediction is marked as paid. If any past prediction is used and that past prediction was unpaid then the predictor gets 15 points. If that past prediction was paid then the predictor gets the usual 10 points. If any prediction is used then the current prediction is marked as paid too.
 
 In case that the predictor does not give any prediction at all, then it loses 10 points.
 
 
 */

void k_folder::gameWithCachedPredictions(vector<vector<int>> predictions, vector<vector<int>> suffixes, int order, int use_predictions, int cacheSize, vector<double>& points){
	
	int target_points = 0; //points that could have been achieved if everything was right!
	vector<int> paidPredictions(predictions.size(), 0);
	int foldPoints = 0;
	
	int tmp_order = order;
	for (int i = 0; i < predictions.size(); i++) {
		target_points += 10;
		order = tmp_order;
		bool flag = false;
		while (order + 1 > suffixes[i].size()) order--;
		for (int j = 0; j < predictions[i].size(); j++) {
			if (predictions[i][j] == -1) {//no prediction made
				foldPoints -= 10;
				break; // or even continue since we should not have any other prediction waiting!
			}
			if (j >= use_predictions) break;
			if (find(suffixes[i].begin(), suffixes[i].begin() + order, predictions[i][j]) != suffixes[i].begin() + order) {//current prediction was successfull.
				if (!flag){
					flag = true;
					foldPoints += 10;
					paidPredictions[i] = 1;
					break;
				}
			}else { //current prediction un-successfull. Start looking for older predictions in cache. See if any of them succeeds
				int tmp = cacheSize;
				int cacheIndex = i - 1;
				while (tmp > 0 && cacheIndex >= 0) {
					if (find(suffixes[i].begin(), suffixes[i].begin() + order, predictions[cacheIndex][j]) != suffixes[i].begin() + order) { //found old prediction contained in cache that succeeds.
						if (!flag){
							flag = true;
							if (paidPredictions[cacheIndex] == 1) foldPoints += 10;
							else{
								paidPredictions[cacheIndex] = 1;
								foldPoints += 15;
							}
							paidPredictions[i] = 1;
							break;
						}
					}
					cacheIndex--;
					tmp--;
				}
				if (!flag && j == use_predictions - 1){ // we detuct points only if we have checked all choises contained in a prediction
					flag = true;
					foldPoints -= 5;
				}
			}
		}
	}
	points.push_back((foldPoints < 0 ? ((foldPoints - target_points) / (double)target_points) * 100 : (foldPoints / (double)target_points) * 100));
	
	//cout << points[points.size() - 1] << endl;
	//cout << "Target: " << target_points << " Got: " << foldPoints << endl;
}

/*
 See document on SVN for detailed description of this specific game - In some words, it constitues a small varation of the above game. Here a more complete cache (LRU) bahaviour is simulated.
 */

void k_folder::gameWithCachedPredictionsV2(vector<vector<int>> predictions, vector<vector<int>> suffixes, int order, int use_predictions, int cacheSize, vector<int>& points){
	
	LRUcache serverCache(cacheSize);
	int foldPoints = 0;
	
	int tmp_order = order;
	for (int i = 0; i < predictions.size(); i++) {
		order = tmp_order;
		while (order + 1 > suffixes[i].size()) order--;
		for (int j = 0; j < predictions[i].size(); j++) {
			if (j >= use_predictions) break;
			bool predictionWasInCache = false;
			if (predictions[i][j] != -1) predictionWasInCache = serverCache.get(predictions[i][j]); // if a prediction made
			if (!predictionWasInCache) foldPoints--;
			if (j < use_predictions - 1) continue; // after all predictions for this sequence has been imported in the cache, then we continue with requests.
			bool requestWasInCache = false;
			for (int o = 0; o < order; o++) {
				requestWasInCache = serverCache.get(suffixes[i][o]);
				if (requestWasInCache) {
					foldPoints++; //benefit
				}else{
					foldPoints--;//cost
				}
			}
		}
	}
	points.push_back(foldPoints);
	
	//cout << points[points.size() - 1] << endl;
	//cout << "Target: " << target_points << " Got: " << foldPoints << endl;
}

void k_folder::pureCacheMechanismGame(vector<vector<int>> suffixes, int order, int cacheSize, vector<int>& points){
	LRUcache serverCache(cacheSize);
	int foldPoints = 0;
	
	int tmp_order = order;
	for (int i = 0; i < suffixes.size(); i++) {
		order = tmp_order;
		while (order + 1 > suffixes[i].size()) order--;
		for (int j = 0; j < order; j++) {
			bool requestWasInCache = serverCache.get(suffixes[i][j]);
			if (requestWasInCache) {
				foldPoints++; //benefit
			}else{
				foldPoints--;//cost
			}
		}
	}
	points.push_back(foldPoints);
	
	//cout << points[points.size() - 1] << endl;
	//cout << "Target: " << target_points << " Got: " << foldPoints << endl;
}


vector<int> k_folder::evaluator(vector<vector<int>>predictions, vector<int> (*evaluateFormula)(vector<vector<int>>, vector<vector<int>>, int, int)){
	vector<int> scores;
	return scores;
}

k_folder::~k_folder(){
	for(int i = 0; i < trainingPaths.size(); i++)remove(trainingPaths[i].c_str());
}
