#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

struct Command {
    std::vector<std::vector<std::string>> pipeline; // multiple commands (for pipes)
    std::string inputFile;
    std::string outputFile;
    bool background = false;
};

Command parseCommand(const std::string &input);

#endif