#pragma once

#include "filesystem"
#include <exception>
#include <source_location>
#include <sstream>
#include <string>

class CompilerException : public std::exception {
public:
  CompilerException(
      const std::string &err_msg,
      const std::source_location loc = std::source_location::current())
      : m_message(err_msg), filename(loc.file_name()), line(loc.line()) {}

  const char *what() {
    std::stringstream out;
    out << "Compilation Error: " << m_message << "\nThrown from "
        << std::filesystem::path(filename).filename() << ":" << line;
    m_message = out.str();
    return m_message.c_str();
  }

private:
  std::string m_message;
  std::string filename;
  int line;
};