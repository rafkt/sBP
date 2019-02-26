//Compile with:
//g++ -std=c++11 -O3 -g -DNDEBUG -I ~/include -L ~/lib wt_distinct_elem_test.cpp -lsdsl -ldivsufsort -ldivsufsort64

#include <sdsl/wavelet_trees.hpp>
#include <sdsl/int_vector.hpp>
#include <sdsl/wt_helper.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace sdsl;


int main(){

	wt_int<> L1, L2;

	int length = 100000;
	int sigma = 100000; //vector width 10 bits

	int_vector<> v1(length, sigma - 1, 64);// = {0,1, 2,3, 4,5, 6,7, 8,9, 0,1, 2,3}; // 0,2,1,3
	int_vector<> v2(length, sigma - 1, 64);
	//int_vector definition: int_vector<>(n,x,l), with n equals size, x default integer value, l width of integer (has no effect for w > 0).
	//so for my case: int_vetor<>(L.size(), 0, L.size()) LplusOne
	//access with: LplusOne[index]

	for (int i = 0; i < v1.size(); i++){
		if (i % 2 == 0) v1[i] = 0;
		else v1[i] = sigma - 1;

		// if (i > sigma - 1) v[i] = i % (sigma - 1);
		// else v[i] = i;

		//cout << v1[i] << " ";
	}
	//cout << endl;

	construct_im(L1, v1);

	cout << "WT_int of length " << length << " filled with 0 and " << sigma - 1 << endl;
	cout << "Sigma: " << L1.sigma << endl;
	cout << "Size in bytes: " << size_in_bytes(L1) << endl;


	for (int i = 0; i < v2.size(); i++){
		// if (i % 2 == 0) v[i] = 0;
		// else v[i] = sigma - 1;

		if (i > sigma - 1) v2[i] = i % (sigma);
		else v2[i] = i;

		//cout << v2[i] << " ";
	}
	//cout << endl;

	construct_im(L2, v2);

	cout << "WT_int of length " << length << " filled from 0 to " << sigma - 1 << endl;
	cout << "Sigma: " << L2.sigma << endl;
	cout << "Size in bytes: " << size_in_bytes(L2) << endl;

	return 0;
}
