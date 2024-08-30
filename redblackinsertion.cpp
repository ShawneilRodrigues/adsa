#include <iostream>

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int data) : data(data) {
        color = RED;
        left = right = parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;

    void rotateLeft(Node* node) {
        Node* rightChild = node->right;
        node->right = rightChild->left;

        if (node->right != nullptr) {
            node->right->parent = node;
        }

        rightChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = rightChild;
        } else if (node == node->parent->left) {
            node->parent->left = rightChild;
        } else {
            node->parent->right = rightChild;
        }

        rightChild->left = node;
        node->parent = rightChild;
    }

    void rotateRight(Node* node) {
        Node* leftChild = node->left;
        node->left = leftChild->right;

        if (node->left != nullptr) {
            node->left->parent = node;
        }

        leftChild->parent = node->parent;

        if (node->parent == nullptr) {
            root = leftChild;
        } else if (node == node->parent->left) {
            node->parent->left = leftChild;
        } else {
            node->parent->right = leftChild;
        }

        leftChild->right = node;
        node->parent = leftChild;
    }

    void fixViolation(Node* node) {
        Node* parent = nullptr;
        Node* grandparent = nullptr;

        while (node != root && node->color != BLACK && node->parent->color == RED) {
            parent = node->parent;
            grandparent = node->parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            } else {
                Node* uncle = grandparent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

public:
    RedBlackTree() : root(nullptr) {}

    void insert(int data) {
        Node* newNode = new Node(data);

        Node* parent = nullptr;
        Node* current = root;

        while (current != nullptr) {
            parent = current;

            if (data < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        newNode->parent = parent;

        if (parent == nullptr) {
            root = newNode;
        } else if (data < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        fixViolation(newNode);
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr) {
            return;
        }

        inorderTraversal(node->left);
        std::cout << node->data << " ";
        inorderTraversal(node->right);
    }

    void printInorder() {
        inorderTraversal(root);
        std::cout << std::endl;
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);
    tree.insert(2);
    tree.insert(6);
    tree.insert(13);

    std::cout << "Inorder traversal of the Red-Black Tree: ";
    tree.printInorder();

    return 0;
}