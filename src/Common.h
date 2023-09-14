#pragma once

#include <iostream>
#include <optional>
#include <string>

enum class TokenType {
  _exit,
  _int_literal,
  _semicolon,
  _open_paren,
  _close_paren,
  _ident,
  _int,
  _eq,
  _plus,
  _dash,
  _star,
  _slash,
};

inline std::optional<int> bin_prec(TokenType type) {
  switch (type) {
  case TokenType::_plus:
  case TokenType::_dash:
    return 0;
  case TokenType::_star:
  case TokenType::_slash:
    return 1;
  default:
    return {};
  }
}

struct Token {
  TokenType type;
  std::optional<std::string> value;
};
