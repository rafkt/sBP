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

	wt_int<> L;

	int_vector<> v = {3,2,1,0,2,1,3,4,1,1,1,3,2,3}; // 0,2,1,3

	construct_im(L, v);

	uint64_t quantity;                          // quantity of characters in interval
    std::vector<uint64_t> cs(L.sigma);      // list of characters in the interval
    std::vector<uint64_t> rank_c_i(L.sigma);    // number of occurrence of character in [0 .. i-1]
    std::vector<uint64_t> rank_c_j(L.sigma);    // number of occurrence of character in [0 .. j-1]

	interval_symbols(L, 3, 7, quantity, cs, rank_c_i, rank_c_j);

	cout << quantity << endl;

	cout << endl;

	for (uint64_t i : cs) cout << i << " ";
	cout << endl;

	for (int i = 0 ; i < cs.size(); i ++){
		cout << rank_c_j[i] << endl;
	}

	return 0;
}
