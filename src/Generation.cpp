#include "Generation.h"
#include "ErrorHandler.h"
#include "Nodes.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

#ifdef __APPLE__
#define ENTRY "_main"
#define SYS_EXIT 0x2000001
#elif __linux
#define ENTRY "_start"
#define SYS_EXIT 0x3c
#endif

size_t s_stack_size = 0;
std::unordered_map<std::string, Generation::Var> Generation::s_vars = {};

std::string Generation::gen_term(const Node::Term *term) {
  struct TermVisitor {
    std::string operator()(const Node::Terms::IntLit *term) {
      std::stringstream out;
      out << "  mov rax, " << term->int_literal.value.value() << "\n";
      out << push("rax");
      return out.str();
    }
    std::string operator()(const Node::Terms::Ident *term) {
      std::stringstream out;
      if (!s_vars.contains(term->ident.value.value())) {
        std::stringstream err;
        err << "Error: Undeclared Variable: " << term->ident.value.value();
        throw CompilerException(err.str());
      }
      const auto &var = s_vars.at(term->ident.value.value());
      std::stringstream pushreg;
      pushreg << "QWORD [rsp+"
              << (s_stack_size - var.stack_loc) * sizeof(void *) << "]\n";
      out << push(pushreg.str());
      return out.str();
    }
    std::string operator()(const Node::Terms::Paren *term) {
      return gen_expr(term->expr);
    }
  };

  TermVisitor visitor;
  return std::visit(visitor, term->term);
}

std::string Generation::gen_bin_expr(const Node::BinExpr *expr) {
  struct ExprVisitor {
    std::string operator()(const Node::BinaryExpression::Add *term) {
      std::stringstream out;
      out << gen_expr(term->rhs);
      out << gen_expr(term->lhs);
      out << pop("rax");
      out << pop("rbx");
      out << "  add rax, rbx\n";
      out << push("rax");
      return out.str();
    }
    std::string operator()(const Node::BinaryExpression::Subtract *term) {
      std::stringstream out;
      out << gen_expr(term->rhs);
      out << gen_expr(term->lhs);
      out << pop("rax");
      out << pop("rbx");
      out << "  sub rax, rbx\n";
      out << push("rax");
      return out.str();
    }
    std::string operator()(const Node::BinaryExpression::Multiply *term) {
      std::stringstream out;
      out << gen_expr(term->rhs);
      out << gen_expr(term->lhs);
      out << pop("rax");
      out << pop("rbx");
      out << "  mul rbx\n";
      out << push("rax");
      return out.str();
    }
    std::string operator()(const Node::BinaryExpression::Divide *term) {
      std::stringstream out;
      out << gen_expr(term->rhs);
      out << gen_expr(term->lhs);
      out << pop("rax");
      out << pop("rbx");
      out << "  div rbx\n";
      out << push("rax");
      return out.str();
    }
  };

  ExprVisitor visitor;
  return std::visit(visitor, expr->expr);
}

std::string Generation::gen_expr(const Node::Expr *expr) {
  struct ExprVisitor {
    std::string operator()(const Node::Term *term) { return gen_term(term); }
    std::string operator()(const Node::BinExpr *expr) {
      return gen_bin_expr(expr);
    }
  };

  ExprVisitor visitor;
  return std::visit(visitor, expr->var);
}

std::string Generation::gen_stmt(const Node::Stmt *stmt) {
  struct StmtVisitor {
    std::string operator()(const Node::Statement::Exit *stmt) {
      std::stringstream out;
      out << gen_expr(stmt->expr);
      out << "  mov rax, " << SYS_EXIT << "\n";
      out << pop("rdi");
      out << "  syscall\n";

      return out.str();
    }
    std::string operator()(const Node::Statement::Int *stmt) {
      std::stringstream out;
      if (s_vars.contains(stmt->ident.value.value())) {
        std::stringstream err;
        err << "Error: Variable " << stmt->ident.value.value() << " exists!";
        throw CompilerException(err.str());
      }

      out << gen_expr(stmt->expr);
      s_vars.insert({stmt->ident.value.value(), {s_stack_size}});
      return out.str();
    }
  };
  StmtVisitor visitor;
  return std::visit(visitor, stmt->stmt);
}

std::string Generation::gen_exec(Node::Exec root) const {
  std::stringstream out;
  out << "global " << ENTRY << "\n" << ENTRY << ":\n";

  for (const auto &stmt : root.stmts) {
    out << gen_stmt(stmt);
  }

  out << "  mov rax, " << SYS_EXIT << "\n";
  out << "  mov rdi, 0\n";
  out << "  syscall\n";

  return out.str();
}

std::string Generation::push(const std::string &reg) {
  std::stringstream out;
  out << "  push " << reg << "\n";
  s_stack_size++;
  return out.str();
}

std::string Generation::pop(const std::string &reg) {
  std::stringstream out;
  out << "  pop " << reg << "\n";
  s_stack_size--;
  return out.str();
}