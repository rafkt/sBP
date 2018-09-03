//
//  k-folder_evaluator_run.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 25/02/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#define CACHE true

#include "predictor.h"
#include <stdio.h>
#include "k_folder.h"
#include <assert.h>
#include <math.h>
#include "LRUcache.h"
#include "csvProducer.h"

using namespace std;

vector<int> numOfAnswers, numOfDifferents;
vector<double> percentages;

LRUcache serverCache(100), simpleCache(100);

string datasetName;

vector<string> inline StringSplit(const string &source, const char *delimiter = " ", bool keepEmpty = false)
{
	vector<string> results;
	
	size_t prev = 0;
	size_t next = 0;
	
	while ((next = source.find_first_of(delimiter, prev)) != std::string::npos)
	{
		if (keepEmpty || (next - prev != 0))
		{
			results.push_back(source.substr(prev, next - prev));
		}
		prev = next + 1;
	}
	
	if (prev < source.size())
	{
		results.push_back(source.substr(prev));
	}
	
	return results;
}

double mean(vector<int> array){
	return accumulate(array.begin(), array.end(), 0.0) / (double) array.size();
}

double mean(vector<double> array){
	return accumulate(array.begin(), array.end(), 0.0) / (double) array.size();
}

double median (vector<int> array){
	return array.size() % 2 == 0 ? (array[array.size() / 2 - 1] + array[array.size() / 2]) / 2.0 : array[array.size() / 2];
}

vector<int> roundVector (vector<double> array){
	vector<int> rounded;
	for (int i = 0; i < array.size(); i ++) {
		double remaining = fmod(array[i], 10.0);
		if (remaining >= 5.0) rounded.push_back(((int)(array[i] / 10)) * 10 + 10);
		else rounded.push_back(((int)(array[i] / 10)) * 10);
	}
	return rounded;
}

int mode(vector<int> array){
	if (array.size() == 0) return -1;
	sort(array.begin(), array.end());
	int counter = 1, max = 1, number = array[0];
	
	for (int i = 1; i < array.size(); i++) {
		if (array[i] != array[i - 1]) {
			if (counter >= max) {
				max = counter;
				counter = 1;
				number = array[i - 1];
			}else counter = 1;
		}else counter++;
	}
	return counter > max ? array[array.size() - 1] : number;
}

int range(vector<int> array){
	if (array.size() == 0) return -1;
	sort(array.begin(), array.end());
	return array[array.size() - 1] - array[0];
}

double variance (vector<int> array){ // it illustrates how far are the values from their mean value. The higher the variance the more far is each value from its mean value
	unsigned long int sum = 0;
	double meanValue = mean(array);
	for (int i = 0; i < array.size(); i++) sum += pow(array[i] - meanValue, 2.0);
	return sum / (double)(array.size() - 1);
}

double cvValue(vector<int> array){ //the smaller the CV value is the less each value varies with the other values of our population
	return sqrt(variance(array)) / mean(array);
}

void multiplePredictionsPerTestingSequence(csvProducer& stats, int history, int cacheSize, int minRecursion, int maxRecursion, vector<predictor*> pr, vector<vector<vector<int>>> q, vector<vector<vector<int>>> s){
	
	
	vector<double> queriesScore;
	vector<int> predictionCacheCostArr, predictionCacheBenefitArr, simpleCostArr, simpleBenefitArr, noCacheCostArr;
	
	for (int i = 0; i < q.size(); i++) { // every fold
		for (int j = 0; j < q[i].size(); j++) { // every query
			int score = 0, predictionCacheCost = 0, predictionCacheBenefit = 0, simpleCost = 0, simpleBenefit = 0, noCacheCost = 0;
			if ((q[i][j].size()) < 3) continue;// query of length one cannot be used for doing predictions (including terminating item we have two items)
			//LRUcache serverCache(cacheSize), simpleCache(cacheSize);
			for (int k = 0; k < q[i][j].size(); k++) {
				if (k == q[i][j].size() - 2) break;
				aMap res;
				if (k + 1 >= history) {
					res = pr[i]->predictWithRecursiveDivider(&q[i][j][k - history + 1], history, 1, minRecursion, maxRecursion);
				}else{
					res = pr[i]->predictWithRecursiveDivider(&q[i][j][0], k + 1, 1, minRecursion, maxRecursion);
				}
				aMap::reverse_iterator mapIt;
				int currentPrediction = -1;
				double currentSupport = -1;
				if (res.size() > 0){
					mapIt = res.rbegin();
					currentPrediction = mapIt->second;
					
					mapIt = res.rend();
					mapIt--;
					currentSupport = mapIt->second / (double) pr[i]->datasetSeqNumber();
				}
				if (cacheSize > 0) {
					if (res.size() > 0){
						if (currentSupport > 0.05){ //if (pr[i]->itemConfidence(currentPrediction) >= 0.1){
							bool predictionInCache = false;
							predictionInCache = serverCache.get(currentPrediction);
							if (!predictionInCache) predictionCacheCost++;
						}
					}
					bool realRequestInCache = false;
					realRequestInCache = serverCache.get(q[i][j][k + 1]);
					if (realRequestInCache) predictionCacheBenefit++;
					else predictionCacheCost++;
					realRequestInCache = simpleCache.get(q[i][j][k + 1]);
					if (realRequestInCache) simpleBenefit++;
					else simpleCost++;
					noCacheCost++;
				}else{
					if (res.size() > 0) if (currentPrediction == q[i][j][k + 1]) score ++;
				}
			}
			if ((q[i][j].size() - 1) == 0) continue;// query of length one cannot be used for doing predictions
			if (cacheSize > 0) {
				predictionCacheCostArr.push_back(predictionCacheCost);
				predictionCacheBenefitArr.push_back(predictionCacheBenefit);
				simpleCostArr.push_back(simpleCost);
				simpleBenefitArr.push_back(simpleBenefit);
				noCacheCostArr.push_back(noCacheCost);
			}else{
				double average = ((score / (double) (q[i][j].size() - 1)) * 100);
				queriesScore.push_back(average);
			}
		}
	}
	
	ostringstream strs;
	stats.addCollumn("Predictor");
	stats.addToCollumn("Predictor", "BWT");
	stats.addCollumn("Dataset");
	vector<string> results = StringSplit(datasetName, "/");
	stats.addToCollumn("Dataset", results[results.size() - 1]);
	stats.addCollumn("History Size");
	stats.addToCollumn("History Size", to_string(history));
	stats.addCollumn("Cache Size");
	stats.addToCollumn("Cache Size", to_string(cacheSize));
	if (cacheSize > 0) {
		
		stats.addCollumn("Prediction with Cache Cost");
		stats.addToCollumn("Prediction with Cache Cost", to_string(mean(predictionCacheCostArr)));
		
		stats.addCollumn("Prediction with Cache Benefit");
		stats.addToCollumn("Prediction with Cache Benefit", to_string(mean(predictionCacheBenefitArr)));
		
		stats.addCollumn("Simple LRU Cache Cost");
		stats.addToCollumn("Simple LRU Cache Cost", to_string(mean(simpleCostArr)));
		
		stats.addCollumn("Simple LRU Cache Benefit");
		stats.addToCollumn("Simple LRU Cache Benefit", to_string(mean(simpleBenefitArr)));
		
		stats.addCollumn("No Cache Cost");
		stats.addToCollumn("No Cache Cost", to_string(mean(noCacheCostArr)));
		
	}else{
		stats.addCollumn("Accuracy");
		stats.addToCollumn("Accuracy", to_string(mean(queriesScore)));
	}
}

void executeEvaluation (int maxSequences, int minRecursion, int maxRecursion, int& queriesNum, int& predictionsAch, int& succPredicts, double& ratioOfSuffixAppearedInAnswers, string datasetPath, bool similarON, bool threasholdON, int folds, double querySize, bool equalSizedQueries, int answersPerPrediction, int lookAheadSuffix, int answersToLook, int cacheSize, double& pureCachePoints, double& predictionCachePoints, double& averagePoints){
	
	int suffixSize = 10; //hardcoded suffix size
	
	
	k_folder kf (datasetPath, maxSequences, folds, querySize, suffixSize);
	vector<predictor*> pr;
	
	vector<int> scores;
	vector<int> points;
	vector<vector<vector<int>>> q;
	q = kf.getQueries();
	vector<vector<vector<int>>> s;
	s = kf.getSuffixes();
	vector<vector<vector<int>>> p; //vector containing a vector of prediction results per fold
	vector<string> paths = kf.getTrainingPaths();
	
	for (int f = 0; f < q.size(); f++) pr.push_back(new predictor(paths[f], similarON, threasholdON));
	
	assert(q.size() == s.size());
	
	//2nd argument: history
	//3rd argument: cache size
	
	if (!CACHE){
		csvProducer csvResults("csvProducerResults.csv", 3);
		multiplePredictionsPerTestingSequence(csvResults, 3, 0, minRecursion, maxRecursion, pr, q, s);
		multiplePredictionsPerTestingSequence(csvResults, 5, 0, minRecursion, maxRecursion, pr, q, s);
		multiplePredictionsPerTestingSequence(csvResults, 7, 0, minRecursion, maxRecursion, pr, q, s);
		csvResults.writeToFile(); exit(0); // for this experiment only
	}else{
		csvProducer csvResults("csvProducerResults.csv", 3);//9);
		multiplePredictionsPerTestingSequence(csvResults, 3, 3, minRecursion, maxRecursion, pr, q, s);
		//multiplePredictionsPerTestingSequence(csvResults, 3, 6, minRecursion, maxRecursion, pr, q, s);
		//multiplePredictionsPerTestingSequence(csvResults, 3, 10, minRecursion, maxRecursion, pr, q, s);
		cout << "History 3 DONE" << endl;
		multiplePredictionsPerTestingSequence(csvResults, 5, 3, minRecursion, maxRecursion, pr, q, s);
		//multiplePredictionsPerTestingSequence(csvResults, 5, 6, minRecursion, maxRecursion, pr, q, s);
		//multiplePredictionsPerTestingSequence(csvResults, 5, 10, minRecursion, maxRecursion, pr, q, s);
		cout << "History 5 DONE" << endl;
		multiplePredictionsPerTestingSequence(csvResults, 7, 3, minRecursion, maxRecursion, pr, q, s);
		cout << "History 7 Cache 3 DONE" << endl;
		//multiplePredictionsPerTestingSequence(csvResults, 7, 6, minRecursion, maxRecursion, pr, q, s);
		cout << "History 7 Cache 6 DONE" << endl;
		//multiplePredictionsPerTestingSequence(csvResults, 7, 10, minRecursion, maxRecursion, pr, q, s);
		cout << "History 7 Cache 10 DONE" << endl;
		csvResults.writeToFile(); exit(0); // for this experiment only
	}
	
	for (int f = 0; f < q.size(); f++) {
		int predictions_number = 0;
		vector<vector<int>> cur_fold_pre;
		for (int i = 0; i < q[f].size(); i++) {
			vector<int> cur_per;
			aMap res = pr[f]->predictWithRecursiveDivider(&q[f][i][0], q[f][i].size(), answersPerPrediction, minRecursion, maxRecursion);
			if (res.size() == 0) {
				cur_per.push_back(-1);
				cur_fold_pre.push_back(cur_per);
				numOfAnswers.push_back(0);
				numOfDifferents.push_back(0);
				percentages.push_back(0);
				cur_per.erase(cur_per.begin(), cur_per.end());
				res.erase(res.begin(), res.end());
				continue;
			}
			int overallOccur = res.begin()->second; //overall occurences of a query in the dataset
			
			//cout << overallOccur << endl;//for debugging reasons
			int sum = 0;//for debugging reasons
			
			numOfAnswers.push_back(overallOccur);
			predictions_number++;
			double maxPercentage = 0.0;
			for (aMap::reverse_iterator mapIt = res.rbegin(); mapIt != res.rend(); mapIt++) {
				//mapIt->first: #times, mapIt->second: wich item, mapIt->first == -1 then mapIt->second: overall times sequence matched
				//map sorted from -1 to maximum, so trasverse it reversely
				if (mapIt->first != -1) {
					double tmp;
					cur_per.push_back(mapIt->second);
					tmp = mapIt->first / (double) overallOccur * 100;
					maxPercentage = (tmp > maxPercentage ? tmp : maxPercentage);
					sum += mapIt->first;//for debugging reasons
					numOfDifferents.push_back(res.size() - 1);
				}
			}
			percentages.push_back(maxPercentage);
			assert(sum == overallOccur);//for debugging reasons
			cur_fold_pre.push_back(cur_per);
			cur_per.erase(cur_per.begin(), cur_per.end());
			res.erase(res.begin(), res.end());
		}
		p.push_back(cur_fold_pre);
		cur_fold_pre.erase(cur_fold_pre.begin(), cur_fold_pre.end());
	}
	
	assert(p.size() == s.size);
	
	for (int f = 0; f < p.size(); f++) kf.evaluateFormula(p[f], s[f], lookAheadSuffix, answersToLook, scores);
	for (int f = 0; f < p.size(); f++) kf.gameWithCachedPredictionsV2(p[f], s[f], lookAheadSuffix, answersToLook, cacheSize, points);//just continue with declaring cacheSize as a function
	//parameter and calculating overall points that should be assigne to a new parameter. Main to printing average points (pointsSum/#folds)
	
	vector<int> pureCachePointsArray;//calling game for pure caching mechanism
	for (int f = 0; f < p.size(); f++) kf.pureCacheMechanismGame(s[f], lookAheadSuffix, cacheSize, pureCachePointsArray);
	
	pureCachePoints = mean(pureCachePointsArray);
	predictionCachePoints = mean(points);
	
	vector<double> percentageFromPureCache(points.size());
	
	//finding the percentage of prediction caching game with pure caching mechanism
	for (int i = 0; i < points.size(); i++) {
		double target = pureCachePointsArray[i];
		double actual = points[i];
		if (actual >= 0 && target > 0) percentageFromPureCache[i] = (actual/target)*100;
		else if (target <= 0 && actual < 0){
			if (target > actual) percentageFromPureCache[i] = -(actual/target)*100;
			else if (target < actual) percentageFromPureCache[i] = (actual/target)*100;
		}else if (target > 0 && actual <= 0) percentageFromPureCache[i] = ((actual-target)/target)*100;
		else if (target < 0 && actual >= 0) percentageFromPureCache[i] = (abs(target)+actual)/abs(target)*100;
	}
	
	//average percentage and actual points for all folds.
	averagePoints =  accumulate(percentageFromPureCache.begin(), percentageFromPureCache.end(), 0.0) / (double) percentageFromPureCache.size();//mean(percentageFromPureCache);
	
	//is first element of the suffix contained somewhere among my predictions ?
	//counting this situation and giving the ratio of suffix[0]_appeared_among_predictions / queries_number
	int firstOfSuffixAppeardTimes = 0;
	int overallQueriesNum = 0;
	for (int f = 0; f < s.size(); f++) {
		for (int i = 0; i < s[f].size(); i++) {
			if (find(p[f][i].begin(), p[f][i].end(), s[f][i][0]) != p[f][i].end()) firstOfSuffixAppeardTimes++;
		}
		overallQueriesNum += p[f].size();
	}
	ratioOfSuffixAppearedInAnswers = firstOfSuffixAppeardTimes / (double) overallQueriesNum * 100;
	//end of this evaluation argument
	
	int counter = 0;
	
	for (int i = 0; i < scores.size(); i++) counter = scores[i] > 0 ? counter + 1 : counter;
	succPredicts = counter;
	
	counter = 0;
	
	for (int f = 0; f < p.size(); f++) for (int i = 0; i < p[f].size(); i++) counter = p[f][i][0] != -1 ? counter + 1 : counter;
	predictionsAch = counter;
	
	queriesNum = overallQueriesNum;
	
	for (int f = 0; f < q.size(); f++) delete pr[f];
	
}


int main(int argc, const char * argv[]){
	
	//writing results to file
	
	ofstream outputFile;
	datasetName = argv[1];
	
	///Users/rafaelktistakis/Documents/PhD_UoL/DissertationProject/backwardsSearchBWT/backwardsSearchBWT/
	
	int res = access("accuracyTest.csv", R_OK);
	if (res < 0) {
		if (errno == ENOENT) {
			// file does not exist
			outputFile.open("accuracyTest.csv");
			outputFile << "Dataset Name,Folds,Query Size,Equal Size,Similar Search ON,Threashold ON,Queries Asked,Queries Answered,Successful Answers, Possible Answers per Prediction, Suffix size,Ratio of Answers %, Ratio of Success %, Coverage, Local Accuracy, (MEAN) Answers per query, (MEDIAN) Answers per query, (MODE) Answers per query, (RANGE) Answers per query, (VARIANCE) Answers per query, (CV) Answers per query, (MEAN) Number of Diff Answers per query, (MEDIAN) Number of Diff Answers per query, (MODE) Number of Diff Answers per query, (RANGE) Number of Diff Answers per query, (VARIANCE) Number of Diff Answers per query, (CV) Number of Diff Answers per query, (MEAN) Answer appears % per query, (MEDIAN) Answer appears % per query, (MODE) Answer appears % per query, (RANGE) Answer appears % per query, (VARIANCE) Answer appears % per query, (CV) Answer appears % per query, Ratio of first suffix appeared among Answers, Pure cache Points, Predictions in Cache Points, Average Points for Game % of Target" << endl;
		}
	}else{
		outputFile.open("accuracyTest.csv", ios_base::app);
	}

	//write anything to outfile: outputFile << "bla bla" << endl;
	
	//------------EXPERIMENT Group 1 -------------------------------------------------
	
	int queriesNum, predictionsAch, succPredicts;
	double ratioOfSuffixAppearedInAnswers;
	bool similarON = false;
	bool threasholdON = true;
	int folds = 10;
	double querySize = 0;
	bool equalSizedQueries = true;
	int answersPerPrediction = 0;
	int lookAheadSuffix = 0;
	int answersToExploit = 0;
	int cacheSize = 50;
	double averagePoints;
	double pureCachePoints;
	double predictionsCachePoints;
	
	int experimentCount = 1;
	
	for (int i = 2; i <= 6; i++) { // query size
		for (int k = 1; k <= 1; k++) { // look ahead suffix
			queriesNum = 0; predictionsAch = 0; succPredicts = 0; ratioOfSuffixAppearedInAnswers = 0;
			querySize = i; answersPerPrediction = 20; lookAheadSuffix = k; answersToExploit = 1;
			similarON = false;
			
			executeEvaluation (stoi(argv[2], nullptr), stoi(argv[3], nullptr), stoi(argv[4], nullptr), queriesNum, predictionsAch, succPredicts, ratioOfSuffixAppearedInAnswers, argv[1], similarON, threasholdON, folds, querySize, equalSizedQueries, answersPerPrediction, lookAheadSuffix, answersToExploit, cacheSize, pureCachePoints, predictionsCachePoints, averagePoints);
			vector<int> percentages_rounded = roundVector(percentages);
			outputFile << argv[1] << "," << folds << "," << querySize << "," << (equalSizedQueries == true ? "YES" : "NO") << "," << (similarON == true ? "YES" : "NO") << "," << (threasholdON == true ? "YES" : "NO") << "," << queriesNum << "," << predictionsAch << "," << succPredicts << "," << answersPerPrediction << "," << lookAheadSuffix << "," << (double)predictionsAch / queriesNum * 100 << "," << succPredicts / (double) queriesNum * 100 << "," << (queriesNum - predictionsAch) / (double) queriesNum * 100 << "," << (predictionsAch != 0 ? succPredicts / (double) predictionsAch * 100 : -1) << "," << mean(numOfAnswers) << "," << median(numOfAnswers) << "," << mode(numOfAnswers) << "," << range(numOfAnswers) << "," << variance(numOfAnswers) << "," << cvValue(numOfAnswers) << "," << mean(numOfDifferents) << "," << median(numOfDifferents) << "," << mode(numOfDifferents) << "," << range(numOfDifferents) << "," << variance(numOfDifferents) << "," << cvValue(numOfDifferents) << "," << mean(percentages_rounded) << "," << median(percentages_rounded) << "," << mode(percentages_rounded) << "," << range(percentages_rounded) << "," << variance(percentages_rounded) << "," << cvValue(percentages_rounded) << "," << ratioOfSuffixAppearedInAnswers  << "," << pureCachePoints << "," << predictionsCachePoints << "," << averagePoints << endl;
			
			percentages.erase(percentages.begin());
			percentages_rounded.erase(percentages_rounded.begin());
			numOfDifferents.erase(numOfDifferents.begin());
			numOfAnswers.erase(numOfAnswers.begin());
			
			cout << "EXPERIMENT " << experimentCount << " DONE" << endl;
			experimentCount++;
			
			
			queriesNum = 0; predictionsAch = 0; succPredicts = 0; ratioOfSuffixAppearedInAnswers = 0;
			similarON = true;
			
			executeEvaluation (stoi(argv[2], nullptr), stoi(argv[3], nullptr), stoi(argv[4], nullptr), queriesNum, predictionsAch, succPredicts, ratioOfSuffixAppearedInAnswers, argv[1], similarON, threasholdON, folds, querySize, equalSizedQueries, answersPerPrediction, lookAheadSuffix, answersToExploit, cacheSize, pureCachePoints, predictionsCachePoints, averagePoints);
			percentages_rounded = roundVector(percentages);
			outputFile << argv[1] << "," << folds << "," << querySize << "," << (equalSizedQueries == true ? "YES" : "NO") << "," << (similarON == true ? "YES" : "NO") << "," << (threasholdON == true ? "YES" : "NO") << "," << queriesNum << "," << predictionsAch << "," << succPredicts << "," << answersPerPrediction << "," << lookAheadSuffix << "," << (double)predictionsAch / queriesNum * 100 << "," << succPredicts / (double) queriesNum * 100 << "," << (queriesNum - predictionsAch) / (double) queriesNum * 100 << "," << (predictionsAch != 0 ? succPredicts / (double) predictionsAch * 100 : -1) << "," << mean(numOfAnswers) << "," << median(numOfAnswers) << "," << mode(numOfAnswers) << "," << range(numOfAnswers) << "," << variance(numOfAnswers) << "," << cvValue(numOfAnswers) << "," << mean(numOfDifferents) << "," << median(numOfDifferents) << "," << mode(numOfDifferents) << "," << range(numOfDifferents) << "," << variance(numOfDifferents) << "," << cvValue(numOfDifferents) << "," << mean(percentages_rounded) << "," << median(percentages_rounded) << "," << mode(percentages_rounded) << "," << range(percentages_rounded) << "," << variance(percentages_rounded) << "," << cvValue(percentages_rounded) << "," << ratioOfSuffixAppearedInAnswers << "," << pureCachePoints << "," << predictionsCachePoints << "," << averagePoints << endl;
			
			percentages.erase(percentages.begin());
			percentages_rounded.erase(percentages_rounded.begin());
			numOfDifferents.erase(numOfDifferents.begin());
			numOfAnswers.erase(numOfAnswers.begin());
			
			cout << "EXPERIMENT " << experimentCount << " DONE" << endl;
			experimentCount++;
			
		}
	}
	
	//------------END------------------------------------------------------------
	outputFile << endl;
	
	
	//cout << "Overall queries: " << q.size() << " Achieved Predictions: " << p.size() << " Successful Ones: " << counter << endl;
	cout << "DONE" << endl;
	outputFile.close();
	
	return 0;
}
