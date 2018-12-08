#include <iostream>
#include <fstream>

#include "lz78.hpp"
#include "sarray.hpp"

int main(void) {
    sarray banana;
   // banana.fromText("BANANAbananaBANANA");

    banana.fromText("bananaBANANAbananaBANANA\nbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANA");
   // banana.print();
   // banana.fromBytes(banana.toBytes());
   // banana.print();
    auto ret = banana.search("anana");
    for (auto x: ret){
        printf("\n = %d ", x);
    }
    // READING PART
    return 0;

    encoder ecode("dat/shakespeare.idx", 10000);
    std::ifstream ifile("dat/shakespeare.txt");
    char* txt = new char[1000000];

    // ENCODE PART

    while(ifile.good()){
        ifile.read(txt, 1000000);
        std::string s(txt);
        ecode.encode(s);
    }
    ecode.close();

    // DECODE PART

    decoder dcode("dat/shakespeare.idx", 10000);
    while(dcode.has) {
        std::string s = dcode.decode(1000);
        std::cout << s;
    }
    dcode.close();
    
    return 0;
}