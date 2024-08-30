#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data) {
        this->data = data;
        left = right = parent = nullptr;
        this->color = RED;
    }
};

class RedBlackTree {
private:
    Node *root;

protected:
    void rotateLeft(Node *&pt) {
        Node *pt_right = pt->right;

        pt->right = pt_right->left;

        if (pt->right != nullptr)
            pt->right->parent = pt;

        pt_right->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_right;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_right;
        else
            pt->parent->right = pt_right;

        pt_right->left = pt;
        pt->parent = pt_right;
    }

    void rotateRight(Node *&pt) {
        Node *pt_left = pt->left;

        pt->left = pt_left->right;

        if (pt->left != nullptr)
            pt->left->parent = pt;

        pt_left->parent = pt->parent;

        if (pt->parent == nullptr)
            root = pt_left;
        else if (pt == pt->parent->left)
            pt->parent->left = pt_left;
        else
            pt->parent->right = pt_left;

        pt_left->right = pt;
        pt->parent = pt_left;
    }

    void fixViolation(Node *&pt) {
        Node *parent_pt = nullptr;
        Node *grand_parent_pt = nullptr;

        while ((pt != root) && (pt->color != BLACK) &&
               (pt->parent->color == RED)) {

            parent_pt = pt->parent;
            grand_parent_pt = pt->parent->parent;

            if (parent_pt == grand_parent_pt->left) {
                Node *uncle_pt = grand_parent_pt->right;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->right) {
                        rotateLeft(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateRight(grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            } else {
                Node *uncle_pt = grand_parent_pt->left;

                if (uncle_pt != nullptr && uncle_pt->color == RED) {
                    grand_parent_pt->color = RED;
                    parent_pt->color = BLACK;
                    uncle_pt->color = BLACK;
                    pt = grand_parent_pt;
                } else {
                    if (pt == parent_pt->left) {
                        rotateRight(parent_pt);
                        pt = parent_pt;
                        parent_pt = pt->parent;
                    }
                    rotateLeft(grand_parent_pt);
                    swap(parent_pt->color, grand_parent_pt->color);
                    pt = parent_pt;
                }
            }
        }

        root->color = BLACK;
    }

    Node *BSTReplace(Node *x) {
        if (x->left && x->right)
            return minValueNode(x->right);

        if (!x->left && !x->right)
            return nullptr;

        if (x->left)
            return x->left;
        else
            return x->right;
    }

    void fixDoubleBlack(Node *x) {
        if (x == root)
            return;

        Node *sibling = getSibling(x), *parent = x->parent;
        if (!sibling) {
            fixDoubleBlack(parent);
        } else {
            if (sibling->color == RED) {
                parent->color = RED;
                sibling->color = BLACK;
                if (sibling == parent->left) {
                    rotateRight(parent);
                } else {
                    rotateLeft(parent);
                }
                fixDoubleBlack(x);
            } else {
                if (hasRedChild(sibling)) {
                    if (sibling->left && sibling->left->color == RED) {
                        if (sibling == parent->left) {
                            sibling->left->color = sibling->color;
                            sibling->color = parent->color;
                            rotateRight(parent);
                        } else {
                            sibling->left->color = parent->color;
                            rotateRight(sibling);
                            rotateLeft(parent);
                        }
                    } else {
                        if (sibling == parent->left) {
                            sibling->right->color = parent->color;
                            rotateLeft(sibling);
                            rotateRight(parent);
                        } else {
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            rotateLeft(parent);
                        }
                    }
                    parent->color = BLACK;
                } else {
                    sibling->color = RED;
                    if (parent->color == BLACK)
                        fixDoubleBlack(parent);
                    else
                        parent->color = BLACK;
                }
            }
        }
    }

    void deleteNode(Node *v) {
        Node *u = BSTReplace(v);

        bool uvBlack = ((u == nullptr || u->color == BLACK) && (v->color == BLACK));
        Node *parent = v->parent;

        if (u == nullptr) {
            if (v == root) {
                root = nullptr;
            } else {
                if (uvBlack) {
                    fixDoubleBlack(v);
                } else {
                    if (getSibling(v) != nullptr)
                        getSibling(v)->color = RED;
                }

                if (v == parent->left)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
            delete v;
            return;
        }

        if (v->left == nullptr || v->right == nullptr) {
            if (v == root) {
                v->data = u->data;
                v->left = v->right = nullptr;
                delete u;
            } else {
                if (v == parent->left)
                    parent->left = u;
                else
                    parent->right = u;
                delete v;
                u->parent = parent;
                if (uvBlack) {
                    fixDoubleBlack(u);
                } else {
                    u->color = BLACK;
                }
            }
            return;
        }

        swap(u->data, v->data);
        deleteNode(u);
    }

public:
    RedBlackTree() { root = nullptr; }

    void insert(const int &data) {
        Node *pt = new Node(data);
        root = BSTInsert(root, pt);
        fixViolation(pt);
    }

    void deleteValue(int data) {
        Node *v = search(root, data);

        if (v == nullptr)
            return;

        deleteNode(v);
    }

    void inorder() { inorderHelper(root); }

private:
    void inorderHelper(Node *root) {
        if (root == nullptr)
            return;

        inorderHelper(root->left);
        cout << root->data << " ";
        inorderHelper(root->right);
    }

    Node *BSTInsert(Node *root, Node *pt) {
        if (root == nullptr)
            return pt;

        if (pt->data < root->data) {
            root->left = BSTInsert(root->left, pt);
            root->left->parent = root;
        } else if (pt->data > root->data) {
            root->right = BSTInsert(root->right, pt);
            root->right->parent = root;
        }

        return root;
    }

    Node *search(Node *root, int data) {
        if (root == nullptr || root->data == data)
            return root;

        if (data < root->data)
            return search(root->left, data);
        else
            return search(root->right, data);
    }

    Node *getSibling(Node *x) {
        if (x->parent == nullptr)
            return nullptr;

        if (x == x->parent->left)
            return x->parent->right;

        return x->parent->left;
    }

    Node *minValueNode(Node *node) {
        Node *ptr = node;

        while (ptr->left != nullptr)
            ptr = ptr->left;

        return ptr;
    }

    bool hasRedChild(Node *node) {
        return (node->left && node->left->color == RED) || (node->right && node->right->color == RED);
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);

    cout << "Inorder traversal before deletion: ";
    tree.inorder();
    cout << endl;

    tree.deleteValue(20);

    cout << "Inorder traversal after deletion: ";
    tree.inorder();
    cout << endl;

    return 0;
}
