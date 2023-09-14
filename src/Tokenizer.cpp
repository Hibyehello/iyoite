#include "Tokenizer.h"
#include "Common.h"
#include "ErrorHandler.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Token> Tokenizer::Tokenize(std::string path) {
  std::string buf;
  m_source = ReadFile(path);

  while (peek().has_value()) {
    char c = peek().value();
    switch (c) {}
    if (std::isspace(peek().value())) {
      get_current();
      continue;
    }
    if (std::isalpha(peek().value())) {
      buf.push_back(get_current());
      while (peek().has_value() && std::isalnum(peek().value())) {
        buf.push_back(get_current());
      }
      if (buf == "exit") {
        m_tokens.push_back({.type = TokenType::_exit});
        buf.clear();
      } else if (buf == "int") {
        m_tokens.push_back({.type = TokenType::_int});
        buf.clear();
      } else {
        m_tokens.push_back({.type = TokenType::_ident, .value = buf});
        buf.clear();
      }
      // else {
      //   std::cerr << "Invalid Syntax: received '" << buf << "'" << std::endl;
      //   throw CompilerException("Error");
      // }
    } else if (std::isdigit(peek().value())) {
      buf.push_back(get_current());
      while (peek().has_value() && std::isdigit(peek().value())) {
        buf.push_back(get_current());
      }
      m_tokens.push_back({.type = TokenType::_int_literal, .value = buf});
      buf.clear();
    } else if (peek().value() == '(') {
      get_current();
      m_tokens.push_back({.type = TokenType::_open_paren});
    } else if (peek().value() == ')') {
      get_current();
      m_tokens.push_back({.type = TokenType::_close_paren});
    } else if (peek().value() == ';') {
      get_current();
      m_tokens.push_back({.type = TokenType::_semicolon});
    } else if (peek().value() == '=') {
      get_current();
      m_tokens.push_back({.type = TokenType::_eq});
    } else if (peek().value() == '+') {
      get_current();
      m_tokens.push_back({.type = TokenType::_plus});
    } else if (peek().value() == '-') {
      get_current();
      m_tokens.push_back({.type = TokenType::_dash});
    } else if (peek().value() == '*') {
      get_current();
      m_tokens.push_back({.type = TokenType::_star});
    } else if (peek().value() == '/') {
      get_current();
      m_tokens.push_back({.type = TokenType::_slash});
    } else {
      throw CompilerException("Invalid Syntax: Unknown Error");
    }
  }
  m_current_idx = 0;
  return m_tokens;
}

std::optional<char> Tokenizer::peek(int offset) const {
  if (m_current_idx + offset >= m_source.length())
    return {};

  return m_source.at(m_current_idx + offset);
}

char Tokenizer::get_current() { return m_source.at(m_current_idx++); }

std::string Tokenizer::ReadFile(std::string path) {
  std::stringstream in;
  std::fstream m_file_stream(path, std::ios::in);
  in << m_file_stream.rdbuf();
  m_file_stream.close();

  return in.str();
}