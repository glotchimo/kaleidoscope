#if !defined(KALEIDOSCOPE_PARSER_H)
#define KALEIDOSCOPE_PARSER_H

#include <map>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

static std::map<char, int> binOpPrecedence;

static std::unique_ptr<ExprAST> parseExpression();
static std::unique_ptr<ExprAST> parseNumExpr();
static std::unique_ptr<ExprAST> parseParenExpr();
static std::unique_ptr<ExprAST> parseIdentExpr();
static std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrecedence, std::unique_ptr<ExprAST> LHS);
static std::unique_ptr<ExprAST> parseEntry();

#endif  // KALEIDOSCOPE_PARSER_H
