#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

static void handleDef() {
  if (parseDef()) {
    fprintf(stderr, "Parsed a function definition\n");
  } else {
    getNextToken();
  }
}

static void handleExtern() {
  if (parseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    getNextToken();
  }
}

static void handleTopLevelExpr() {
  if (parseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expression\n");
  } else {
    getNextToken();
  }
}

static void loop() {
  while (1) {
    fprintf(stderr, "kaleidoscope> ");
    switch (curToken) {
    case tokEOF:
      return;
    case ';':
      getNextToken();
      break;
    case tokDef:
      handleDef();
      break;
    case tokExtern:
      handleExtern();
      break;
    default:
      handleTopLevelExpr();
      break;
    }
  }
}

int main() {
  binOpPrecedence['<'] = 10;
  binOpPrecedence['+'] = 20;
  binOpPrecedence['-'] = 20;
  binOpPrecedence['*'] = 40;

  fprintf(stderr, "kaleidoscope> ");
  getNextToken();

  loop();

  return 0;
}
