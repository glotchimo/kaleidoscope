#include "lexer.hpp"

std::string identStr;
double numVal;
int curToken;

int getToken() {
  static int last_char = ' ';

  // eat whitespace
  while (isspace(last_char)) {
    last_char = getchar();
  }

  // parse for identifier or keywords (def, extern)
  if (isalpha(last_char)) {
    identStr = last_char;

    while (isalnum(last_char = getchar())) {
      identStr += last_char;
    }

    if (identStr == "def") {
      return tokDef;
    } else if (identStr == "extern") {
      return tokExtern;
    } else {
      return tokIdent;
    }
  }

  // parse for numeric literals
  else if (isdigit(last_char || last_char == '.')) {
    std::string num_str;

    do {
      num_str += last_char;
      last_char = getchar();
    } while (isdigit(last_char || last_char == '.'));

    numVal = strtod(num_str.c_str(), 0);

    return tokNum;
  }

  // eat comments
  else if (last_char == '#') {
    do {
      last_char = getchar();
    } while (last_char != EOF && last_char != '\n' && last_char != '\r');

    if (last_char != EOF) {
      return getToken();
    }
  }

  // parse end-of-file characters
  else if (last_char == EOF) {
    return tokEOF;
  }
  int this_char = last_char;
  last_char = getchar();

  return this_char;
}

int getNextToken() { return curToken = getToken(); }
