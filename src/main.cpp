#include <iostream>
#include <fstream>

#include "lz78.hpp"

int main(void) {

    encoder ecode("dat/shakespeare.idx", 10000);
    std::ifstream ifile("dat/shakespeare.txt");
    char* txt = new char[1000000];

    while(ifile.good()){
        ifile.read(txt, 1000000);
        std::string s(txt);
        ecode.encode(s);
    }
    ecode.close();

    decoder dcode("dat/shakespeare.idx", 10000);
    while(dcode.has) {
        std::string s = dcode.decode(1000);
        std::cout << s;
    }
    dcode.close();
    
    return 0;
}