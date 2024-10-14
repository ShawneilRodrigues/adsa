#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BPlusTreeNode {
public:
    bool isLeaf;
    vector<int> keys;
    vector<BPlusTreeNode*> children;

    BPlusTreeNode(bool isLeaf) {
        this->isLeaf = isLeaf;
    }
};

class BPlusTree {
private:
    BPlusTreeNode* root;
    int t;  // Minimum degree

    // Function to split a child
    void splitChild(BPlusTreeNode* parent, int i) {
        BPlusTreeNode* nodeToSplit = parent->children[i];
        BPlusTreeNode* newNode = new BPlusTreeNode(nodeToSplit->isLeaf);

        parent->keys.insert(parent->keys.begin() + i, nodeToSplit->keys[t - 1]);
        parent->children.insert(parent->children.begin() + i + 1, newNode);

        newNode->keys.assign(nodeToSplit->keys.begin() + t, nodeToSplit->keys.end());
        nodeToSplit->keys.resize(t - 1);

        if (!nodeToSplit->isLeaf) {
            newNode->children.assign(nodeToSplit->children.begin() + t, nodeToSplit->children.end());
            nodeToSplit->children.resize(t);
        }
    }

    // Insert in non-full node
    void insertNonFull(BPlusTreeNode* node, int key) {
        if (node->isLeaf) {
            node->keys.push_back(key);
            sort(node->keys.begin(), node->keys.end());
        } else {
            int i = node->keys.size() - 1;
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;
            if (node->children[i]->keys.size() == 2 * t - 1) {
                splitChild(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], key);
        }
    }

    // Merge two nodes
    void merge(BPlusTreeNode* parent, int idx) {
        BPlusTreeNode* child1 = parent->children[idx];
        BPlusTreeNode* child2 = parent->children[idx + 1];

        // Pull down the key from the parent to the child1
        child1->keys.push_back(parent->keys[idx]);

        // Transfer all the keys from child2 to child1
        child1->keys.insert(child1->keys.end(), child2->keys.begin(), child2->keys.end());

        // Transfer all the child pointers from child2 to child1 if not a leaf
        if (!child1->isLeaf) {
            child1->children.insert(child1->children.end(), child2->children.begin(), child2->children.end());
        }

        // Remove the key from parent
        parent->keys.erase(parent->keys.begin() + idx);
        parent->children.erase(parent->children.begin() + idx + 1);

        // Free memory of child2
        delete child2;
    }

    // Borrow key from the left sibling
    void borrowFromPrev(BPlusTreeNode* parent, int idx) {
        BPlusTreeNode* child = parent->children[idx];
        BPlusTreeNode* sibling = parent->children[idx - 1];

        child->keys.insert(child->keys.begin(), parent->keys[idx - 1]);
        if (!child->isLeaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }
        parent->keys[idx - 1] = sibling->keys.back();
        sibling->keys.pop_back();
    }

    // Borrow key from the right sibling
    void borrowFromNext(BPlusTreeNode* parent, int idx) {
        BPlusTreeNode* child = parent->children[idx];
        BPlusTreeNode* sibling = parent->children[idx + 1];

        child->keys.push_back(parent->keys[idx]);
        if (!child->isLeaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }
        parent->keys[idx] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
    }

    // Remove key from the leaf node
    void removeFromLeaf(BPlusTreeNode* node, int idx) {
        node->keys.erase(node->keys.begin() + idx);
    }

    // Remove key from the internal node
    void removeFromNonLeaf(BPlusTreeNode* node, int idx) {
        int key = node->keys[idx];

        if (node->children[idx]->keys.size() >= t) {
            BPlusTreeNode* predecessor = node->children[idx];
            while (!predecessor->isLeaf) {
                predecessor = predecessor->children.back();
            }
            int predKey = predecessor->keys.back();
            node->keys[idx] = predKey;
            remove(node->children[idx], predKey);
        } else if (node->children[idx + 1]->keys.size() >= t) {
            BPlusTreeNode* successor = node->children[idx + 1];
            while (!successor->isLeaf) {
                successor = successor->children.front();
            }
            int succKey = successor->keys.front();
            node->keys[idx] = succKey;
            remove(node->children[idx + 1], succKey);
        } else {
            merge(node, idx);
            remove(node->children[idx], key);
        }
    }

    // Recursive remove function
    void remove(BPlusTreeNode* node, int key) {
        int idx = 0;
        while (idx < node->keys.size() && node->keys[idx] < key) {
            idx++;
        }

        if (idx < node->keys.size() && node->keys[idx] == key) {
            if (node->isLeaf) {
                removeFromLeaf(node, idx);
            } else {
                removeFromNonLeaf(node, idx);
            }
        } else {
            if (node->isLeaf) {
                cout << "Key " << key << " not found in the tree.\n";
                return;
            }
            bool flag = (idx == node->keys.size());
            if (node->children[idx]->keys.size() < t) {
                if (idx > 0 && node->children[idx - 1]->keys.size() >= t) {
                    borrowFromPrev(node, idx);
                } else if (idx < node->keys.size() && node->children[idx + 1]->keys.size() >= t) {
                    borrowFromNext(node, idx);
                } else {
                    if (idx != node->keys.size()) {
                        merge(node, idx);
                    } else {
                        merge(node, idx - 1);
                    }
                }
            }
            if (flag && idx > node->keys.size()) {
                remove(node->children[idx - 1], key);
            } else {
                remove(node->children[idx], key);
            }
        }
    }

public:
    BPlusTree(int t) {
        this->t = t;
        root = new BPlusTreeNode(true);
    }

    void insert(int key) {
        BPlusTreeNode* r = root;
        if (r->keys.size() == 2 * t - 1) {
            BPlusTreeNode* newNode = new BPlusTreeNode(false);
            newNode->children.push_back(r);
            splitChild(newNode, 0);
            root = newNode;
            insertNonFull(root, key);
        } else {
            insertNonFull(r, key);
        }
    }

    void remove(int key) {
        if (!root) {
            cout << "The tree is empty.\n";
            return;
        }
        remove(root, key);
        if (root->keys.empty()) {
            BPlusTreeNode* temp = root;
            if (!root->isLeaf) {
                root = root->children[0];
            } else {
                root = nullptr;
            }
            delete temp;
        }
    }

    void printTree(BPlusTreeNode* node, int level) {
        if (!node) return;
        cout << "Level " << level << " Keys: ";
        for (int key : node->keys) {
            cout << key << " ";
        }
        cout << "\n";
        if (!node->isLeaf) {
            for (BPlusTreeNode* child : node->children) {
                printTree(child, level + 1);
            }
        }
    }

    void print() {
        printTree(root, 0);
    }
};

int main() {
    BPlusTree bpt(3);
    int keysToInsert[] = {10, 20, 5, 6, 12, 30, 7, 17};

    for (int key : keysToInsert) {
        bpt.insert(key);
    }

    cout << "B+ Tree after insertion:\n";
    bpt.print();

    // Deleting keys
    cout << "\nDeleting key 6:\n";
    bpt.remove(6);
    bpt.print();

    cout << "\nDeleting key 20:\n";
    bpt.remove(20);
    bpt.print();

    return 0;
}
