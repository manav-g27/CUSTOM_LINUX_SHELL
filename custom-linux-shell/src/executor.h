#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"
#include <string>

void executeCommand(const Command &cmd, const std::string &cmdLine);

#endif