#include <fstream>
#include <cstring>
#include <string>

struct obstream {

    int idx;
    unsigned int sft;

    char* buf;
    int buflen;

    std::ofstream file;

    obstream(){}

    obstream(std::string& filename, int buflen = 1000, char* buf = NULL) {
    	open(filename, buflen, buf);
    }

    void open(std::string& filename, int buflen = 1000, char* buf = NULL) {
    	sft = 0;
        idx = -1;

        this->buf = buf;
        this->buflen = buflen;
        if(this->buf == NULL) {
            this->buf = new char[buflen];
            memset(this->buf, 0, this->buflen);
        }

        file.open(filename, std::ofstream::binary);
    }

    void write(int value) {
        if(sft == 0) {
            idx++;
            if(idx == buflen) {
                file.write(buf, buflen);
                memset(buf, 0, buflen);
                idx = 0;
            }
        }
        sft = (sft - 1) % 8;
        buf[idx] |= (value == 0 ? 0 : 1) << sft;
    }

    void write(int value, int k) {
        while(k-- > 0) {
            int bit = value & (1 << k);
            write(bit);
        }
    }

    void close(){
        file.write(buf, idx + 1);
        file.close();
    }
};

struct ibstream {

    int idx;
    unsigned int sft;

    char* buf;
    int buflen;

    std::ifstream file;

    ibstream(){};

    ibstream(std::string& filename, int buflen = 1000) {
    	open(filename, buflen);
    }

    void open(std::string& filename, int buflen = 1000) {
    	sft = 0;
        idx = -1;

        this->buflen = buflen;
        this->buf = new char[buflen];
        file.open(filename, std::ofstream::binary);
        file.read(buf, buflen);
    }

    int read() {
        if(sft == 0) {
            idx++;
            if(idx == buflen) {
                file.read(buf, buflen);
                idx = 0;
            }
        }
        sft = (sft - 1) % 8;
        return ((buf[idx] & (1 << sft)) == 0 ? 0 : 1);
    }

    int read(int k) {
        int value = 0;
        while(k-- > 0) {
            value |= read() << k;
        }
        return value;
    }

    void close(){
        file.close();
    }
};