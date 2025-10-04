#include "jobs.h"
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

std::vector<Job> jobList;

void addJob(pid_t pid, const std::string &cmd) {
    jobList.push_back({pid, cmd, true});
}

void listJobs() {
    for (size_t i = 0; i < jobList.size(); ++i) {
        int status;
        pid_t result = waitpid(jobList[i].pid, &status, WNOHANG);
        if (result == 0) {
            jobList[i].running = true;
        } else {
            jobList[i].running = false;
        }

        std::cout << "[" << i + 1 << "] PID: " << jobList[i].pid
                  << " - " << (jobList[i].running ? "Running" : "Done")
                  << " - " << jobList[i].command << "\n";
    }
}