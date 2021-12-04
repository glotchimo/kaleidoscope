#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include <memory>
#include <string>
#include <vector>

class expr_ast {
public:
  virtual ~expr_ast() {}
};

class expr_ast_number : public expr_ast {
  double value;

public:
  expr_ast_number(double val) : value(val) {}
};

class expr_ast_variable : public expr_ast {
  std::string name;

public:
  expr_ast_variable(const std::string &name) : name(name) {}
};

class expr_ast_binary : public expr_ast {
  char op;
  std::unique_ptr<expr_ast> lhs, rhs;

public:
  expr_ast_binary(char op, std::unique_ptr<expr_ast> lhs,
                  std::unique_ptr<expr_ast> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

class expr_ast_call : public expr_ast {
  std::string callee;
  std::vector<std::unique_ptr<expr_ast>> args;

public:
  expr_ast_call(const std::string &callee,
                std::vector<std::unique_ptr<expr_ast>> args)
      : callee(callee), args(std::move(args)) {}
};

class proto_ast {
  std::string name;
  std::vector<std::string> args;

public:
  proto_ast(const std::string &name, std::vector<std::string> args)
      : name(name), args(std::move(args)) {}

  const std::string &get_name() const { return name; }
};

class func_ast {
  std::unique_ptr<proto_ast> proto;
  std::unique_ptr<expr_ast> body;

public:
  func_ast(std::unique_ptr<proto_ast> proto, std::unique_ptr<expr_ast> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

std::unique_ptr<expr_ast> error_expr(const char *Str);
std::unique_ptr<proto_ast> error_proto(const char *Str);
std::unique_ptr<func_ast> error_func(const char *Str);

#endif // KALEIDOSCOPE_AST_H
