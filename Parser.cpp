#include "Parser.hpp"

static ExprAST *ParseNumExpr() {
    ExprAST *Result = new NumberExprAST(NumVal);
    getNextToken();
    return Result;
}

static ExprAST *ParseParenExpr() {
    getNextToken();
    ExprAST *V = ParseExpression();
    if (!V)
        return 0;

    if (CurTok != ')')
        return Error("expected ')'");

    getNextToken();
    return V;
}

static ExprAST *ParseIdentExpr() {
    std::string IdentName = IdentStr;

    getNextToken();
    if (CurTok != '(')
        return new VariableExprAST(IdentName);

    getNextToken();
    std::vector<ExprAST *> Args;
    if (CurTok != ')') {
        while (1) {
            ExprAST *Arg = ParseExpression();
            if (!Arg)
                return 0;
            Args.push_back(Arg);

            if (CurTok == ')')
                break;
            else if (CurTok != ',')
                return Error("Expected ')' or ',' in argument list");

            getNextToken();
        }
    }

    getNextToken();

    return new CallExprAST(IdentName, Args);
}

static ExprAST *ParsePrimary() {
    switch (CurTok) {
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