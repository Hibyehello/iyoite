#pragma once

#include "ArenaAlloc.h"
#include "Common.h"
#include "ErrorHandler.h"
#include "Nodes.h"

#include <optional>
#include <vector>

class Parser {
public:
  Parser() : m_allocator(1024 * 1024 * 4), m_current_idx(0) {}

  Node::Exec parse_exec(std::vector<Token> tokens);
  std::optional<Node::Term *> parse_term();
  std::optional<Node::Expr *> parse_expr(int min_prec = 0);
  std::optional<Node::Stmt *> parse_stmt();

private:
  std::vector<Token> m_tokens;
  size_t m_current_idx;

  std::optional<Token> peek(int offset = 0) const;
  Token get_current();
  inline Token try_get_current(const TokenType &type, std::string err_msg) {
    if (peek().has_value() && peek().value().type == type) {
      return get_current();
    } else {
      throw CompilerException(err_msg);
    }
  }

  inline std::optional<Token> try_get_current(const TokenType &type) {
    if (peek().has_value() && peek().value().type == type) {
      return get_current();
    } else {
      return {};
    }
  }
  ArenaAllocator m_allocator;
};