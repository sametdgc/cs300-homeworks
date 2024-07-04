#pragma once

template <class Key, class Value>
class AVLSearchTree {
private:
    struct Node {
        Key key;
        Value value;
        int height;
        Node* left;
        Node* right;
    };

    Node* root;

    // Private methods for tree operations
    int getHeight(Node* node);
    int getBalance(Node* node);
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* balance(Node* node);
    Node* insert(Node* node, const Key& key, const Value& value);
    Node* minValueNode(Node* node);
    Node* deleteNode(Node* root, const Key& key);
    void destroyTree(Node* node);
    Value search(Node* node, const Key& key);
    int countUniqueWords(Node* node); // New private method for counting unique words


public:
    AVLSearchTree();
    ~AVLSearchTree();
    void insert(const Key& key, const Value& value);
    void remove(const Key& key);
    Value search(const Key& key);
    int uniqueWordCount(); // New public member function to get the unique word count

};
