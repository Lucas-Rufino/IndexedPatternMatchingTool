#include <vector>
#include <utility>
#include <string>

struct sarray {
    std::string text;
    std::vector<int> sarray;

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