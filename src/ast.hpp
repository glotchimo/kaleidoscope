#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

#include <memory>
#include <string>
#include <vector>

class ExprAST {
public:
  virtual ~ExprAST() {}
};

class ExprASTNumber : public ExprAST {
  double value;

public:
  ExprASTNumber(double val) : value(val) {}
};

class ExprASTVariable : public ExprAST {
  std::string name;

public:
  ExprASTVariable(const std::string &name) : name(name) {}
};

class ExprASTBinary : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> lhs, rhs;

public:
  ExprASTBinary(char op, std::unique_ptr<ExprAST> lhs,
                std::unique_ptr<ExprAST> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
};

class ExprASTCall : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  ExprASTCall(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)) {}
};

class ProtoAST {
  std::string name;
  std::vector<std::string> args;

public:
  ProtoAST(const std::string &name, std::vector<std::string> args)
      : name(name), args(std::move(args)) {}

  const std::string &getName() const { return name; }
};

class FuncAST {
  std::unique_ptr<ProtoAST> proto;
  std::unique_ptr<ExprAST> body;

public:
  FuncAST(std::unique_ptr<ProtoAST> proto, std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
};

std::unique_ptr<ExprAST> logExprErr(const char *Str);
std::unique_ptr<ProtoAST> logProtoErr(const char *Str);
std::unique_ptr<FuncAST> logFuncErr(const char *Str);

#endif // KALEIDOSCOPE_AST_H
