#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

static std::map<char, int> binOpPrecedence;

static int getTokenPrecedence() {
    if (!isascii(curTok)) {
        return -1;
    }

    int tokenPrecedence = binOpPrecedence[curTok];
    if (tokenPrecedence <= 0) {
        return -1;
    }

    return tokenPrecedence;
}

int main() {
    binOpPrecedence['<'] = 10;
    binOpPrecedence['+'] = 20;
    binOpPrecedence['-'] = 20;
    binOpPrecedence['*'] = 40;
}