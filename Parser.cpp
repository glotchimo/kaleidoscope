#include "Parser.hpp"

static ExprAST *ParseNumExpr() {
    ExprAST *result = new NumberExprAST(numVal);
    getNextToken();
    return result;
}

static ExprAST *ParseParenExpr() {
    getNextToken();
    ExprAST *expr = ParseExpression();

    if (!expr) {
        return 0;
    }

    if (curTok != ')') {
        return Error("expected ')'");
    }

    getNextToken();
    return expr;
}

static ExprAST *ParseIdentExpr() {
    std::string identName = identStr;

    getNextToken();
    if (curTok != '(') {
        return new VariableExprAST(identName);
    }

    getNextToken();
    std::vector<ExprAST *> args;
    if (curTok != ')') {
        while (1) {
            ExprAST *arg = ParseExpression();
            if (!arg) {
                return 0;
            }
            args.push_back(arg);

            if (curTok == ')') {
                break;
            } else if (curTok != ',') {
                return Error("Expected ')' or ',' in argument list");
            }

            getNextToken();
        }
    }

    getNextToken();

    return new CallExprAST(identName, args);
}

static ExprAST *ParsePrimary() {
    switch (curTok) {
    case tok_ident:
        return ParseIdentExpr();
    case tok_num:
        return ParseNumExpr();
    case '(':
        return ParseParenExpr();
    default:
        return Error("unknown token when expecting an expression");
    }
}