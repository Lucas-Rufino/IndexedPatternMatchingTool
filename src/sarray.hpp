#include <vector>
#include <utility>
#include <string>
#include <set>

enum sarray_compress {normal, smaller_ints};

struct sarray {
public:
    std::string text;
    std::vector<int> sarray;
    std::vector<int> bl;

private:
    void addToArray(std::vector<char>& vec, int value, sarray_compress strategy){
        if(strategy == normal){
            for(int i=0; i<4; i++)
                vec.push_back((value >> (i*8))&255);
        } else {
            std::cerr<< "NOTIMPLEMENTED";
        }
    }

    int toLine(int x){
       int aux = lower_bound(bl.begin(),bl.end(),x) - bl.begin();
       //printf("%d %d %d\n", x, x, aux);
       return aux;
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
    void print(bool full=true){
        std::cout<<'"'<<text<<'"'<<std::endl;
        for(auto x:sarray){
            std::cout<<x<<" ";
        }
        std::cout<<std::endl;
        if(full){
            for(int i=0;i<text.size(); i++){
                std::cout<<i<<" | "<<text.substr(sarray[i],text.size()-sarray[i])<<std::endl;

            }
        }
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
        bl.clear();
        for(int i=0;i<sze;i++){
            text[i]=bytes[i+4];
            if(text[i] =='\n' || text[i]=='\r')
                bl.push_back(i);
        }
        bl.push_back(sze);
        sarray.resize(sze);
        for(int i=0;i<sze;i++)sarray[i]=getFromArray(bytes, 4 + sze, i, sarray_compress::normal);
    }

    void fromText(std::string txt){
        text=txt;
        std::vector<int> data;
        int txtSize = txt.size();
        if(txtSize == 1) {
            sarray.resize(1);
            sarray[0] = 0;
            bl.clear();
            if(text[0]=='\n' || text[1]=='\r')bl.push_back(0);
            bl.push_back(1);
        } else {
            sarray.resize(txtSize);
            data.resize(txtSize);
            std::vector<std::pair<std::pair<int, int>, int> > base;
            base.resize(txtSize);

            for(int i = 0; i < txtSize; i++){
                base[i] = {{txt[i], 0}, i};
                data[i] = txt[i];
                if(txt[i] =='\n' || txt[i]=='\r')
                    bl.push_back(i);
            }
            bl.push_back(txtSize);

            for(int bs = 1; bs < txtSize; bs *= 2){
                for(int i = 0; i < txtSize; i++) {
                    if(base[i].second + bs >= txtSize) base[i].first.second = -200;
                    else base[i].first.second = data[base[i].second + bs];
                }
                sort(base.begin(), base.end());
                int ind = 0;
                for(int i = 0; i < txtSize; i++) {
                    auto aux = base[i].first;
                    data[base[i].second] = ind;
                    base[i].first.first = ind;
                    if(i<txtSize-1){
                        if(aux != base[i+1].first) ind++;
                    }
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
    std::vector<int> search(std::string pattern, bool by_lines=true) {
        //print();
        int L = 0, R = text.size() - 1;
        for(int i = 0, sz=pattern.size(); i < sz; i++){
            L=getPosBin(pattern,i,L,R,true);
            R=getPosBin(pattern,i,L,R,false);
        }
        std::vector<int> ret;
        while(L<R){
            ret.push_back(sarray[L++]);
        }
        //printf("%d -- %d\n",L,R);
        if(by_lines){
            std::set<int> lines;
            for (auto x: ret)lines.insert(toLine(x));
            ret.clear();
            for (auto x: lines)ret.push_back(x);
        }
        return ret;
    }
};