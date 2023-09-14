#pragma once

#include <iostream>
#include <string>
#include <vector>

class ArgParser {
public:
    int parse(int numOfArgs, char** args);
    std::vector<std::string>& get_input_files() { return m_input_files; }
private:
    std::vector<std::string> m_input_files;
};