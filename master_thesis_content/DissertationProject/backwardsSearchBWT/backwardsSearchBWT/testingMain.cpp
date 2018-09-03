//
//  testingMain.cpp
//  backwardsSearchBWT
//
//  Created by Rafael Ktistakis on 16/03/2015.
//  Copyright (c) 2015 Rafael Ktistakis. All rights reserved.
//

#include "LRUcache.h"
#include <stdio.h>
#include <assert.h>


int main(int argc, const char * argv[]){
	
	LRUcache cache(5);
	
	cache.get(1);
	cache.get(2);
	cache.get(3);
	cache.get(4);
	cache.get(5);
	
	cout << cache << endl;
	
	cache.get(4);
	cache.get(3);
	cout << cache.get(5) << endl;
	
	cout << cache << endl;
	
	cout << cache.get(15) << endl;
	
	cout << cache << endl;
	
	return 0;
}