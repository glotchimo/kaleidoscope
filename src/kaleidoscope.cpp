#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

static void initModule() {
  context = std::make_unique<llvm::LLVMContext>();
  module = std::make_unique<llvm::Module>("kaleidoscope", *context);
  builder = std::make_unique<llvm::IRBuilder<>>(*context);
}

static void handleDef() {
  if (auto fnAST = parseDef()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "Read function definition:\n\n");
      fnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

static void handleExtern() {
  if (auto protoAST = parseExtern()) {
    if (auto *fnIR = protoAST->codegen()) {
      fprintf(stderr, "Read extern:\n\n");
      fnIR->print(llvm::errs());
      fprintf(stderr, "\n");
    }
  } else {
    getNextToken();
  }
}

static void handleTopLevelExpr() {
  if (auto fnAST = parseTopLevelExpr()) {
    if (auto *fnIR = fnAST->codegen()) {
      fprintf(stderr, "Read top-level expression:\n\n");
      fnIR->print(llvm::errs());
      fprintf(stderr, "\n");
      fnIR->eraseFromParent();
    }
  } else {
    getNextToken();
  }
}

static void loop() {
  while (true) {
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

  initModule();

  loop();

  module->print(llvm::errs(), nullptr);

  return 0;
}
