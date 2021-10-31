#include "AST.hpp"

ExprAST *Error(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return 0;
}

PrototypeAST *ErrorP(const char *Str) {
    Error(Str);
    return 0;
}

FunctionAST *ErrorF(const char *Str) {
    Error(Str);
    return 0;
}