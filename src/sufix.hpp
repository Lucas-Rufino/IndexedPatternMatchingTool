#include <cstring>
#include <vector>
#include <string>

struct trie {

    struct node {
        int value;
        node** stree;

        node(int value = -1, node** stree = NULL) {
            this->value = value;
            this->stree = stree;
        }
    };

    int count;
    int value;
    int index;
    node** tree;

    trie() {
        count = 0;
        value = 0;
        index = 0;
        tree = new node*[256];
        memset(tree, 0, 256 * sizeof(node*));
    }

    void add(std::string& s, int i) {
        int locus = 0;
        node** stree = tree;
        for(int l=s.size() ; i<l ; i++) {
            node* n = stree[s[i]];
            if(n == NULL) {
                stree[s[i]] = new node(++count);
                value = locus;
                index = i;
                return;
            } else {
                if(n->stree == NULL) {
                    n->stree = new node*[256];
                    memset(n->stree, 0, 256 * sizeof(node*));
                }
                stree = n->stree;
                locus = n->value;
            }
        }
        value = locus;
        index = i;
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

    rtrie() {
        tree.push_back(new node(0, 0, '\0'));
    }

    void apply(std::vector<char>& txt, int index, char letter) {
        int size = tree[index]->size;

        if(letter) {
            node* n = new node(++size, index, letter);
            index = tree.size();
            tree.push_back(n);
        }

        txt.insert(txt.end(), size, '\0');
        int locus = txt.size() - 1;
        for(int i=index ; i>0 ; i=tree[i]->index) {
            txt[locus--] = tree[i]->letter;
        }
    }
};