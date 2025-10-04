#ifndef JOBS_H
#define JOBS_H
#include <vector>
#include <sys/types.h>
#include <string>

struct Job {
    pid_t pid;
    std::string command;
    bool running;
};

extern std::vector<Job> jobList;

void addJob(pid_t pid, const std::string &cmd);
void listJobs();

#endif