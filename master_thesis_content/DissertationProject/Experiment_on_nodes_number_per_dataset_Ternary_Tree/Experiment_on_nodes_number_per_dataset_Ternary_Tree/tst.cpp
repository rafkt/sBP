#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>  
#include <cmath> 
#include <unistd.h>

using namespace std;

vector< unsigned int* > Trie;
	ofstream myfile;
unsigned int* headerTableS =new unsigned int [5267657];

unsigned int distItems=0;
unsigned int numRows=0;
unsigned long nodeNUM=0;
unsigned long Items = 0;
bool chained;
unsigned int chainCounter=0;
//unsigned int numofChains=0;
unsigned int dummy=9999999;
vector <unsigned int> chain;
//double support;
string file;
unsigned int* deltaHeader;

unsigned int maxCount=0;
unsigned int maxItem=0;
unsigned int whatBlah=0;
bool* checkTrie;


	void readFileGetHeader() {
	    string STRING;
	    char * cstr, *p;
	    ifstream infile;
	    infile.open(file.c_str());
	    //vector <unsigned int> tmpV;

	    int a=0;
	    while(a<1) {// To get you all the lines.
	    
		getline(infile,STRING); // Saves the line in STRING.
		if (!infile.eof()){	    	    
			cstr = new char [STRING.size()+1];
			strcpy (cstr, STRING.c_str());
			p=strtok (cstr," ");
			while (p!=NULL  ){
				if(atoi(p)!=-1 && atoi(p) != -2){
                    Items++;
					if (headerTableS[atoi(p)]==0){
						distItems++;
						//if (maxItem < atoi(p)) maxItem=atoi(p);
					}
					headerTableS[atoi(p)]++;
				}else if (atoi(p) == -2) break;
					p=strtok(NULL," ");
			}
			delete[] cstr;
		    
		}else{
		 break;
		}
		//cout<<"=>"<<numRows<<", "<<distItems<<"|"<<endl;
		    numRows++;
	    }
		maxItem+=1;
		//cout<<"Distinct Items: "<< distItems<<endl;
		//cout<<"Number of Transactions: "<< numRows<<endl;
	    infile.close();
	}

	vector<unsigned int> getVector(string s){
		unsigned int count_level=0;

	    char * cstr, *p;
		vector<unsigned int> items;

		cstr = new char [s.size()+1];
		strcpy (cstr, s.c_str());

		p=strtok (cstr," ");

		while ( p!=NULL ){
			if(atoi(p)!=-1 && atoi(p) != -2) items.push_back(atoi(p));
			p=strtok(NULL," ");
		}
		delete[] cstr;
		return items;
	}


	unsigned int searchSibling(unsigned int tmpJ,unsigned int transI){

		unsigned int l=0, m=1, r=2, id=3, pc=4; 
		unsigned int* tmpV3= new unsigned int[5];
		tmpV3=Trie.at(tmpJ);
		unsigned int currentNode=headerTableS[tmpV3[id]];
		//cout<<transI<<endl;
		unsigned int searchFor=headerTableS[transI];
		//delete[] tmpV3;
		bool flag=true;
		while(flag){
		unsigned int* tmpV2= new unsigned int[5];
		tmpV2=Trie.at(tmpJ);		
			if (transI==tmpV2[id]){//Trie[tmpJ][id]){
				flag=false;
			}else{
				tmpV2=Trie.at(tmpJ);		
				if (searchFor>currentNode){				
					if(tmpV2[r]==0){
					flag=false;
					}else{
					tmpJ=tmpV2[r];
					tmpV2=Trie.at(tmpJ);
					currentNode=headerTableS[tmpV2[id]];
					}
				}else if (searchFor<=currentNode){
					if(tmpV2[l]==0){
					flag=false;
					}else{
					tmpJ=tmpV2[l];
					tmpV2=Trie.at(tmpJ);
					currentNode=headerTableS[tmpV2[id]];	
					}
				}
			}
		//delete[] tmpV2;
		}
		return tmpJ;
	}


	void manageTrie(vector<unsigned int> & trans){
		bool match=false;
		bool equal=false;
		unsigned int l=0, m=1, r=2, id=3, pc=4; 
		unsigned int tmpJ=0;
		unsigned int tmpI=0;
		unsigned int* tmpVGL= new unsigned int[5];
		//start handling transactions
		for(int i=0; i <trans.size(); i++){
		unsigned int* tmpV2= new unsigned int[5];

		tmpI=i+1;
		//set tmV2 
		tmpV2=Trie.at(tmpJ);
		if(equal && tmpV2[m]==0){
			tmpI--;
			match=false;
		}else{ 
			tmpV2=Trie.at(tmpJ);
			tmpJ=tmpV2[m];
			tmpV2=Trie.at(tmpJ);
			if(trans[i]==tmpV2[id]){
				match=true;
				equal=true;
				if(i==trans.size()-1)
				tmpV2[pc]++;
			}else{//if no match we search for sibblings

				tmpJ=searchSibling(tmpJ,trans[i]);//return the tmpJ of last non zero value
				tmpV2=Trie.at(tmpJ);
				if(trans[i]==tmpV2[id]){
					match=true;	
					equal=true;

				if(i==trans.size()-1)
					tmpV2[pc]++;			
				}else{
					match=false;
					tmpV2=Trie.at(tmpJ);
					if(headerTableS[trans[i]]>headerTableS[tmpV2[id]]){
						unsigned int* tmpV= new unsigned int[5];
						for(int j=0;j<5;j++){
							if(j==id){ 
								tmpV[j]=trans[i];		
							}else if(j==pc){ 
								if(i==trans.size()-1) tmpV[j]=1;
							}else{
								tmpV[j]=0;
							}
						}
						Trie.push_back(tmpV);
						tmpV2=Trie.at(tmpJ);
						tmpV2[r]=Trie.size()-1;
						Trie[tmpJ]=tmpV2;
						tmpJ=tmpV2[r];
					}else{
							unsigned int* tmpV= new unsigned int[5];
						for(int j=0;j<5;j++){

							if(j==id){
								tmpV[j]=trans[i];
							}else if(j==pc){ 
								if(i==trans.size()-1) tmpV[j]=1;
							}else{
								tmpV[j]=0;		
							}
						}
						Trie.push_back(tmpV);
						tmpV2=Trie.at(tmpJ);
						tmpV2[l]=Trie.size()-1;
						tmpJ=tmpV2[l];			
					}//end else			
				}//end if after search sibling
			}//end if that detemines match or not
		}
			if(match==false){
				i=trans.size();
				tmpVGL=tmpV2;
			}
			tmpVGL=tmpV2;
		}//end for

			if(match==false){

				for(;tmpI<trans.size();tmpI++){
					unsigned int* tmpV= new unsigned int[5];
					//unsigned int* tmpV4= new unsigned int[4];
					for(int j=0;j<5;j++){
						if(j==id){ 
							tmpV[j]=trans[tmpI];
						}else if(j==pc){ 
								if(tmpI==trans.size()-1) tmpV[j]=1;
						}else {
							tmpV[j]=0;
						}
					}
					Trie.push_back(tmpV);
					tmpVGL=Trie.at(tmpJ);
					tmpVGL[m]=Trie.size()-1;
					tmpJ=tmpVGL[m];//Trie[tmpJ][m];	
				}

			}

	}//end manageTrie()



int main(int arc, char * argv[]){
	file=argv[1];
	for( unsigned int i=0; i<5267657; i++){
		//headerTable[i]=new unsigned int[2];
		headerTableS[i]=0;
	}

	//cout <<"entering readFileGetHeader"<<endl;
	readFileGetHeader();

	    vector<unsigned int> transaction;
	    unsigned int* tmpV=new unsigned int [5];



		//set first node to  |0  1  0  0|
			for(int j=0; j<5; j++){

			if(j==1){
				tmpV[j]=1;
			}else{
				tmpV[j]=0;
			}
			}
			Trie.push_back(tmpV);

		bool firstItem=true;
	    string STRING;
	    ifstream infile;
	    infile.open(file.c_str());
	    getline(infile,STRING);
	    transaction=getVector(STRING);

		for(int i=0;i<transaction.size();i++){
	    unsigned int* tmpV3=new unsigned int [5];
			for(int j=0; j<5; j++){
				if(j==1 && i<transaction.size()-1){ 
				tmpV3[j]=(i+2);
				}else if(j==3){ 
				tmpV3[j]=transaction[i];
				}else if(j==4){
					if(i==transaction.size()-1){
					 tmpV3[j]=1;
					}else{tmpV3[j]=0;}
				}else{
				tmpV3[j]=0;
				}	
			}
			Trie.push_back(tmpV3);
		}	    
		transaction.clear();

	    int a=0;
		unsigned int progress=0;
	    while(a<1){ // To get you all the lines.

		getline(infile,STRING); // Saves the line in STRING.
		if (!infile.eof()){	
			transaction=getVector(STRING);
			manageTrie(transaction);
		}else{
		 break;
		}
			transaction.clear();

		//if (progress%100000==0) cout<<"progress: "<<progress<<" Trie Size: "<<Trie.size()<<endl;
		progress++;
	    }

	    infile.close();

	//cout<<"Trie Size: "<<Trie.size()<<endl;
    
    ofstream outputFile;
    
    int res = access("Results.csv", R_OK);
    if (res < 0) {
        if (errno == ENOENT) {
            // file does not exist
            outputFile.open("Results.csv");
            outputFile << "File Name,Nodes Number,Number of Sequences,Number of items,Distinct Items in Dataset" << endl;
        }
    }else{
        outputFile.open("Results.csv", ios_base::app);
    }
    outputFile << file << "," << Trie.size() - 1 << "," << numRows << "," << Items << "," << distItems << endl;
    
    outputFile.close();
    cout << "Done!\n";

return 0;
}

