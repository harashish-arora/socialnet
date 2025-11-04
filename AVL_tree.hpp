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
    time_t timestamp;
    string content;
    Node* left;
    Node* right;
    int height;
    Node(time_t t, const string &c) : timestamp(t), content(c), left(nullptr), right(nullptr), height(0) {}
};

class AVLTree {
private:
    Node* root;

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

    Node* insertHelper(Node* node, time_t timestamp, const string& content) {
        if (node == nullptr)
            return new Node(timestamp, content);

        if (timestamp < node -> timestamp)
            node -> left = insertHelper(node -> left, timestamp, content);
        else if (timestamp > node->timestamp)
            node -> right = insertHelper(node -> right, timestamp, content);
        else // duplicate timestamp - ignore (same second collision)
            return node;

        node -> height = 1 + max(height(node -> left), height(node -> right));

        int balance = getBalance(node);

        // Left Left Case
        if (balance > 1 && timestamp < node -> left -> timestamp) 
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && timestamp > node -> right -> timestamp) 
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && timestamp > node -> left -> timestamp) {
            node -> left = leftRotate(node -> left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && timestamp < node -> right -> timestamp) {
            node -> right = rightRotate(node -> right);
            return leftRotate(node);
        }

        return node;
    }
    
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current -> left != nullptr)
            current = current -> left;
        return current;
    }

    Node* deleteNodeHelper(Node* root, time_t key) {
        if (root == nullptr) return root;

        if (key < root -> timestamp)
            root -> left = deleteNodeHelper(root -> left, key);
        else if (key > root -> timestamp)
            root -> right = deleteNodeHelper(root -> right, key);
        else {
            if ((root -> left == nullptr) || (root -> right == nullptr)) {
                Node* temp = root -> left ? root -> left : root -> right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } 
                else *root = *temp;
                delete temp;
            }
            else {
                Node* temp = minValueNode(root -> right);
                root -> timestamp = temp -> timestamp;
                root -> content = temp -> content;
                root -> right = deleteNodeHelper(root -> right, temp -> timestamp);
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

        // Reverse in-order: right -> root -> left (most recent first)
        printRecent(node -> right, count, N);

        if (N == -1 || count < N) {
            string timestamp_str = ctime(&node -> timestamp);
            // Remove trailing newline from ctime
            if (!timestamp_str.empty() && timestamp_str.back() == '\n')
                timestamp_str.pop_back();
            cout << GREEN << timestamp_str << ": " << node -> content << RESET << endl;
            count++;
        }

        printRecent(node -> left, count, N);
    }

public:
    AVLTree() : root(nullptr) {}

    ~AVLTree() { deleteTree(root); }

    void insert(const string& content) {
        time_t now = time(nullptr);
        root = insertHelper(root, now, content);
    }

    void deleteNode(time_t timestamp) {
        root = deleteNodeHelper(root, timestamp);
    }

    void outputPosts(int N) {
        int count = 0;
        printRecent(root, count, N);
    }
};

#endif
