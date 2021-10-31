#if !defined(KALEIDOSCOPE_LEXER_H)
#define KALEIDOSCOPE_LEXER_H

#include <string>

enum Token {
    tok_eof = -1,
    tok_def = -2,
    tok_extern = -3,
    tok_ident = -4,
    tok_num = -5,
};

static std::string IdentStr;
static double NumVal;
static int CurTok;

static int getToken();
static int getNextToken();

#endif // KALEIDOSCOPE_LEXER_H
