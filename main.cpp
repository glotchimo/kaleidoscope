#include <map>
#include <string>

#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main() {
    bin_op_precedence['<'] = 10;
    bin_op_precedence['+'] = 20;
    bin_op_precedence['-'] = 20;
    bin_op_precedence['*'] = 40;
}