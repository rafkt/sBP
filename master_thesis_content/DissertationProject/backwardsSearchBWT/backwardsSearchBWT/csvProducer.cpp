//
//  csvProducer.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 20/06/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include "csvProducer.h"
#include <iomanip>
#include <algorithm>

using namespace::std;

csvProducer::csvProducer(string name, int rows): fileName(name), rowsNumber(rows){
}

int csvProducer::state(){
	return names.size();
}

void csvProducer::addCollumn(string name, vector<string> values){//should add a column with all corresponding dataset names
	vector<string>::iterator it;
	it = find (names.begin(), names.end(), name);
	if (it != names.end()) return; // already exists
	names.push_back(name);
	columns.push_back(values);
}

void csvProducer::addCollumn(string name){
	vector<string>::iterator it;
	it = find (names.begin(), names.end(), name);
	if (it != names.end()) return; // already exists
	names.push_back(name);
	vector<string> values;
	columns.push_back(values);
}

void csvProducer::addToCollumn(string name, string value){// should add dataset name too
	vector<string>::iterator it;
	
	it = find (names.begin(), names.end(), name);
	if (it == names.end()) return;//nothing found
	columns[it - names.begin()].push_back(value);
}


void csvProducer::writeToFile(){
	ofstream outputFile;
	bool writeNames = false;
	
	int res = access("csvProducerResults.csv", R_OK);
	if (res < 0) {
		if (errno == ENOENT) writeNames = true;
	}
	
	outputFile.open(fileName, ios_base::app);
	
	for (int i = 0; i < rowsNumber; i++) {
		if ( i == 0 && writeNames) { //we should write the names row first
			for (int k = 0; k < names.size(); k++) {
				if (k < names.size() - 1) outputFile << names[k] << ",";
				else outputFile << names[k] << endl;
			}
		}
		//write all the remaining rows with their values
		for (int k = 0; k < columns.size(); k++) {
			if (k < columns.size() - 1) outputFile << columns[k][i] << ",";
			else outputFile << columns[k][i] << endl;
		}
	}
	
	outputFile.close();
}
