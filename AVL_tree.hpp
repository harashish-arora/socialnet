#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
using namespace std;

// Color codes for output
#define RESET "\033[0m"
#define GREEN "\033[32m"

struct Node {
    int key;
    time_t timestamp;
    string content;
    Node* left;
    Node* right;
    int height;
    Node(int k, time_t t, const string &c) : key(k), timestamp(t), content(c), left(nullptr), right(nullptr), height(0) {}
};

class AVLTree {
private:
    Node* root;
    int nextId;

    int height(Node* N) {
        if (N == nullptr) return -1;
        return (N -> height);
    }

    Node* rightRotate(Node* y) {
        Node* x = y -> left;
        Node* T2 = x -> right;
 
        x -> right = y;
        y -> left = T2;

        y -> height = max(height(y -> left), height(y -> right)) + 1;
        x -> height = max(height(x -> left), height(x -> right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x -> right;
        Node* T2 = y -> left;

        x -> right = T2;
        y -> left = x;

        y -> height = max(height(y -> left), height(y -> right)) + 1;
        x -> height = max(height(x -> left), height(x -> right)) + 1;
        
        return y;
    }

    int getBalance(Node* N) {
        if (N == nullptr) return 0;
        return height(N -> left) - height(N -> right);
    }

    Node* insertHelper(Node* node, int key, time_t timestamp, const string& content) {
        if (node == nullptr)
            return new Node(key, timestamp, content);

        if (key < node->key)
            node->left = insertHelper(node->left, key, timestamp, content);
        else if (key > node->key)
            node->right = insertHelper(node->right, key, timestamp, content);
        else // duplicate key should not happen
            return node;

        node -> height = 1 + max(height(node -> left), height(node -> right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }
    
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* deleteNodeHelper(Node* root, int key) {
        if (root == nullptr) return root;

        if (key < root->key)
            root->left = deleteNodeHelper(root->left, key);
        else if (key > root->key)
            root->right = deleteNodeHelper(root->right, key);
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else *root = *temp;
                delete temp;
            }
            else {
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->timestamp = temp->timestamp;
                root->content = temp->content;
                root->right = deleteNodeHelper(root->right, temp->key);
            }
        }

        if (root == nullptr) return root;

        // Update height
        root -> height = 1 + max(height(root -> left), height(root -> right));

        // Get balance factor
        int balance = getBalance(root);

        // Rebalance
        // Left Left Case
        if (balance > 1 && getBalance(root -> left) >= 0) 
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root -> left) < 0) {
            root -> left = leftRotate(root -> left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root -> right) <= 0) 
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root -> right) > 0) {
            root -> right = rightRotate(root -> right);
            return leftRotate(root);
        }

        return root;
    }

    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node -> left);
            deleteTree(node -> right);
            delete node;
        }
    }

    void printRecent(Node* node, int &count, int N) {
        if (node == nullptr || (N != -1 && count >= N)) return;

        printRecent(node -> right, count, N);

        if (N == -1 || count < N) {
            string timestamp_str = ctime(&node -> timestamp);
            if (!timestamp_str.empty() && timestamp_str.back() == '\n')
                timestamp_str.pop_back();
            cout << GREEN << timestamp_str << ": " << node -> content << RESET << endl;
            count++;
        }

        printRecent(node -> left, count, N);
    }

public:
    AVLTree() : root(nullptr), nextId(0) {}

    ~AVLTree() { deleteTree(root); }

    bool hasPosts() const {
        return root != nullptr; 
    }

    void insert(const string& content) {
        time_t now = time(nullptr);
        int key = nextId++;
        root = insertHelper(root, key, now, content);
    }

    void deleteNode(int key) {
        root = deleteNodeHelper(root, key);
    }

    void outputPosts(int N) {
        int count = 0;
        printRecent(root, count, N);
    }
};

#endif
