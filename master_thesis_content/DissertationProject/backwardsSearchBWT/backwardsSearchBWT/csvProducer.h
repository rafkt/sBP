//
//  csvProducer.h
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 20/06/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//



#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>


using namespace::std;

class csvProducer{
public:
	csvProducer(string, int);
	void addCollumn(string, vector<string>);
	void addCollumn(string);
	void addToCollumn(string, string);
	void writeToFile();
	int state();
private:
	vector<vector<string>> columns;
	vector<string> names;
	string fileName;
	int rowsNumber;
};