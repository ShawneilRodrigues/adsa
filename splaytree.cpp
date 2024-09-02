#include <iostream>
using namespace std;

struct Node {
    int key;
    Node *left, *right;

    Node(int key) : key(key), left(nullptr), right(nullptr) {}
};

class SplayTree {
private:
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        return y;
    }

    Node* splay(Node* root, int key) {
        if (root == nullptr || root->key == key)
            return root;

        // Key lies in left subtree
        if (root->key > key) {
            if (root->left == nullptr)
                return root;

            // Zig-Zig (Left Left)
            if (root->left->key > key) {
                root->left->left = splay(root->left->left, key);
                root = rightRotate(root);
            }
            // Zig-Zag (Left Right)
            else if (root->left->key < key) {
                root->left->right = splay(root->left->right, key);
                if (root->left->right != nullptr)
                    root->left = leftRotate(root->left);
            }

            return (root->left == nullptr) ? root : rightRotate(root);
        }
        // Key lies in right subtree
        else {
            if (root->right == nullptr)
                return root;

            // Zag-Zig (Right Left)
            if (root->right->key > key) {
                root->right->left = splay(root->right->left, key);
                if (root->right->left != nullptr)
                    root->right = rightRotate(root->right);
            }
            // Zag-Zag (Right Right)
            else if (root->right->key < key) {
                root->right->right = splay(root->right->right, key);
                root = leftRotate(root);
            }

            return (root->right == nullptr) ? root : leftRotate(root);
        }
    }

    Node* insert(Node* root, int key) {
        if (root == nullptr)
            return new Node(key);

        root = splay(root, key);

        if (root->key == key)
            return root;

        Node* newNode = new Node(key);

        if (root->key > key) {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
        }
        else {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
        }

        return newNode;
    }

    Node* deleteNode(Node* root, int key) {
        if (root == nullptr)
            return nullptr;

        root = splay(root, key);

        if (root->key != key)
            return root;

        if (root->left == nullptr) {
            Node* temp = root;
            root = root->right;
            delete temp;
        }
        else {
            Node* temp = root;
            root = splay(root->left, key);
            root->right = temp->right;
            delete temp;
        }

        return root;
    }

    void preOrder(Node* root) {
        if (root != nullptr) {
            cout << root->key << " ";
            preOrder(root->left);
            preOrder(root->right);
        }
    }

public:
    Node* root;

    SplayTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void deleteKey(int key) {
        root = deleteNode(root, key);
    }

    void search(int key) {
        root = splay(root, key);
    }

    void preOrderTraversal() {
        preOrder(root);
        cout << endl;
    }
};

int main() {
    SplayTree tree;
    tree.insert(100);
    tree.insert(50);
    tree.insert(200);
    tree.insert(40);
    tree.insert(30);
    tree.insert(20);

    cout << "Preorder traversal after insertion: ";
    tree.preOrderTraversal();

    tree.search(50);
    cout << "Preorder traversal after searching 50: ";
    tree.preOrderTraversal();

    tree.deleteKey(20);
    cout << "Preorder traversal after deleting 20: ";
    tree.preOrderTraversal();

    return 0;
}
