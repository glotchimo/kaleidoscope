#include "ast.hpp"

std::unique_ptr<ExprAST> logExprErr(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  return 0;
}

std::unique_ptr<ProtoAST> logProtoErr(const char *str) {
  logExprErr(str);
  return 0;
}

std::unique_ptr<FuncAST> logFuncErr(const char *str) {
  logExprErr(str);
  return 0;
}
