#include "parser.h"
#include <sstream>

// Very simple whitespace tokenizer that recognizes |, <, >, &
Command parseCommand(const std::string &input) {
    Command cmd;
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> currentCmd;

    while (ss >> token) {
        if (token == "|") {
            if (!currentCmd.empty()) cmd.pipeline.push_back(currentCmd);
            currentCmd.clear();
        } else if (token == "<") {
            ss >> cmd.inputFile;
        } else if (token == ">") {
            ss >> cmd.outputFile;
        } else if (token == "&") {
            cmd.background = true;
        } else {
            currentCmd.push_back(token);
        }
    }

    if (!currentCmd.empty()) cmd.pipeline.push_back(currentCmd);
    return cmd;
}