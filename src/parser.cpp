#include "parser.hpp"

std::map<char, int> binOpPrecedence;

int getTokenPrecedence() {
  if (!isascii(curToken)) {
    return -1;
  }

  int tokenPrecedence = binOpPrecedence[curToken];
  if (tokenPrecedence <= 0) {
    return -1;
  }

  return tokenPrecedence;
}

std::unique_ptr<ExprAST> parseExpr() {
  auto lhs = parseEntryExpr();
  if (!lhs) {
    return nullptr;
  }

  return parseBinOpRHS(0, std::move(lhs));
}

std::unique_ptr<ExprAST> parseEntryExpr() {
  switch (curToken) {
  case tokIdent:
    return parseIdentExpr();
  case tokNum:
    return parseNumExpr();
  case '(':
    return parseParenExpr();
  default:
    return logExprErr("unknown Token when expecting an expression");
  }
}

std::unique_ptr<ExprAST> parseNumExpr() {
  auto result = std::make_unique<ExprASTNumber>(numVal);
  getNextToken();
  return std::move(result);
}

std::unique_ptr<ExprAST> parseParenExpr() {
  getNextToken();
  auto expr = parseExpr();
  if (!expr) {
    return nullptr;
  }

  if (curToken != ')') {
    return logExprErr("expected ')'");
  }

  getNextToken();
  return expr;
}

std::unique_ptr<ExprAST> parseIdentExpr() {
  std::string identName = identStr;

  getNextToken();
  if (curToken != '(') {
    return std::make_unique<ExprASTVariable>(identName);
  }

  getNextToken();
  std::vector<std::unique_ptr<ExprAST>> args;
  if (curToken != ')') {
    while (1) {
      if (auto arg = parseExpr()) {
        args.push_back(std::move(arg));
      } else {
        return nullptr;
      }

      if (curToken == ')') {
        break;
      } else if (curToken != ',') {
        return logExprErr("Expected ')' or ',' in argument list");
      }

      getNextToken();
    }
  }

  getNextToken();
  return std::make_unique<ExprASTCall>(identName, std::move(args));
}

std::unique_ptr<ExprAST> parseBinOpRHS(int exprPrecedence,
                                       std::unique_ptr<ExprAST> lhs) {
  while (1) {
    int tokenPrecedence = getTokenPrecedence();
    if (tokenPrecedence > exprPrecedence) {
      return lhs;
    }

    int bin_op = curToken;
    getNextToken();

    auto rhs = parseEntryExpr();
    if (!rhs) {
      return nullptr;
    }

    int nextPrecedence = getTokenPrecedence();
    if (tokenPrecedence < nextPrecedence) {
      rhs = parseBinOpRHS(++tokenPrecedence, std::move(rhs));
      if (!rhs) {
        return nullptr;
      }
    }

    lhs =
        std::make_unique<ExprASTBinary>(bin_op, std::move(lhs), std::move(rhs));
  }
}

std::unique_ptr<ProtoAST> parseProto() {
  if (curToken != tokIdent) {
    return logProtoErr("Expected function name in prototype");
  }

  std::string func_name = identStr;
  getNextToken();

  if (curToken != '(') {
    return logProtoErr("Expected ')' in prototype");
  }

  std::vector<std::string> argNames;
  while (getNextToken() == tokIdent) {
    argNames.push_back(identStr);
  }

  if (curToken != ')') {
    return logProtoErr("Expected ')' in prototype");
  }

  getNextToken();
  return std::make_unique<ProtoAST>(func_name, std::move(argNames));
}

std::unique_ptr<ProtoAST> parseExtern() {
  getNextToken();
  return parseProto();
}

std::unique_ptr<FuncAST> parseDef() {
  getNextToken();
  auto proto = parseProto();
  if (!proto) {
    return nullptr;
  }

  if (auto expr = parseExpr()) {
    return std::make_unique<FuncAST>(std::move(proto), std::move(expr));
  }

  return nullptr;
}

std::unique_ptr<FuncAST> parseTopLevelExpr() {
  if (auto expr = parseExpr()) {
    auto proto = std::make_unique<ProtoAST>("", std::vector<std::string>());
    return std::make_unique<FuncAST>(std::move(proto), std::move(expr));
  }

  return nullptr;
}
