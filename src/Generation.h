#pragma once

#include "Nodes.h"
#include <string>
#include <unordered_map>

class Generation {
public:
  Generation() = default;

  [[nodiscard]] std::string gen_exec(Node::Exec root) const;
  [[nodiscard]] static std::string gen_stmt(const Node::Stmt *stmt);
  [[nodiscard]] static std::string gen_expr(const Node::Expr *expr);
  [[nodiscard]] static std::string gen_term(const Node::Term *term);
  [[nodiscard]] static std::string gen_bin_expr(const Node::BinExpr *expr);

private:
  struct Var {
    size_t stack_loc;
  };
  static std::string push(const std::string &reg);
  static std::string pop(const std::string &reg);
  static std::unordered_map<std::string, Var> s_vars;
};