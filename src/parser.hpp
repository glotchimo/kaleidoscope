#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include <map>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

extern std::map<char, int> binOpPrecedence;

std::unique_ptr<ExprAST> parseExpr();
std::unique_ptr<ExprAST> parseEntryExpr();
std::unique_ptr<ExprAST> parseNumExpr();
std::unique_ptr<ExprAST> parseParenExpr();
std::unique_ptr<ExprAST> parseIdentExpr();

std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrecedence,
                                       std::unique_ptr<ExprAST> lhs);

std::unique_ptr<ProtoAST> parseProto();
std::unique_ptr<ProtoAST> parseExtern();

std::unique_ptr<FuncAST> parseDef();
std::unique_ptr<FuncAST> parseTopLevelExpr();

#endif // KALEIDOSCOPE_PARSER_H
