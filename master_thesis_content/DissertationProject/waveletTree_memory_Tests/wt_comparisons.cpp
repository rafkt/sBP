#include <sdsl/wavelet_trees.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sdsl;

int main(int argc, char* argv[])
{
    string fileName(argv[1]);
    wt_int<> wtInt;
    wt_gmr<> wtGmr;
    wt_rlmn<> wtRlmn;
    wm_int<> wmInt;
    wt_blcd<> wtBlcd;
    wt_hutu<> wtHutu;
    wt_huff<> wtHuff;
    
    construct(wtInt, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wtGmr, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wtRlmn, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wmInt, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wtBlcd, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wtHutu, "convertedSequencesForBWT/" + fileName, 'd');
    construct(wtHuff, "convertedSequencesForBWT/" + fileName, 'd');
    
    ofstream outputFile;
    
    int res = access("WaveletTreesComparison.csv", R_OK);
    if (res < 0) {
        if (errno == ENOENT) {
            // file does not exist
            outputFile.open("WaveletTreesComparison.csv");
            outputFile << "File Name,wt_int,wt_gmr,wt_rlmn,wm_int,wt_blcd,wt_hutu,wt_huff" << endl;
        }
    }else{
        outputFile.open("WaveletTreesComparison.csv", ios_base::app);
    }
    outputFile << argv[1] << "," << size_in_mega_bytes(wtInt) << "," << size_in_mega_bytes(wtGmr) << "," << size_in_mega_bytes(wtRlmn) << "," << size_in_mega_bytes(wmInt) << "," << size_in_mega_bytes(wtBlcd) << "," << size_in_mega_bytes(wtHutu) << "," << size_in_mega_bytes(wtHuff) << endl;
    
    outputFile.close();
    cout << "Done!\n";
    
}
