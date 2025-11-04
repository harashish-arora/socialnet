#ifndef SOCIAL_NET_HPP
#define SOCIAL_NET_HPP

#include "user.hpp"
#include "AVL_tree.hpp"
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>

using namespace std;

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

class SocialNet {
private:
    unordered_map<string, User> users;

    string toLower(const string &s) {
        string res = s;
        for (char &c : res)
            if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        return res;
    }

    static bool compareCandidates(const pair<string, int> &a, const pair<string, int> &b) {
        if (a.second != b.second)
            return a.second > b.second;
        return a.first < b.first;
    }


public:
    void addUser(const string &uname) {
        string name = toLower(uname);
        if (users.count(name)) {
            cerr << YELLOW << "Username " << toLower(uname) << " already exists. Try a different username.\n" << RESET;
            return;
        }
        User u(name);
        users[name] = u;
        cout << GREEN << "User " << toLower(uname) << " added successfully.\n" << RESET;
    }

    void addFriend(const string &u1, const string &u2) {
        string a = toLower(u1);
        string b = toLower(u2);
        if (a == b) {
            cerr << YELLOW << "A user cannot be friends with themselves.\n" << RESET;
            return;
        }
        if (!users.count(a)) {
            cerr << YELLOW << "User " << toLower(u1) << " does not exist.\n" << RESET;
            return;
        }
        if (!users.count(b)) {
            cerr << YELLOW << "User " << toLower(u2) << " does not exist.\n" << RESET;
            return;
        }
        if (users[a].friends.count(b) || users[b].friends.count(a)) {
            cerr << YELLOW << "Error: '" << toLower(u1) << "' and '" << toLower(u2) << "' are already friends.\n" << RESET;
            return;
        }
        
        users[a].friends.insert(b);
        users[b].friends.insert(a);

        cout << GREEN << "Users " << toLower(u1) << " and " << toLower(u2) << " are now friends.\n" << RESET;
    }

    void listFriends(const string &uname) {
        string name = toLower(uname);
        if (!users.count(name)) {
            cerr << YELLOW << "User not found.\n" << RESET;
            return;
        }
        vector<string> friendList(users[name].friends.begin(), users[name].friends.end());
        sort(friendList.begin(), friendList.end());

        cout << GREEN << "The friends of " << toLower(uname) << " are the following:\n" << RESET;
        for (string &f : friendList)
            cout << GREEN << f << " ";
        cout << GREEN << "\n" << RESET;
    }

    void addPost(const string &uname, const string &content) {
        string name = toLower(uname);
        if (!users.count(name)) {
            cerr << YELLOW << "User " << toLower(uname) << " does not exist.\n" << RESET;
            return;
        }
        users[name].posts.insert(content);
        cout << GREEN << "Post added for user " << toLower(uname) << ".\n" << RESET;
    }

    void outputPosts(const string &uname, int N) {
        string name = toLower(uname);
        if (!users.count(name)) {
            cerr << YELLOW << "User " << toLower(uname) << " does not exist.\n" << RESET;
            return;
        }
            if (N < -1) {
                cerr << YELLOW << "Invalid N: " << N << ". N must be -1 or non-negative.\n" << RESET;
                return;
            }
            if (!users[name].posts.hasPosts()) {
                cerr << YELLOW << "User " << toLower(uname) << " has no posts.\n" << RESET;
                return;
            }
            users[name].posts.outputPosts(N);
    }

    int degreesOfSeparation(const string &u1, const string &u2) {
        string a = toLower(u1);
        string b = toLower(u2);

        if (!users.count(a) || !users.count(b)) {
            cerr << YELLOW << "Error: One or both users do not exist.\n" << RESET;
            return -2;
        }

        queue<pair<string, int>> q;
        unordered_set<string> visited;

        q.push({a, 0});
        visited.insert(a);

        while(!q.empty()) {
            pair<string, int> curr = q.front();
            q.pop();
            string vertex = curr.first;
            int dist = curr.second;

            if (vertex == b) {
                return dist;
            }

            for (string f : users[vertex].friends) {
                if (!visited.count(f)) {
                    visited.insert(f);
                    q.push({f, dist + 1});
                }
            }
        }
        return -1;
    }

    void suggestFriends(const string &uname, int N) {
        string name = toLower(uname);
        if (!users.count(name)) {
            cerr << YELLOW << "User " << toLower(uname) << " does not exist.\n" << RESET;
            return;
        }

        auto &fset = users[name].friends;
        unordered_map<string, int> mutuals;

        for (const string &f : fset) {
            for (const string &fof : users[f].friends) {
                if (fof == name || fset.count(fof)) continue;
                mutuals[fof]++;
            }
        }

        // Convert to sortable vector
        vector<pair<string, int>> candidates(mutuals.begin(), mutuals.end());

        // Sort by number of mutual friends (descending), then alphabetically
        sort(candidates.begin(), candidates.end(), compareCandidates);

        // Output
        if (candidates.empty()) {
            cerr << YELLOW << "No friend suggestions available for " << toLower(uname) << ".\n" << RESET;
            return;
        }
        if (N != 0) {
            cout << GREEN << "Friend suggestions for " << toLower(uname) << ":\n";
            int count = 0;
            for (auto &p : candidates) {
                if (count == N) break;
                cout << GREEN << p.first << " (" << p.second << " mutuals)\n" << RESET;
                count++;
            }
        }   
    }
};

#endif
