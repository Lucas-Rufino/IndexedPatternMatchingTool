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

    std::vector<int> search(std::string pattern) {
        int L = 0, R = text.size() - 1;
        for(int i = 0, sz=pattern.size(); i < sz; i++){
            int l=L, r=R;
            // while(l<r-1){
            //     int m=(l+r)/2;
            //     if(m+i < text.size()){
            //         if(text[m+i]>=pattern[i]){
            //             r=m;
            //         } else {
            //             l=m;
            //         }
            //     } else {
            //         r=m;
            //     }
            // }
            // if(text[l+i]>=pattern[i])r=l;
            // L=r;
            printf("R: %d %d %d\n",r, i, sz);
        }

    }
};