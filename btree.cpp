#include <iostream>
#include <vector>
using namespace std;

class BTreeNode {
public:
    vector<int> keys;
    vector<BTreeNode*> children;
    bool isLeaf;

    BTreeNode(bool isLeaf);
};

BTreeNode::BTreeNode(bool isLeaf) {
    this->isLeaf = isLeaf;
}

class BTree {
    BTreeNode* root;
    int t; // Minimum degree (t = 2)

public:
    BTree(int t);
    void insert(int key);
    void printTree();

private:
    void insertNonFull(BTreeNode* x, int key);
    void splitChild(BTreeNode* x, int i);
    void printNode(BTreeNode* node, string prefix, int childNum);
};

BTree::BTree(int t) {
    root = new BTreeNode(true);
    this->t = t;
}

void BTree::insert(int key) {
    BTreeNode* r = root;
    if (r->keys.size() == 2 * t - 1) {
        BTreeNode* s = new BTreeNode(false);
        root = s;
        s->children.push_back(r);
        splitChild(s, 0);
        insertNonFull(s, key);
    } else {
        insertNonFull(r, key);
    }
}

void BTree::insertNonFull(BTreeNode* x, int key) {
    int i = x->keys.size() - 1;

    if (x->isLeaf) {
        x->keys.push_back(0);
        while (i >= 0 && key < x->keys[i]) {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = key;
    } else {
        while (i >= 0 && key < x->keys[i]) {
            i--;
        }
        i++;
        if (x->children[i]->keys.size() == 2 * t - 1) {
            splitChild(x, i);
            if (key > x->keys[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], key);
    }
}

void BTree::splitChild(BTreeNode* x, int i) {
    BTreeNode* y = x->children[i];
    BTreeNode* z = new BTreeNode(y->isLeaf);
    x->children.insert(x->children.begin() + i + 1, z);
    x->keys.insert(x->keys.begin() + i, y->keys[t - 1]);

    for (int j = 0; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
    }

    if (!y->isLeaf) {
        for (int j = 0; j < t; j++) {
            z->children.push_back(y->children[j + t]);
        }
        y->children.resize(t);
    }

    y->keys.resize(t - 1);
}

void BTree::printTree() {
    printNode(root, "", 0);
}

void BTree::printNode(BTreeNode* node, string prefix, int childNum) {
    if (node == root) {
        cout << "root - " << node->keys[0] << endl;
    } else {
        cout << prefix << childNum << " child: ";
        for (int key : node->keys) {
            cout << key << " ";
        }
        cout << endl;
    }

    if (!node->isLeaf) {
        for (int i = 0; i < node->children.size(); i++) {
            printNode(node->children[i], prefix + (childNum > 0 ? "  " : ""), i + 1);
        }
    }
}

int main() {
    BTree tree(2);
    int input[] = {2, 8, 7, 3, 12, 5, 9, 6, 16, 4, 21};
    
    for (int key : input) {
        tree.insert(key);
    }

    tree.printTree();
    
    return 0;
}
