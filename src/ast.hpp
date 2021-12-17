#if !defined(KALEIDOSCOPE_AST_H)
#define KALEIDOSCOPE_AST_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <vector>

extern std::unique_ptr<llvm::LLVMContext> context;
extern std::unique_ptr<llvm::IRBuilder<>> builder;
extern std::unique_ptr<llvm::Module> module;
extern std::map<std::string, llvm::Value *> namedValues;

class ExprAST {
public:
  virtual ~ExprAST() {}
  virtual llvm::Value *codegen() = 0;
};

class NumberExprAST : public ExprAST {
  double value;

public:
  NumberExprAST(double val) : value(val) {}
  llvm::Value *codegen() override;
};

class VariableExprAST : public ExprAST {
  std::string name;

public:
  VariableExprAST(const std::string &name) : name(name) {}
  llvm::Value *codegen() override;
};

class BinaryExprAST : public ExprAST {
  char op;
  std::unique_ptr<ExprAST> lhs, rhs;

public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs,
                std::unique_ptr<ExprAST> rhs)
      : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
  llvm::Value *codegen() override;
};

class CallExprAST : public ExprAST {
  std::string callee;
  std::vector<std::unique_ptr<ExprAST>> args;

public:
  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<ExprAST>> args)
      : callee(callee), args(std::move(args)) {}
  llvm::Value *codegen() override;
};

class ProtoAST {
  std::string name;
  std::vector<std::string> args;

public:
  ProtoAST(const std::string &name, std::vector<std::string> args)
      : name(name), args(std::move(args)) {}
  const std::string &getName() const { return name; }
  llvm::Function *codegen();
};

class FuncAST {
  std::unique_ptr<ProtoAST> proto;
  std::unique_ptr<ExprAST> body;

public:
  FuncAST(std::unique_ptr<ProtoAST> proto, std::unique_ptr<ExprAST> body)
      : proto(std::move(proto)), body(std::move(body)) {}
  llvm::Function *codegen();
};

std::unique_ptr<ExprAST> logErr(const char *Str);
std::unique_ptr<ProtoAST> logProtoErr(const char *Str);
std::unique_ptr<FuncAST> logFuncErr(const char *Str);
llvm::Value *logValErr(const char *str);

#endif // KALEIDOSCOPE_AST_H
