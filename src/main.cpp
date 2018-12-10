#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <chrono>  // for high_resolution_clock


// Record start time


#include "lz78.hpp"
#include "sarray.hpp"

using namespace std;

#ifdef TIME_ME_FLAG
#define time_start(str) {start = std::chrono::high_resolution_clock::now();time_stamp=str;}
#define time_end {std::chrono::duration<double> dur = std::chrono::high_resolution_clock::now() - start; std::cerr<<"Time of "<<time_stamp<<": "<<dur.count()<<std::endl;}
#else
#define time_start(str) {}
#define time_end {}
#endif

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();
    string time_stamp = "";
    if(argc == 1 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
        printf("Usage: \n\tipmt index FILE\n\tipmt search [OPTION] [PATTERN] INDEX\n\t\toptions:\n\t\t\t-c --count: shows just the count\n\t\t\t-p --pattern FILE: search for all the patterns in the file\n");
        return 0;
    } else if(argc == 3 && strcmp(argv[1], "index") == 0) {
        ifstream infile(argv[2]);
        infile.seekg (0, infile.end);
        int length = infile.tellg();
        infile.seekg (0, infile.beg);

        if(length < 0) {
            std::cerr << "file not found!" << endl;
            return -1;
        }

        char* txt = new char[length];
        infile.read(txt, length);
        string fileText(txt);
        infile.close();

        time_start("index");
        sarray aux;
        aux.fromText(fileText);
        std::vector<char> toFile = aux.toBytes();
        time_end;

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

        time_start("encode");
        encoder ecode(newName, 10000);
        ecode.encode(fileText);
        ecode.close();
        time_end;
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
                    ifstream pfile(argv[i]);

                    if(pfile.tellg() < 0) {
                        std::cerr << "patterns file not found!" << endl;
                        return -1;
                    }

                    string pattern;
                    while(!pfile.eof()) {
                        getline(pfile, pattern);
                        if(pattern.size() != 0){ 
                            patterns.push_back(pattern);
                        }
                    }
                    pfile.close();
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

        if(infile.tellg() < 0) {
            std::cerr << "index file not found!" << endl;
            return -1;
        }

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
        time_start("decode");
        decoder dcode(idxFilePath, 10000, infile.tellg());
        string txt = dcode.decode(sizeTxt);                 // <---- string do texto completa
        dcode.close();
        time_end;
        sarray aux;

        time_start("search");
        aux.fromBytes(idxFileBytes);

        set<int> result;

        for(string s : patterns) {
            vector<int> indexes = aux.search(s);
            result.insert(indexes.begin(), indexes.end());
        }
        time_end;
        if(count) {
            cout << result.size() << endl;
        } else {
            string line;
            int count_line = 0;
            stringstream ss(txt);
            for(int i : result) {
                while(count_line <= i){
                    getline(ss, line,'\n');
                    count_line++;
                }
                cout << line << endl;
            }
        }
    } else {
        std::cerr << "Wrong usage, try using: ipmt --help";
        return -1;
    }

    return 0;
}