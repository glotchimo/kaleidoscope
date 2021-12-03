#if !defined(KALEIDOSCOPE_LEXER_H)
#define KALEIDOSCOPE_LEXER_H

#include <string>

enum token {
    tok_eof = -1,
    tok_def = -2,
    tok_extern = -3,
    tok_ident = -4,
    tok_num = -5,
};

static std::string ident_str;
static double num_val;
static int cur_tok;

static int get_token();
static int get_next_token();

#endif  // KALEIDOSCOPE_LEXER_H
