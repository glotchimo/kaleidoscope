#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

static void handle_def() {
  if (parse_def()) {
    fprintf(stderr, "Parsed a function definition\n");
  } else {
    get_next_token();
  }
}

static void handle_extern() {
  if (parse_extern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    get_next_token();
  }
}

static void handle_top_level_expr() {
  if (parse_top_level_expr()) {
    fprintf(stderr, "Parsed a top-level expression\n");
  } else {
    get_next_token();
  }
}

static void loop() {
  while (1) {
    fprintf(stderr, "kaleidoscope> ");
    switch (cur_tok) {
    case tok_eof:
      return;
    case ';':
      get_next_token();
      break;
    case tok_def:
      handle_def();
      break;
    case tok_extern:
      handle_extern();
      break;
    default:
      handle_top_level_expr();
      break;
    }
  }
}

int main() {
  bin_op_precedence['<'] = 10;
  bin_op_precedence['+'] = 20;
  bin_op_precedence['-'] = 20;
  bin_op_precedence['*'] = 40;

  fprintf(stderr, "kaleidoscope> ");
  get_next_token();

  loop();

  return 0;
}
