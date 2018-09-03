#include <sdsl/wavelet_trees.hpp>
#include <sdsl/suffix_arrays.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sdsl;

void clean_myfile() {
    remove( "tmp.txt" );
}

int main(int argc, char* argv[])
{
    //string fileName(argv[1]);
    wt_int<> wt;
    csa_wt<> csa;
    
    std::filebuf fb;
    fb.open ("tmp.txt",std::ios::out);
    std::ostream os(&fb);
    os << "banana$\n";
    fb.close();
    
    int_vector<> v(5, -1, 0);
    v[3] = 150;
    cout << size_in_bytes(v) << endl;
    util::bit_compress(v);
    cout << v << endl;
    cout << size_in_bytes(v) << endl;
    store_to_file(v, "sdb.sdsl");
    construct(wt, "sdb.sdsl", 0);
    //construct(csa, "tmp.txt", 1);
    for (size_t i = 0; i < wt.size(); i++){
        cout << wt[i] << " ";
    }
    cout << endl;
    cout << size_in_bytes(wt) << endl;
    

//
//    cout << csa << endl;
//    
//    for (size_t i = 0; i < csa.size(); i++){
//        cout << i << ": " << csa[i] << endl;
//    }
//    cout << endl;
//    
//    cout << csa.psi[2] << endl;
//    
//    for (size_t i = 0; i < csa.size(); i++){
//        cout << i << ": " << csa.L[i] << endl;
//    }
//    cout << endl;
//    
//    cout << csa.L.select(1, 'b') << endl;
//    
//    
//    int_vector<> v(wt.sigma, 1, 0);
//    v[3] = 18446744073709551615;
//    cout << size_in_bytes(v) << endl;
//    util::bit_compress(v);
//    cout << v << endl;
//    cout << size_in_bytes(v) << endl;
//    
//    
//    
//    cout << endl;
//    ofstream output("tmp.txt", std::ios::binary | std::ios::trunc);
//    atexit( clean_myfile );
//    
//    cout << "-------------" << endl;
    
}
