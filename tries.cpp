#include <iostream>
#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // Inserts a word into the trie
    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    // Searches for a word in the trie
    bool search(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return node->isEndOfWord;
    }

    // Checks if any word in the trie starts with the given prefix
    bool startsWith(const std::string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return true;
    }

    // Destructor to deallocate the memory used by the trie
    ~Trie() {
        deleteTrie(root);
    }

private:
    // Helper function to recursively delete all nodes
    void deleteTrie(TrieNode* node) {
        for (auto& pair : node->children) {
            deleteTrie(pair.second);
        }
        delete node;
    }
};

int main() {
    Trie trie;
    trie.insert("apple");
    trie.insert("app");
    std::cout << std::boolalpha;
    std::cout << "Search 'apple': " << trie.search("apple") << std::endl;  // true
    std::cout << "Search 'app': " << trie.search("app") << std::endl;      // true
    std::cout << "Search 'appl': " << trie.search("appl") << std::endl;    // false
    std::cout << "Starts with 'app': " << trie.startsWith("app") << std::endl;  // true
    std::cout << "Starts with 'ap': " << trie.startsWith("ap") << std::endl;    //
    return 0;
}
