#include <iostream>
#include <sstream>
#include <string>
#include "social_net.hpp"

using namespace std;

// Color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"
#define BOLD "\033[1m"

void printBanner() {
    cout << CYAN << BOLD;
    cout << R"(
╔═══════════════════════════════════════════════════════════════╗
║                                                               ║
║   ███████╗ ██████╗  ██████╗██╗ █████╗ ██╗     ███╗   ██╗      ║
║   ██╔════╝██╔═══██╗██╔════╝██║██╔══██╗██║     ████╗  ██║      ║
║   ███████╗██║   ██║██║     ██║███████║██║     ██╔██╗ ██║      ║
║   ╚════██║██║   ██║██║     ██║██╔══██║██║     ██║╚██╗██║      ║
║   ███████║╚██████╔╝╚██████╗██║██║  ██║███████╗██║ ╚████║      ║
║   ╚══════╝ ╚═════╝  ╚═════╝╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝      ║
║                                                               ║
║                 Network Simulator v1.0                        ║
║                                                               ║
╚═══════════════════════════════════════════════════════════════╝
)" << RESET << endl;
    cout << GREEN << "Welcome to SocialNet Simulator!" << RESET << endl;
    cout << YELLOW << "Type your commands below (or 'EXIT' to quit)" << RESET << endl;
    cout << string(63, '=') << endl << endl;
}

int main() {
    // ios::sync_with_stdio(false);
    // cin.tie(nullptr);

    SocialNet sn;
    string line;

    printBanner();

    while (getline(cin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "EXIT" || command == "QUIT") {
            cout << endl << CYAN << BOLD << string(63, '=') << RESET << endl;
            cout << GREEN << "Exiting SocialNet Simulator. Goodbye!" << RESET << endl;
            cout << CYAN << BOLD << string(63, '=') << RESET << endl;
            break;
        }

        if (command == "ADD_USER") {
            string uname;
            ss >> uname;
            if (uname.empty()) {
                cerr << RED << "Error: Username not provided.\n" << RESET;
                continue;
            }
            sn.addUser(uname);
        }

        else if (command == "ADD_FRIEND") {
            string u1, u2;
            ss >> u1 >> u2;
            if (u1.empty() || u2.empty()) {
                cerr << RED << "Error: Two usernames required.\n" << RESET;
                continue;
            }
            sn.addFriend(u1, u2);
        }

        else if (command == "LIST_FRIENDS") {
            string uname;
            ss >> uname;
            if (uname.empty()) {
                cerr << RED << "Error: Username not provided.\n" << RESET;
                continue;
            }
            sn.listFriends(uname);
        }

        else if (command == "SUGGEST_FRIENDS") {
            string uname;
            int N;
            ss >> uname >> N;
            if (uname.empty()) {
                cerr << RED << "Error: Username not provided.\n" << RESET;
                continue;
            }
            sn.suggestFriends(uname, N);
        }

        else if (command == "DEGREES_OF_SEPARATION") {
            string u1, u2;
            ss >> u1 >> u2;
            if (u1.empty() || u2.empty()) {
                cerr << RED << "Error: Two usernames required.\n" << RESET;
                continue;
            }
            int deg = sn.degreesOfSeparation(u1, u2);
            if (deg != -1) cout << GREEN << deg << "\n" << RESET;
        }

        else if (command == "ADD_POST") {
            string uname;
            ss >> uname;
            string content;
            getline(ss, content);
            if (!content.empty() && content[0] == ' ') content.erase(0, 1);

            // Remove quotes around post content (defensive - input may or may not have them)
            if (!content.empty() && content.front() == '"') content.erase(0, 1);
            if (!content.empty() && content.back() == '"') content.pop_back();

            if (uname.empty() || content.empty()) {
                cerr << RED << "Error: Invalid ADD_POST syntax.\n" << RESET;
                continue;
            }
            sn.addPost(uname, content);
        }

        else if (command == "OUTPUT_POSTS") {
            string uname, Nstr;
            ss >> uname >> Nstr;
            if (uname.empty() || Nstr.empty()) {
                cerr << RED << "Error: OUTPUT_POSTS requires a username and N (use -1 for all).\n" << RESET;
                continue;
            }
            // Validate N is an integer
            try {
                size_t idx = 0;
                int N = stoi(Nstr, &idx);
                if (idx != Nstr.size()) throw invalid_argument("trailing");
                sn.outputPosts(uname, N);
            } catch (exception &e) {
                cerr << RED << "Error: N must be an integer (use -1 for all).\n" << RESET;
                continue;
            }
        }

        else {
            cerr << RED << "Error: Unknown command '" << command << "'.\n" << RESET;
        }
    }

    return 0;
}
