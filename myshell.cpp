#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

// Function to split input into tokens
vector<char*> parseInput(string input) {
    vector<char*> args;
    stringstream ss(input);
    string token;

    while (ss >> token) {
        char* arg = new char[token.size() + 1];
        strcpy(arg, token.c_str());
        args.push_back(arg);
    }

    args.push_back(nullptr);  // execvp expects a null-terminated array
    return args;
}

// Function to execute the command
void executeCommand(vector<char*>& args) {
    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        if (execvp(args[0], args.data()) == -1) {
            perror("myshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Fork failed
        perror("myshell");
    } else {
        // Parent process waits
        int status;
        waitpid(pid, &status, 0);
    }
}

// Free allocated memory
void cleanArgs(vector<char*>& args) {
    for (char* arg : args) {
        delete[] arg;
    }
}

int main() {
    string input;
    cout << "Welcome to MyShell (C++)! Type 'exit' to quit." << endl;

    while (true) {
        cout << "myshell > ";
        getline(cin, input);

        if (input == "exit") break;

        vector<char*> args = parseInput(input);
        if (!args.empty() && args[0] != nullptr) {
            executeCommand(args);
        }
        cleanArgs(args);
    }

    return 0;
}
