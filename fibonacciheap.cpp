#include <iostream>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <climits> // Add this line to include the <climits> header file
using namespace std;

struct Node {
    int key;
    int degree;
    Node *parent;
    Node *child;
    Node *left;
    Node *right;
    bool mark;

    Node(int val) {
        key = val;
        degree = 0;
        parent = child = nullptr;
        left = right = this;
        mark = false;
    }
};

class FibonacciHeap {
private:
    Node* minNode;
    int nodeCount;

    void insertNodeIntoRootList(Node* node) {
        if (!minNode) {
            minNode = node;
        } else {
            node->left = minNode;
            node->right = minNode->right;
            minNode->right->left = node;
            minNode->right = node;
            if (node->key < minNode->key) {
                minNode = node;
            }
        }
    }

    void consolidate() {
        int maxDegree = static_cast<int>(log2(nodeCount)) + 1;
        vector<Node*> degreeTable(maxDegree, nullptr);

        Node* start = minNode;
        Node* w = minNode;
        do {
            Node* x = w;
            int degree = x->degree;
            while (degreeTable[degree]) {
                Node* y = degreeTable[degree];
                if (x->key > y->key) swap(x, y);
                link(y, x);
                degreeTable[degree] = nullptr;
                degree++;
            }
            degreeTable[degree] = x;
            w = w->right;
        } while (w != start);

        minNode = nullptr;
        for (auto& node : degreeTable) {
            if (node) {
                if (!minNode) {
                    minNode = node;
                } else {
                    insertNodeIntoRootList(node);
                    if (node->key < minNode->key) {
                        minNode = node;
                    }
                }
            }
        }
    }

    void link(Node* y, Node* x) {
        // Remove y from the root list
        y->left->right = y->right;
        y->right->left = y->left;

        // Make y a child of x
        y->parent = x;
        if (!x->child) {
            x->child = y;
            y->right = y->left = y;
        } else {
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        x->degree++;
        y->mark = false;
    }

    void cut(Node* x, Node* y) {
        if (x->right == x) {
            y->child = nullptr;
        } else {
            x->left->right = x->right;
            x->right->left = x->left;
            if (y->child == x) {
                y->child = x->right;
            }
        }
        y->degree--;
        insertNodeIntoRootList(x);
        x->parent = nullptr;
        x->mark = false;
    }

    void cascadingCut(Node* y) {
        Node* z = y->parent;
        if (z) {
            if (!y->mark) {
                y->mark = true;
            } else {
                cut(y, z);
                cascadingCut(z);
            }
        }
    }

public:
    FibonacciHeap() {
        minNode = nullptr;
        nodeCount = 0;
    }

    Node* insert(int key) {
        Node* node = new Node(key);
        insertNodeIntoRootList(node);
        nodeCount++;
        return node;
    }

    void merge(FibonacciHeap* other) {
        if (!other->minNode) return;
        if (!minNode) {
            minNode = other->minNode;
        } else {
            Node* tempLeft = minNode->left;
            minNode->left = other->minNode->left;
            other->minNode->left->right = minNode;
            other->minNode->left = tempLeft;
            tempLeft->right = other->minNode;
            if (other->minNode->key < minNode->key) {
                minNode = other->minNode;
            }
        }
        nodeCount += other->nodeCount;
    }

    Node* getMin() {
        return minNode;
    }

    int extractMin() {
        Node* oldMin = minNode;
        if (oldMin) {
            if (oldMin->child) {
                Node* child = oldMin->child;
                do {
                    Node* next = child->right;
                    insertNodeIntoRootList(child);
                    child->parent = nullptr;
                    child = next;
                } while (child != oldMin->child);
            }

            oldMin->left->right = oldMin->right;
            oldMin->right->left = oldMin->left;

            if (oldMin == oldMin->right) {
                minNode = nullptr;
            } else {
                minNode = oldMin->right;
                consolidate();
            }
            nodeCount--;
        }
        int minValue = oldMin->key;
        delete oldMin;
        return minValue;
    }

    void decreaseKey(Node* node, int newKey) {
        if (newKey > node->key) {
            cerr << "New key is greater than current key" << endl;
            return;
        }

        node->key = newKey;
        Node* parent = node->parent;

        if (parent && node->key < parent->key) {
            cut(node, parent);
            cascadingCut(parent);
        }

        if (node->key < minNode->key) {
            minNode = node;
        }
    }

    void deleteNode(Node* node) {
        decreaseKey(node, INT_MIN);
        extractMin();
    }
};

int main() {
    FibonacciHeap fibHeap;

    // Insertion
    fibHeap.insert(10);
    fibHeap.insert(20);
    fibHeap.insert(30);

    // Get minimum element
    cout << "Min element: " << fibHeap.getMin()->key << endl;

    // Extract minimum element
    cout << "Extracted Min element: " << fibHeap.extractMin() << endl;

    // Get new minimum
    cout << "New Min element: " << fibHeap.getMin()->key << endl;

    return 0;
}
