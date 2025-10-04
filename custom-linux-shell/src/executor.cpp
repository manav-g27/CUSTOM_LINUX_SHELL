#include "executor.h"
#include "jobs.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>

void executeCommand(const Command &cmd, const std::string &cmdLine) {
    if (cmd.pipeline.empty()) return;
    int numPipes = static_cast<int>(cmd.pipeline.size()) - 1;
    std::vector<int> pipes(2 * std::max(0, numPipes));

    // Create pipes
    for (int i = 0; i < numPipes; ++i)
        if (pipe(&pipes[2 * i]) == -1) {
            perror("pipe failed");
            return;
        }

    for (int i = 0; i < (int)cmd.pipeline.size(); ++i) {
        pid_t pid = fork();

        if (pid == 0) { // Child
            // restore default signals for child
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            // Input redirection (only on first command)
            if (i == 0 && !cmd.inputFile.empty()) {
                int fd = open(cmd.inputFile.c_str(), O_RDONLY);
                if (fd < 0) perror("open input failed");
                else {
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
            }

            // Output redirection (only on last command)
            if (i == (int)cmd.pipeline.size() - 1 && !cmd.outputFile.empty()) {
                int fd = open(cmd.outputFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) perror("open output failed");
                else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            // Pipe connections
            if (i > 0) {
                dup2(pipes[2 * (i - 1)], STDIN_FILENO);
            }
            if (i < numPipes) {
                dup2(pipes[2 * i + 1], STDOUT_FILENO);
            }

            // Close all pipe fds in child
            for (int fd : pipes) if (fd) close(fd);

            std::vector<char*> args;
            for (auto &a : cmd.pipeline[i]) args.push_back(const_cast<char*>(a.c_str()));
            args.push_back(nullptr);

            execvp(args[0], args.data());
            perror("execvp failed");
            _exit(127);
        } else if (pid < 0) {
            perror("fork failed");
            // cleanup pipes
            for (int fd : pipes) if (fd) close(fd);
            return;
        } else {
            // parent: register background job if requested
            if (cmd.background) addJob(pid, cmdLine);
        }
    }

    // parent closes pipes
    for (int fd : pipes) if (fd) close(fd);

    if (!cmd.background) {
        for (size_t i = 0; i < cmd.pipeline.size(); ++i) {
            int status;
            wait(&status);
        }
    } else {
        std::cout << "[Running in background]" << std::endl;
    }
}