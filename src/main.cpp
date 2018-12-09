#include <iostream>
#include <fstream>
#include <cstring>

#include "lz78.hpp"
#include "sarray.hpp"

int main(void) {
    // sarray banana;
    // banana.fromText("bananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANAbananaBANANA");
    // banana.print();
    // banana.fromBytes(banana.toBytes());
    // banana.print();
    // auto ret = banana.search("anana");
    // for (auto x: ret){
    //     printf("\n %d ", x);
    // }
    // READING PART
    // return 0;

    encoder ecode("dat/dna50MB.idx", 1000000);
    std::ifstream ifile("dat/dna50MB");
    char* txt = new char[52428801];

    // ENCODE PART

    //while(!ifile.eof()){
    ifile.read(txt, 52428801);
    std::string s(txt);
    ecode.encode(s);
    //}
    ecode.close();

    // DECODE PART

    decoder dcode("dat/dna50MB.idx", 1000000);
    while(dcode.has) {
        std::string s = dcode.decode(1000000);
        std::cout << s;
    }
    dcode.close();
    
    return 0;
}