#include "Parser.h"
#include "Common.h"
#include "ErrorHandler.h"
#include "Nodes.h"

#include <optional>

void printType(TokenType Type) {
  std::string type;
  /*
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
  */
  switch (Type) {
  case TokenType::_exit:
    type = "exit";
    break;
  case TokenType::_int_literal:
    type = "int_literal";
    break;
  case TokenType::_semicolon:
    type = "semicolon";
    break;
  case TokenType::_open_paren:
    type = "open_paren";
    break;
  case TokenType::_close_paren:
    type = "close_paren";
    break;
  case TokenType::_ident:
    type = "ident";
    break;
  case TokenType::_int:
    type = "int";
    break;
  case TokenType::_eq:
    type = "eq";
    break;
  case TokenType::_plus:
    type = "add";
    break;
  case TokenType::_dash:
    type = "subtract";
    break;
  case TokenType::_star:
    type = "multiply";
    break;
  case TokenType::_slash:
    type = "divide";
    break;
  default:
    type = "unknown";
  }

  std::cout << "Token type is " << type << std::endl;
}

std::optional<Node::Term *> Parser::parse_term() {
  if (auto int_lit = try_get_current(TokenType::_int_literal)) {
    Node::Terms::IntLit *int_expr = m_allocator.alloc<Node::Terms::IntLit>();
    int_expr->int_literal = int_lit.value();
    Node::Term *term = m_allocator.alloc<Node::Term>();
    term->term = int_expr;
    return term;

  } else if (auto token_ident = try_get_current(TokenType::_ident)) {
    Node::Terms::Ident *ident = m_allocator.alloc<Node::Terms::Ident>();
    ident->ident = token_ident.value();
    Node::Term *term = m_allocator.alloc<Node::Term>();
    term->term = ident;
    return term;

  } else if (auto token_paren = try_get_current(TokenType::_open_paren)) {
    std::optional<Node::Expr *> expr = parse_expr();
    if (!expr.has_value()) {
      throw CompilerException("Expected Expression");
    }
    try_get_current(TokenType::_close_paren, "Expected ')'");
    Node::Terms::Paren *paren_term = m_allocator.alloc<Node::Terms::Paren>();
    paren_term->expr = expr.value();
    Node::Term *term = m_allocator.alloc<Node::Term>();
    term->term = paren_term;
    return term;
  } else {
    return {};
  }
}

std::optional<Node::Expr *> Parser::parse_expr(int min_prec) {
  std::optional<Node::Term *> lhs_term = parse_term();
  if (!lhs_term.has_value()) {
    return {};
  }

  Node::Expr *lhs_expr = m_allocator.alloc<Node::Expr>();
  lhs_expr->var = lhs_term.value();

  while (true) {
    std::optional<Token> cur_token = peek();
    if (!cur_token.has_value())
      break;
    std::optional<int> prec = bin_prec(cur_token->type);
    if (!prec.has_value() || prec.value() < min_prec)
      break;

    Token op = get_current();
    int next_min_prec = prec.value() + 1;
    std::optional<Node::Expr *> rhs_expr = parse_expr(next_min_prec);
    if (!rhs_expr.has_value()) {
      throw CompilerException("Unable to Parse Expression");
    }

    Node::BinExpr *expr = m_allocator.alloc<Node::BinExpr>();

    Node::Expr *lhs = m_allocator.alloc<Node::Expr>();

    switch (op.type) {
    case TokenType::_plus: {
      Node::BinaryExpression::Add *add_expr =
          m_allocator.alloc<Node::BinaryExpression::Add>();
      lhs->var = lhs_expr->var;
      add_expr->lhs = lhs;
      add_expr->rhs = rhs_expr.value();
      expr->expr = add_expr;
      break;
    }
    case TokenType::_dash: {
      Node::BinaryExpression::Subtract *sub_expr =
          m_allocator.alloc<Node::BinaryExpression::Subtract>();
      lhs->var = lhs_expr->var;
      sub_expr->lhs = lhs;
      sub_expr->rhs = rhs_expr.value();
      expr->expr = sub_expr;
      break;
    }
    case TokenType::_star: {
      Node::BinaryExpression::Multiply *mul_expr =
          m_allocator.alloc<Node::BinaryExpression::Multiply>();
      lhs->var = lhs_expr->var;
      mul_expr->lhs = lhs;
      mul_expr->rhs = rhs_expr.value();
      expr->expr = mul_expr;
      break;
    }
    case TokenType::_slash: {
      Node::BinaryExpression::Divide *div_expr =
          m_allocator.alloc<Node::BinaryExpression::Divide>();
      lhs->var = lhs_expr->var;
      div_expr->lhs = lhs;
      div_expr->rhs = rhs_expr.value();
      expr->expr = div_expr;
      break;
    }
    default:
      break;
    }

    lhs_expr->var = expr;
  }
  return lhs_expr;
}

std::optional<Node::Stmt *> Parser::parse_stmt() {
  if (peek().has_value() && peek().value().type == TokenType::_exit &&
      peek(1).has_value() && peek(1).value().type == TokenType::_open_paren) {
    get_current();
    get_current();
    Node::Statement::Exit *stmt_exit =
        m_allocator.alloc<Node::Statement::Exit>();
    if (auto expr = parse_expr()) {

      stmt_exit->expr = expr.value();
    } else {
      throw CompilerException("Invalid Expression");
    }
    try_get_current(TokenType::_close_paren, "Expected ')'");
    try_get_current(TokenType::_semicolon, "Expected ';'");
    Node::Stmt *stmt = m_allocator.alloc<Node::Stmt>();
    stmt->stmt = stmt_exit;
    return stmt;
  } else if (peek().has_value() && peek().value().type == TokenType::_int &&
             peek(1).has_value() && peek(1).value().type == TokenType::_ident &&
             peek(2).has_value() && peek(2).value().type == TokenType::_eq) {
    get_current();
    Node::Statement::Int *stmt_int = m_allocator.alloc<Node::Statement::Int>();
    stmt_int->ident = get_current();
    get_current();
    if (auto expr = parse_expr()) {
      stmt_int->expr = expr.value();
    } else {
      throw CompilerException("Invalid Expression");
    }
    try_get_current(TokenType::_semicolon, "Expected ';'");
    Node::Stmt *stmt = m_allocator.alloc<Node::Stmt>();
    stmt->stmt = stmt_int;
    return stmt;
  } else {
    throw CompilerException("Invalid Syntax");
  }
};

Node::Exec Parser::parse_exec(std::vector<Token> tokens) {
  m_tokens = tokens;
  Node::Exec exec;
  while (peek().has_value()) {
    if (auto stmt = parse_stmt()) {
      exec.stmts.push_back(stmt.value());
    } else {
      throw CompilerException("Invalid Statment");
    }
  }

  return exec;
}

std::optional<Token> Parser::peek(int offset) const {
  if (m_current_idx + offset >= m_tokens.size())
    return {};

  return m_tokens.at(m_current_idx + offset);
}

Token Parser::get_current() {
#ifdef IYO_DEBUG
  printType(m_tokens.at(m_current_idx).type);
#endif
  return m_tokens.at(m_current_idx++);
}