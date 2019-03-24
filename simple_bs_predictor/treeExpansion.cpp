#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;


// void treeExpansion(vector<int> xy, int index){
	
// 	if (index == xy.size()){
		
// 		return;
// 	}
// 	else{
// 		int previous = -1;
// 		if (xy[index] != -2){
// 			for (int k = 0; k < index + 1; k++){ cout << xy[k] << " ";} cout << endl; //print query for information
// 			cout << "Is search successful? (1/0) " << endl;
// 			int answer;
// 			cin >> answer;
// 			if (answer == 0){
// 				//we treat this a question mark (or -2 for our case)
// 				previous = xy[index];
// 				xy[index] = -2;
// 				treeExpansion(xy , index);
// 				xy[index] = previous;
// 				return;
// 			}else{
// 				cout << "search was successful, do you still want to expand ? (1/0)" << endl;
// 				cin >> answer;
// 				if (answer == 1){
// 					previous = xy[index];
// 					xy[index] = -2;
// 					treeExpansion(xy , index);
// 					xy[index] = previous;
// 					return;
// 				}else{
// 					treeExpansion(xy , index + 1);
// 					xy[index] = previous;
// 					previous = -1;
// 				}
// 			}
// 		}

// 		if (xy[index] == -2){
// 			for (int k = 0; k < xy.size(); k++){ cout << xy[k] << " ";} cout << endl;
// 			//we meet a real ?; find distinct
// 			cout << "Met a ?, give distinct: " << endl;
// 			cout << "How many?" << endl;
// 			int quantity;
// 			cin >> quantity;
// 			int counter = 0;
// 			vector<int> cs;
// 			do{
// 				cout << "Give distinct symbol" << endl;
// 				int symbol;
// 				cin >> symbol;
// 				cs.push_back(symbol);
// 				counter++;
// 			}while(counter < quantity);

// 			//go through the symbols now

// 			for (int i = quantity - 1; i > -1; i--){

// 				if (cs[i] == -2) {continue;}

// 				xy[index] = cs[i];

// 				for (int k = 0; k < index + 1; k++){
// 					cout << xy[k] << " ";
// 				}
// 				cout << endl;

// 				treeExpansion(xy, index + 1);
// 				if (previous != -1){
// 					xy[index] = previous;
// 					previous = -1;
// 				}
// 			}
		
// 		}
// 	}

// }

//it Can't do the order as initially described for subSeq predictor.
//For now we can do 1 error, then call again for 2 error, then call with 1 deleted item etc. and repeat.
//Either way, even if you could replicate the same behaviour recursively, you have to go to the root again which
//defies the whole purpose

void treeExpansion(vector<int> xy, int index, int expansions, int rangeStart,vector<int>& ranges){
	int newRangeStart;
	if (index == xy.size()) {
		ranges.push_back(rangeStart);
		return;
	}
	else{

		cout << "Searching for: ";
		for (int k = 0; k < index + 1; k++){
			if (xy[k] == -100) continue;
			cout << xy[k] << " ";
		}
		cout << endl;
		
		cout << "Successful? 1/0 ";
		int answer; cin >> answer; if (answer == 0) return;
		cout << "give range (one number): "; cin >> rangeStart;
		

		treeExpansion(xy, index + 1, expansions, rangeStart, ranges);

		if (index + 1 == xy.size()) return;
		else if (expansions == 0) return;
			

			int previous = xy[index + 1];
			//this is the place that we treat the index as a ?

			for (int k = 0; k < index + 1; k++){
				if (xy[k] == -100) continue;
				cout << xy[k] << " ";
			}
			cout << endl;
			cout << "Expanding, how many symbols are there? ";
			int quantity; vector<int> cs;
			cin >> quantity; cout << endl;
			int counter = 0;
			do{
				cout << "Give distinct symbol " << endl;
				int symbol;
				cin >> symbol;
				cs.push_back(symbol);
				counter++;
			}while(counter < quantity);

		for (int smb : cs){
			xy[index + 1] = smb;
			treeExpansion(xy, index + 1, expansions - 1, rangeStart, ranges);
		}
		xy[index + 1] = previous;
	}
}


// if (expansions == 0) return;
// 		int previous = xy[index + 1];
// 		xy[index + 1] = -2;
// 		treeExpansion(xy, index + 1, expansions - 1);
// 		xy[index + 1] = previous;


// if (expansions == 0) return;
// 		int previous = xy[index + 1];
// 		//this is the place that we treat the index as a ?

// 			cout << "Expanding, how many symbols are there? ";
// 			int quantity; vector<int> cs;
// 			cin >> quantity; cout << endl;
// 			int counter = 0;
// 			do{
// 				cout << "Give distinct symbol " << endl;
// 				int symbol;
// 				cin >> symbol;
// 				cs.push_back(symbol);
// 				counter++;
// 			}while(counter < quantity);

// 		for (int smb : cs){
// 			xy[index + 1] = smb;
// 			treeExpansion(xy, index + 1, expansions - 1);
// 		}
// 		xy[index + 1] = previous;


int main(){

	vector<int> ranges;

	vector<int> x = {1, 2, 3, 4, 5};

	treeExpansion(x, 0, 1, 0, ranges);

	for (int i : ranges) cout << i << " ";
	cout << endl;
	
	return 0;
}