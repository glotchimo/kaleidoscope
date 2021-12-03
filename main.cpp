#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main() {
    binOpPrecedence['<'] = 10;
    binOpPrecedence['+'] = 20;
    binOpPrecedence['-'] = 20;
    binOpPrecedence['*'] = 40;
}