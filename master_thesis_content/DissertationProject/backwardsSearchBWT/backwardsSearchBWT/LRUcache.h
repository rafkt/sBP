//
//  LRUcache.h
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 12/05/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include <string.h>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

// concider using a std::rotate for move to front operation

class LRUcache{
public:
	LRUcache(); // create a cache of size 10
	LRUcache(int); // create a cache of sieze s
	~LRUcache(); // clear everything
	bool get(int); // returns true if the item with specific id is already in cache - if not contained in the cache returns false, after that the missing item has been placed in the cache though. Every item that was requested it will have to be placed on top.
	friend ostream& operator<<(ostream&, const LRUcache&);
private:
	vector<int> cacheStack;
	int size;
	bool put (int); // place the item with specific id into the cache (if it's not there already) - if the procedure doesn't complete return false, otherwise true. Item has to be on top.
	bool flush(); // Remove the last item of the vector. Return true if flushed, false otherwise
	
};