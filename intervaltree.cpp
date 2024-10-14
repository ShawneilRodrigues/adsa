#include <iostream>
#include <vector>
using namespace std;

// Structure to represent an interval
struct Interval {
    int low, high;
};

// Node structure of interval tree
struct ITNode {
    Interval *interval;
    int max;
    ITNode *left, *right;
};

// A utility function to create a new Interval Search Tree Node
ITNode *newNode(Interval i) {
    ITNode *temp = new ITNode;
    temp->interval = new Interval(i);
    temp->max = i.high;
    temp->left = temp->right = nullptr;
    return temp;
}

// A utility function to insert a new interval into the Interval Tree
ITNode *insert(ITNode *root, Interval i) {
    if (root == nullptr)
        return newNode(i);

    int low = root->interval->low;

    // If the interval to be inserted has a low value smaller than the current node, go to the left subtree
    if (i.low < low)
        root->left = insert(root->left, i);
    else
        root->right = insert(root->right, i);

    // Update the max value of this node
    if (root->max < i.high)
        root->max = i.high;

    return root;
}

// A utility function to check if two given intervals overlap
bool doOverlap(Interval i1, Interval i2) {
    return (i1.low <= i2.high && i2.low <= i1.high);
}

// Function to search for any interval that overlaps with the given interval
void overlapSearch(ITNode *root, Interval i) {
    if (root == nullptr) return;

    if (doOverlap(*(root->interval), i))
        cout << "[" << root->interval->low << ", " << root->interval->high << "]\n";

    if (root->left != nullptr && root->left->max >= i.low)
        overlapSearch(root->left, i);

    overlapSearch(root->right, i);
}

// Inorder traversal for interval tree
void inorder(ITNode *root) {
    if (root == nullptr) return;
    inorder(root->left);
    cout << "[" << root->interval->low << ", " << root->interval->high << "] max = " << root->max << endl;
    inorder(root->right);
}

int main() {
    vector<Interval> intervals = {{15, 20}, {10, 30}, {17, 19}, {5, 20}, {12, 15}, {30, 40}};
    ITNode *root = nullptr;

    for (const auto &i : intervals)
        root = insert(root, i);

    cout << "Inorder traversal of constructed Interval Tree is:\n";
    inorder(root);

    Interval x = {6, 7};
    cout << "\nSearching for interval overlapping with [" << x.low << ", " << x.high << "]\n";
    overlapSearch(root, x);

    return 0;
}
