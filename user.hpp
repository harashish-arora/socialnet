#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <unordered_set>
#include "AVL_tree.hpp"

using namespace std;

class User {
public:
    string username;
    AVLTree posts;
    unordered_set<string> friends;
    
    User() : username(""), posts(), friends() {}

    User(const string &uname) {
        username = toLower(uname);
    }
    
    string toLower(const string &s) {
        string res = s;
        for (char &c : res)
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        return res;
    }
};


#endif
