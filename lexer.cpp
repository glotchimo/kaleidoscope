#include "lexer.hpp"

static int getToken() {
    static int lastChar = ' ';

    // eat whitespace
    while (isspace(lastChar)) {
        lastChar = getchar();
    }

    // parse for identifier or keywords (def, extern)
    if (isalpha(lastChar)) {
        identStr = lastChar;

        while (isalnum(lastChar = getchar())) {
            identStr += lastChar;
        }

        if (identStr == "def") {
            return tok_def;
        } else if (identStr == "extern") {
            return tok_extern;
        } else {
            return tok_ident;
        }
    }

    // parse for numeric literals
    else if (isdigit(lastChar || lastChar == '.')) {
        std::string numStr;

        do {
            numStr += lastChar;
            lastChar = getchar();
        } while (isdigit(lastChar || lastChar == '.'));

        numVal = strtod(numStr.c_str(), 0);

        return tok_num;
    }

    // eat comments
    else if (lastChar == '#') {
        do {
            lastChar = getchar();
        } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');

        if (lastChar != EOF) {
            return getToken();
        }
    }

    // parse end-of-file characters
    else if (lastChar == EOF) {
        return tok_eof;
    }

    // default parse continuation
    else {
        int ThisChar = lastChar;
        lastChar = getchar();

        return ThisChar;
    }
}

static int getNextToken() {
    return curTok = getToken();
}