#include "Lexer.hpp"

static int getToken() {
    static int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar)) {
        IdentStr = LastChar;
        while (isalnum(LastChar = getchar()))
            IdentStr += LastChar;

        if (IdentStr == "def")
            return tok_def;
        else if (IdentStr == "extern")
            return tok_extern;
        else
            return tok_ident;
    } else if (isdigit(LastChar || LastChar == '.')) {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar || LastChar == '.'));

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_num;
    } else if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return getToken();
    } else if (LastChar == EOF) {
        return tok_eof;
    } else {
        int ThisChar = LastChar;
        LastChar = getchar();
        return ThisChar;
    }
}

static int getNextToken() {
    return CurTok = getToken();
}