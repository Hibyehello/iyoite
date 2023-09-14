#include "ArgParser.h"
#include "ErrorHandler.h"

#include <filesystem>

int ArgParser::parse(int argc, char **argv) {
  std::vector<std::string> args(argv + 1, argv + argc);

  for (const auto &arg : args) {
    if (!std::filesystem::exists(arg)) {
      throw CompilerException(arg + ": no such file!");
    }
    m_input_files.push_back(arg);
  }

  return 0;
}