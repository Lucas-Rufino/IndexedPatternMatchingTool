#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>

#include "lz78.hpp"
#include "sarray.hpp"

using namespace std;

vector<string> getPatterns(string patternsFile){
    ifstream pfile(patternsFile);
    vector<string> patterns;
    string pattern;
    while(!pfile.eof()) {
        getline(pfile, pattern);
        if(pattern.size() != 0){ 
            patterns.push_back(pattern);
        }
    }
    return patterns;
}

int main(int argc, char* argv[]) {
    if(argc == 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printf("Should be a help menu here\n");
        return 0;
    } else if(argc == 3 && strcmp(argv[1], "index") == 0) {
        ifstream infile(argv[2]);
        infile.seekg (0, infile.end);
        int length = infile.tellg();
        infile.seekg (0, infile.beg);

        char* txt = new char[length];
        infile.read(txt, length);
        string fileText(txt);
        infile.close();

        sarray aux;
        aux.fromText(fileText);
        std::vector<char> toFile = aux.toBytes();

        int len = strlen(argv[2]) + 5;
        char* newName = new char[len];
        for(int i=0 ; i<len ; i++) {
            newName[i] = argv[2][i];
            if(newName[i] == '.' || newName[i] == '\0') {
                newName[i++] = '.';
                newName[i++] = 'i';
                newName[i++] = 'd';
                newName[i++] = 'x';
                newName[i] = '\0';
                break;
            }
        }

        ofstream outfile (newName);
        for(int i=24 ; i>=0 ; i-=8) {
            outfile.put(char(toFile.size() >> i));
        }
        for(int i=24 ; i>=0 ; i-=8) {
            outfile.put(char(fileText.size() >> i));
        }

        outfile.write(toFile.data(), toFile.size());
        outfile.close();

        encoder ecode(newName, 10000);
        ecode.encode(fileText);
        ecode.close();
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
                    std::cerr << "Wrong usage, try using: ipmt --help";
                    return -1;
                }
            }
        }
        if(state != 3){
            std::cerr << "Wrong usage, try using: ipmt --help";
            return -1;
        }

        ifstream infile(idxFilePath);

        int sizeSfx = 0;
        for(int i=24 ; i>=0 ; i-=8) {
            sizeSfx = (sizeSfx << 8) | infile.get();
        }

        int sizeTxt = 0;
        for(int i=24 ; i>=0 ; i-=8) {
            sizeTxt = (sizeTxt << 8) | infile.get();
        }

        char* data = new char[sizeSfx];
        infile.read(data, sizeSfx);
        vector<char> idxFileBytes(data, data + sizeSfx);

        decoder dcode(idxFilePath, 10000, infile.tellg());
        string txt = dcode.decode(sizeTxt);                 // <---- string do texto completa
        dcode.close();

        sarray aux;
        aux.fromBytes(idxFileBytes);

        for(string s : patterns) {
            vector<int> indexes = aux.search(s);
            //TODO: IMPRESSÃO DE LINHAS OU DA CONTAGEM DE LINHAS DE ACORDO COM A RESPOSTA DO SEARCH 
        }

    } else {
        std::cerr << "Wrong usage, try using: ipmt --help";
        return -1;
    }

    return 0;
}