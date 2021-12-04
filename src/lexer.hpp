#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <string>

enum token {
  tok_eof = -1,
  tok_def = -2,
  tok_extern = -3,
  tok_ident = -4,
  tok_num = -5,
};

extern std::string ident_str;
extern double num_val;
extern int cur_tok;

int get_token();
int get_next_token();

#endif // KALEIDOSCOPE_LEXER_H
