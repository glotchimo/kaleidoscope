#include "lexer.hpp"

std::string identStr;
double numVal;
int curToken;

int getToken() {
  static int lastChar = ' ';

  // eat whitespace
  while (isspace(lastChar)) {
    lastChar = getchar();
  }

  // parse for identifier or keywords (def, extern)
  if (isalpha(lastChar)) {
    identStr = lastChar;

    while (isalnum(lastChar = getchar()))
      identStr += lastChar;

    if (identStr == "def")
      return tokDef;
    else if (identStr == "extern")
      return tokExtern;
    else
      return tokIdent;
  }

  // parse for numeric literals
  if (isdigit(lastChar) || lastChar == '.') {
    std::string numStr;

    do {
      numStr += lastChar;
      lastChar = getchar();
    } while (isdigit(lastChar) || lastChar == '.');

    numVal = strtod(numStr.c_str(), nullptr);
    return tokNum;
  }

  // eat comments
  if (lastChar == '#') {
    do
      lastChar = getchar();
    while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

    if (lastChar != EOF)
      return getToken();
  }

  // parse end-of-file characters
  if (lastChar == EOF)
    return tokEOF;

  int thisChar = lastChar;
  lastChar = getchar();
  return thisChar;
}

int getNextToken() { return curToken = getToken(); }
