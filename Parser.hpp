#if !defined(KALEIDOSCOPE_PARSER_H)
#define KALEIDOSCOPE_PARSER_H

#include "AST.hpp"
#include "Lexer.hpp"

static ExprAST *ParseExpression();
static ExprAST *ParseNumExpr();
static ExprAST *ParseParenExpr();
static ExprAST *ParseIdentExpr();
static ExprAST *ParsePrimary();

#endif // KALEIDOSCOPE_PARSER_H
