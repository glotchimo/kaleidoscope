#if !defined(KALEIDOSCOPE_LEXER_H)
#define KALEIDOSCOPE_LEXER_H

#include <string>

enum Token {
  tokEOF = -1,
  tokDef = -2,
  tokExtern = -3,
  tokIdent = -4,
  tokNum = -5,
};

extern std::string identStr;
extern double numVal;
extern int curToken;

int getToken();
int getNextToken();

#endif // KALEIDOSCOPE_LEXER_H
