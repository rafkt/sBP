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

void treeExpansion(vector<int> xy, int index, int expansions){

	for (int k = 0; k < index + 1; k++){
			if (xy[k] == -100) continue;
			cout << xy[k] << " ";
		}
		cout << endl;

	if (index == xy.size() - 1) {
		cout << "End reached: " << " :";
		for (int k = 0; k < index + 1; k++){
			if (xy[k] == -100) continue;
			cout << xy[k] << " ";
		}
		cout << endl;
		return;
	}
	else{
		treeExpansion(xy, index + 1, expansions);
		//cout << "successful" << endl;

		if (expansions == 0) return;
		int previous = xy[index + 1];
		xy[index + 1] = -2;
		treeExpansion(xy, index + 1, expansions - 1);
		xy[index + 1] = previous;

	}
}


int main(){

	vector<int> x = {-100, 5, 4, 3, 2, 1};

	treeExpansion(x, 0, 2);
	
	return 0;
}