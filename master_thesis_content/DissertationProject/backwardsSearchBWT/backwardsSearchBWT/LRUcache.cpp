//
//  LRUcache.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 12/05/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include "LRUcache.h"
#include <algorithm>


LRUcache::LRUcache(): size(10){}
LRUcache::LRUcache(int s): size(s){}
LRUcache::~LRUcache(){
	cacheStack.clear();
	vector<int> tmp;
	vector<int>().swap(tmp);
}
bool LRUcache::get(int item){
	vector<int>::iterator it = find(cacheStack.begin(), cacheStack.end(), item);
	if ( it != cacheStack.end()) {
		int pos = it - cacheStack.begin();
		cacheStack.erase(it);
		cacheStack.insert(cacheStack.begin(), item);
		return true;
	}else{
		flush();
		put(item);
	}
	return false;
}

bool LRUcache::put (int item){
	cacheStack.insert(cacheStack.begin(), item);
	return true;
}

bool LRUcache::flush(){
	if (cacheStack.size() >= size) {
		cacheStack.pop_back();
		return true;
	}
	return false;
}

ostream& operator<<(ostream& os, const LRUcache& c)
{
	for (int i = 0; i < c.cacheStack.size(); i++) {
		os << c.cacheStack[i] << " ";
	}
	os << endl;
	return os;
}