#include <string>
#include <vector>
#include <cmath>

#include "bstream.hpp"
#include "sufix.hpp"

struct encoder {

    obstream obits;
    int thr = 1;
    int k = 1;
    trie t;

    encoder(std::string filename, int buflen = 1000) {
        obits.open(filename, buflen);
        thr = 1;
        k = 1;
    }

    void encode(std::string& txt) {
        int len = txt.size();
        int i = 0;
        
        while(i < len) {
            t.add(txt, i);
            int index = t.index;
            int value = t.value;

            int size = (value == 0 ? 1 : ceil(log2(value + 1)));

            obits.write(size, k);
            obits.write(value, size);
            obits.write(txt[index], 8);

            if(thr == size) {
                thr = (thr << 1) | 1;
                k++;
            }

            i = index + 1;
        }
    }

    void close() {
        obits.write(0, k);
        obits.close();
    }
};

struct decoder {

    #define get(p, n) std::get<n>(p)

    std::vector<char> txt;
    ibstream ibits;
    rtrie tree;
    int thr;
    int has;
    int k;

    decoder(std::string filename, int buflen = 1000) {
        ibits.open(filename, buflen);
        thr = 1;
        k = 1;
        has = ibits.read(k);
    }

    std::string decode(int len) {
        while((txt.size() < len) && has) {
            if(thr == has) {
                thr = (thr << 1) | 1;
                k++;
            }

            int value = ibits.read(has);
            char letter = char(ibits.read(8));
            tree.apply(txt, value, letter);
            has = ibits.read(k);
        }

        int size = txt.size();
        char* data = txt.data();
        int min = (len<size ? len : size);
        std::string info(data, data + min);
        txt = std::vector<char>(data + min, data + txt.size());
        return info;
    }
    
    void close() {
        ibits.close();
    }
    #undef get
};