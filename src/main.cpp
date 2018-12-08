#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "lz78.hpp"
#include "sarray.hpp"
using namespace std;
vector<string> getPatterns(string patternsFile){
    //TODO: get patterns from file
}

int main(int argc, char* argv[]) {
    if(argc == 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printf("Should be a help menu here\n");
        return 0;
    } else if(argc == 3 && strcmp(argv[1], "index") == 0) {
        string fileText;
        //TODO:: read the file in argv[2], and put in fileText;
        sarray aux;
        aux.fromText(fileText);
        std::vector<char> toFile = aux.toBytes();
        //TODO: encode AND write to an idx the toFile
    } else if(argc >= 3 && strcmp(argv[1], "search") == 0) {
        int state=0;
        bool count=false;
        vector<string> patterns;
        string idxFilePath;
        for (int i=2; i < argc; i++){
            if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--count") == 0)
                count = true;
            else if(strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--pattern") == 0)
                state=1;
            else {
                if(state==0){
                    patterns.push_back(string(argv[i]));
                    state=2;
                } else if(state==1){
                    patterns = getPatterns(string(argv[i]));
                    state=2;
                } else if(state==2){
                    idxFilePath = string(argv[i]);
                    state=3;
                } else{
                    std::cerr<<"Wrong usage, try using: ipmt --help"
                    return -1;
                }
            }
        }
        if(state != 3){
            std::cerr<<"Wrong usage, try using: ipmt --help"
            return -1;
        }
        //TODO:read and decode idxFilePath to idxFileBytes
        vector<char> idxFileBytes;
        sarray aux;
        aux.fromBytes(idxFileBytes);
        //TODO: use search on each pattern and print output
    } else {
        std::cerr<<"Wrong usage, try using: ipmt --help"
        return -1;
    }
    
    return 0;
}