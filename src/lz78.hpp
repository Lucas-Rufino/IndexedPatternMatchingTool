#include <string>
#include <vector>
#include <cmath>

#include "bstream.hpp"
#include "sufix.hpp"

struct encoder {

    obstream obits;
    trie t;
    int k;

    encoder(std::string filename, int buflen = 1000) {
        obits.open(filename, buflen);
        k = 0;
    }

    void encode(std::vector<unsigned char>& txt) {
        int len = txt.size();
        for(int i=0 ; i<len ; i++) {
            i = t.add(txt, i);
            int locus = t.locus;
            int count = t.count;

            k = ceil(log2(ceil(log2(count + 1)) + 1));
            int size = (locus == 0 ? 1 : ceil(log2(locus + 1)));

            obits.write(size, k);
            obits.write(locus, size);
            obits.write(txt[i], 8);
        }
    }

    void close() {
        obits.write(0, k);
        obits.close();
    }
};

struct decoder {

    #define get(p, n) std::get<n>(p)

    std::vector<unsigned char> txt;
    ibstream ibits;
    rtrie tree;
    int has;
    int k;

    decoder(std::string filename, int buflen = 1000, int local = -1) {
        k = ceil(log2(ceil(log2(tree.count + 1)) + 1));
        ibits.open(filename, buflen, local);
        has = ibits.read(k);
    }

    std::vector<unsigned char> decode(int len) {
        while((txt.size() < len) && has) {
            int value = ibits.read(has);

            unsigned char letter = (unsigned char)(ibits.read(8));

            tree.apply(txt, value, letter);

            k = ceil(log2(ceil(log2(tree.count + 1)) + 1));

            has = ibits.read(k);

        }

        int size = txt.size();
        unsigned char* data = txt.data();
        int min = (len<size ? len : size);
        std::vector<unsigned char> info(data, data + min);
        txt = std::vector<unsigned char>(data + min, data + txt.size());
        return info;
    }
    
    void close() {
        ibits.close();
    }
    #undef get
};