#include <cstring>
#include <vector>
#include <string>

struct trie {

    struct node {
        int locus;
        node** stree;

        node(int locus = -1, node** stree = NULL) {
            this->locus = locus;
            this->stree = stree;
        }
    };

    int count;
    int locus;
    node** tree;

    trie() {
        count = 0;
        locus = 0;
        tree = new node*[256];
        memset(tree, 0, 256 * sizeof(node*));
    }

    int add(std::vector<unsigned char>& s, int i) {
        locus = 0;
        node** stree = tree;
        for(int l=s.size() ; i<l ; i++) {
            node* n = stree[s[i]];
            if(n == NULL) {
                stree[s[i]] = new node(++count);
                break;
            } else {
                if(n->stree == NULL) {
                    n->stree = new node*[256];
                    memset(n->stree, 0, 256 * sizeof(node*));
                }
                stree = n->stree;
                locus = n->locus;
            }
        }
        return i;
    }
};

struct rtrie {

    struct node {
        int size;
        int index;
        char letter;

        node(int size, int index, char letter) {
            this->size = size;
            this->index = index;
            this->letter = letter;
        }
    };

    std::vector<node*> tree;
    int count;

    rtrie() {
        tree.push_back(new node(0, 0, '\0'));
        count = 1;
    }

    void apply(std::vector<unsigned char>& txt, int index, unsigned char letter) {
        int size = tree[index]->size;

        if(letter) {
            node* n = new node(++size, index, letter);
            index = tree.size();
            tree.push_back(n);
            count = index + 1;
        }

        txt.insert(txt.end(), size, '\0');
        int locus = txt.size() - 1;
        for(int i=index ; i>0 ; i=tree[i]->index) {
            txt[locus--] = tree[i]->letter;
        }
    }
};