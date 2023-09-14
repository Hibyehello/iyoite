#pragma once

#include "Common.h"
#include <variant>

namespace Node {

struct Stmt;
struct Term;
struct Expr;
struct BinExpr;
struct Exec;

namespace Terms {
struct IntLit {
  Token int_literal;
};

struct Ident {
  Token ident;
};

struct Paren {
  Node::Expr *expr;
};

} // namespace Terms

namespace BinaryExpression {
struct Add {
  Expr *lhs;
  Expr *rhs;
};

struct Subtract {
  Expr *lhs;
  Expr *rhs;
};

struct Multiply {
  Expr *lhs;
  Expr *rhs;
};

struct Divide {
  Expr *lhs;
  Expr *rhs;
};
} // namespace BinaryExpression

struct BinExpr {
  std::variant<BinaryExpression::Add *, BinaryExpression::Multiply *,
               BinaryExpression::Subtract *, BinaryExpression::Divide *>
      expr;
};

struct Term {
  std::variant<Terms::IntLit *, Terms::Ident *, Terms::Paren *> term;
};

struct Expr {
  std::variant<Term *, BinExpr *> var;
};

namespace Statement {
struct Exit {
  Expr *expr;
};

struct Int {
  Token ident;
  Expr *expr;
};

} // namespace Statement

struct Stmt {
  std::variant<Statement::Exit *, Statement::Int *> stmt;
};

struct Exec {
  std::vector<Stmt *> stmts;
};

} // namespace Node