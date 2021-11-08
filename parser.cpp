#include "parser.hpp"

static ExprAST *parseExpression() {
    ExprAST *lhs = parseEntry();
    if (!lhs) {
        return 0;
    }
}

static ExprAST *parseNumExpr() {
    ExprAST *result = new NumberExprAST(numVal);
    getNextToken();
    return result;
}

static ExprAST *parseParenExpr() {
    getNextToken();
    ExprAST *expr = parseExpression();

    if (!expr) {
        return 0;
    }

    if (curTok != ')') {
        return Error("expected ')'");
    }

    getNextToken();
    return expr;
}

static ExprAST *parseIdentExpr() {
    std::string identName = identStr;

    getNextToken();
    if (curTok != '(') {
        return new VariableExprAST(identName);
    }

    getNextToken();
    std::vector<ExprAST *> args;
    if (curTok != ')') {
        while (1) {
            ExprAST *arg = parseExpression();
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

static ExprAST *parseEntry() {
    switch (curTok) {
    case tok_ident:
        return parseIdentExpr();
    case tok_num:
        return parseNumExpr();
    case '(':
        return parseParenExpr();
    default:
        return Error("unknown token when expecting an expression");
    }
}