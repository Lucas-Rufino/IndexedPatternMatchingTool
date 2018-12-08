#include <vector>
#include <utility>
#include <string>
enum sarray_compress {normal, smaller_ints};
struct sarray {
public:
    std::string text;
    std::vector<int> sarray;

private:
    void addToArray(std::vector<char>& vec, int value, sarray_compress strategy){
        if(strategy == normal){
            for(int i=0; i<4; i++)
                vec.push_back((value >> (i*8))&255);
        } else {
            std::cerr<< "NOTIMPLEMENTED";
        }
    }

    int getFromArray(std::vector<char>& vec, int offset, int position, sarray_compress strategy){
        if(strategy == normal){
            int value=0;
            for(int i=0; i<4; i++)
                value+=((((unsigned char)vec[offset + position * 4 + i]) << (i*8)));
            return value;
        } else {
            std::cerr<< "NOTIMPLEMENTED";
            return -1;
        }
    }

public:
    void print(){
        std::cout<<'"'<<text<<'"'<<std::endl;
        for(auto x:sarray){
            std::cout<<x<<" ";
        }
        std::cout<<std::endl;
    }

    std::vector<char> toBytes(){
        std::vector<char> ret;
        for(int i=0;i<4;i++)
            ret.push_back(255&(text.size()>>(i*8)));
        for (auto ch: text){
            ret.push_back(ch);
        }
        for (auto ch: sarray){
            addToArray(ret, ch, sarray_compress::normal);
        }
        // for(auto x:ret)std::cout<<"["<<(x)<<"]";
        // std::cout<<"\n";
        // for(auto x:ret)std::cout<<"("<<((int) x)<<")";
        return ret;
    }

    void fromBytes(std::vector<char> bytes){
        int sze = 0;
        for(int i=0;i<4;i++)
            sze += ((unsigned char)bytes[i])<<(i*8);
        text.resize(sze);
        for(int i=0;i<sze;i++)text[i]=bytes[i+4];
        text.resize(sze);
        for(int i=0;i<sze;i++)sarray[i]=getFromArray(bytes, 4 + sze, i, sarray_compress::normal);
    }

    void fromText(std::string txt){
        text=txt;
        std::vector<int> data;
        int txtSize = txt.size();
        if(txtSize == 1) {
            sarray.resize(1);
            sarray[0] = 0;
        } else {
            sarray.resize(txtSize);
            data.resize(txtSize);
            std::vector<std::pair<std::pair<int, int>, int> > base;
            base.resize(txtSize);

            for(int i = 0; i < txtSize; i++){
                base[i] = {{txt[i], 0}, i};
                data[i] = txt[i];
            }

            for(int bs = 1; bs < txtSize; bs *= 2){
                for(int i = 0; i < txtSize; i++) {
                    if(base[i].second + bs >= txtSize) base[i].first.second = -1;
                    else base[i].first.second = data[base[i].second + bs];
                }
                sort(base.begin(), base.end());
                int ind = 0;
                for(int i = 0; i < txtSize; i++) {
                    if(i){
                        if(base[i].first != base[i-1].first) ind++;
                    }
                    data[base[i].second] = ind;
                    base[i].first.first = ind;
                }
            }
            for(int i = 0; i < txtSize; i++) {
                sarray[data[i]] = i;
            }
        }
    }

private:
    int getPosBin(std::string pattern, int i, int L, int R, bool equals){
        int l=L, r=R;
        while(l<r-1){
            int m=(l+r)/2;
            int M = sarray[m];
            if(M+i < text.size()){
                if(text[M+i]>pattern[i] || (equals&&text[M+i]==pattern[i])){
                    r=m;
                } else {
                    l=m;
                }
            } else {
                r=m;
            }
        }
        int M = sarray[l];
        if(text[M+i]>pattern[i] || (equals&&text[M+i]==pattern[i]))r=l;
        return r;
    }

public:
    std::vector<int> search(std::string pattern) {
        int L = 0, R = text.size() - 1;
        for(int i = 0, sz=pattern.size(); i < sz; i++){
            L=getPosBin(pattern,i,L,R,true);
            R=getPosBin(pattern,i,L,R,false);
        }
        std::vector<int> ret;
        while(L<R){
            ret.push_back(sarray[L++]);
        }
        return ret;
    }
};