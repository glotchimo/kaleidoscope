#include "ast.hpp"

std::unique_ptr<expr_ast> error_expr(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return 0;
}

std::unique_ptr<proto_ast> error_proto(const char *Str) {
  error_expr(Str);
  return 0;
}

std::unique_ptr<func_ast> error_func(const char *Str) {
  error_expr(Str);
  return 0;
}
