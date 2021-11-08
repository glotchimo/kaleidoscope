#if !defined(KALEIDOSCOPE_PARSER_H)
#define KALEIDOSCOPE_PARSER_H

#include "ast.hpp"
#include "lexer.hpp"

static ExprAST *parseExpression();
static ExprAST *parseNumExpr();
static ExprAST *parseParenExpr();
static ExprAST *parseIdentExpr();
static ExprAST *parseBinOpRHS();
static ExprAST *parseEntry();

#endif // KALEIDOSCOPE_PARSER_H
