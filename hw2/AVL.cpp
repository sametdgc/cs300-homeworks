#include <algorithm>
#include "AVL.h"

// Constructor
template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree() : root(nullptr) {}

// Destructor
template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree() {
    destroyTree(root);
}

// Helper method to get height of a node
template <class Key, class Value>
int AVLSearchTree<Key, Value>::getHeight(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Helper method to get balance factor of a node
template <class Key, class Value>
int AVLSearchTree<Key, Value>::getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Helper method to perform right rotation
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::rotateRight(Node* node) {
    Node* leftChild = node->left;
    Node* rightGrandChild = leftChild->right;

    // Perform rotation
    leftChild->right = node;
    node->left = rightGrandChild;

    // Update heights
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    leftChild->height = std::max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;

    // Return new root
    return leftChild;
}

// Helper method to perform left rotation
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::rotateLeft(Node* node) {
    Node* rightChild = node->right;
    Node* leftGrandChild = rightChild->left;

    // Perform rotation
    rightChild->left = node;
    node->right = leftGrandChild;

    // Update heights
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    rightChild->height = std::max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;

    // Return new root
    return rightChild;
}

// Helper method to balance a node
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::balance(Node* node) {
    // Update height of node
    node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

    // Get balance factor of node
    int balanceFactor = getBalance(node);

    // Left heavy
    if (balanceFactor > 1) {
        if (getBalance(node->left) < 0) {
            // Left-right case
            node->left = rotateLeft(node->left);
        }
        // Left-left case
        return rotateRight(node);
    }
    // Right heavy
    else if (balanceFactor < -1) {
        if (getBalance(node->right) > 0) {
            // Right-left case
            node->right = rotateRight(node->right);
        }
        // Right-right case
        return rotateLeft(node);
    }

    // Node is balanced
    return node;
}

// Helper method to insert a node into the tree
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::insert(Node* node, const Key& key, const Value& value) {
    // Perform standard BST insertion
    if (node == nullptr) {
        Node* newNode = new Node{ key, value, 1, nullptr, nullptr };
        return newNode;
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    }
    else if (key > node->key) {
        node->right = insert(node->right, key, value);
    }
    else {
        // Key already exists, update value
        node->value = value;
        return node;
    }

    // Update height of current node
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // Rebalance the node
    return balance(node);
}

// Public method to insert a key-value pair into the tree
template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key& key, const Value& value) {
    root = insert(root, key, value);
}

// Helper method to find the node with the minimum value in a subtree
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Helper method to delete a node from the tree
template <class Key, class Value>
typename AVLSearchTree<Key, Value>::Node* AVLSearchTree<Key, Value>::deleteNode(Node* root, const Key& key) {
    // Perform standard BST delete
    if (root == nullptr)
        return root;

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // Node with only one child or no child
        if (root->left == nullptr || root->right == nullptr) {
            Node* temp = root->left ? root->left : root->right;
            // No child case
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else { // One child case
                *root = *temp;
            }
            delete temp;
        }
        else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* temp = minValueNode(root->right);
            // Copy the inorder successor's data to this node
            root->key = temp->key;
            root->value = temp->value;
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node then return
    if (root == nullptr)
        return root;

    // Update height of the current node
    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    // Rebalance the node
    return balance(root);
}

// Public method to remove a key from the tree
template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key& key) {
    root = deleteNode(root, key);
}

// Helper method to search for a key in the tree
template <class Key, class Value>
Value AVLSearchTree<Key, Value>::search(Node* node, const Key& key) {
    if (node == nullptr)
        return nullptr;

    if (key < node->key)
        return search(node->left, key);
    else if (key > node->key)
        return search(node->right, key);
    else
        return node->value;
}

// Public method to search for a key in the tree
template <class Key, class Value>
Value AVLSearchTree<Key, Value>::search(const Key& key) {
    return search(root, key);
}

// Helper method to destroy the tree
template <class Key, class Value>
void AVLSearchTree<Key, Value>::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
