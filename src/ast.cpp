#include "ast.hpp"

std::unique_ptr<expr_ast> error_expr(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  return 0;
}

std::unique_ptr<proto_ast> error_proto(const char *str) {
  error_expr(str);
  return 0;
}

std::unique_ptr<func_ast> error_func(const char *str) {
  error_expr(str);
  return 0;
}
