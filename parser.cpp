#include "parser.hpp"

static int getTokenPrecedence() {
    if (!isascii(curTok)) {
        return -1;
    }

    int tokenPrecedence = binOpPrecedence[curTok];
    if (tokenPrecedence <= 0) {
        return -1;
    }

    return tokenPrecedence;
}

static std::unique_ptr<ExprAST> parseExpression() {
    auto lhs = parseEntry();
    if (!lhs) {
        return 0;
    }
}

static std::unique_ptr<ExprAST> parseNumExpr() {
    std::unique_ptr<ExprAST> result(new NumberExprAST(numVal));
    getNextToken();
    return result;
}

static std::unique_ptr<ExprAST> parseParenExpr() {
    getNextToken();
    std::unique_ptr<ExprAST> expr = parseExpression();

    if (!expr) {
        return 0;
    }

    if (curTok != ')') {
        return Error("expected ')'");
    }

    getNextToken();
    return expr;
}

static std::unique_ptr<ExprAST> parseIdentExpr() {
    std::string identName = identStr;

    getNextToken();
    if (curTok != '(') {
        std::unique_ptr<ExprAST> expr(new VariableExprAST(identName));
        return expr;
    }

    getNextToken();
    std::vector<std::unique_ptr<ExprAST> > args;
    if (curTok != ')') {
        while (1) {
            std::unique_ptr<ExprAST> arg = parseExpression();
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

    std::unique_ptr<ExprAST> expr(new CallExprAST(identName, args));
    return expr;
}

static std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrecedence, std::unique_ptr<ExprAST> LHS) {
    while (1) {
        int tokPrecedence = getTokenPrecedence();
        if (tokPrecedence > exprPrecedence) {
            return LHS;
        }

        int binOp = curTok;
        getNextToken();

        auto RHS = parseEntry();
        if (!RHS) {
            return nullptr;
        }
    }
}

static std::unique_ptr<ExprAST> parseEntry() {
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