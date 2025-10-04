#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include "parser.h"
#include "executor.h"
#include "jobs.h"

int main() {
    std::string line;
    std::vector<std::string> history;

    // Ignore Ctrl+C and Ctrl+Z in shell main process
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    while (true) {
        std::cout << "myshell> ";
        if (!std::getline(std::cin, line)) break;

        if (line.empty()) continue;

        history.push_back(line);

        // built-ins: exit, cd, pwd, history, jobs
        if (line == "exit") {
            std::cout << "Exiting shell..." << std::endl;
            break;
        }
        if (line.rfind("cd ", 0) == 0 || line == "cd") {
            std::string path = (line == "cd") ? std::string(getenv("HOME")) : line.substr(3);
            if (chdir(path.c_str()) != 0) perror("cd failed");
            continue;
        }
        if (line == "pwd") {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) std::cout << cwd << std::endl;
            else perror("pwd failed");
            continue;
        }
        if (line == "history") {
            for (size_t i = 0; i < history.size(); ++i)
                std::cout << i+1 << "  " << history[i] << std::endl;
            continue;
        }
        if (line == "jobs") {
            listJobs();
            continue;
        }

        Command cmd = parseCommand(line);
        executeCommand(cmd, line);
    }

    return 0;
}