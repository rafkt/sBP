#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;


int main(){

	vector<int> query = {1, 2, 3, 4, 5, 6, 7};

	vector<vector<int>> queue;

	

	

	for (int d = 0; d < query.size() - 1; d++){
		//reset prediction count here
		vector<int> shorter(query.begin() + d, query.end());

		int l = 0;
		queue.push_back(shorter);
		while (queue.size() != 0){
			vector<int> current = queue[0];
			queue.erase(queue.begin());

			//do studff with current
			for (int item : current) cout << item << " ";
			cout << endl;
			if (l == 0) cout << "0-error prediction finished" << endl;
			else if (l == current.size() - 1) cout << "1-error predictions finished" << endl;// stop condition here
			//end of doing stuff


			if (l < shorter.size() - 1){
				for (int i = l; i < current.size() - 1; i++){
					vector<int> next = current;
					next[i] = -2;
					queue.push_back(next); 
				}
			}
			l++;
		}
		cout << "2-errors predictions finished" << endl; //stop condition here
	}
}