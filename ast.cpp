#include "ast.hpp"

std::unique_ptr<ExprAST> Error(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return 0;
}

std::unique_ptr<PrototypeAST> ErrorP(const char *Str) {
    Error(Str);
    return 0;
}

std::unique_ptr<FunctionAST> ErrorF(const char *Str) {
    Error(Str);
    return 0;
}