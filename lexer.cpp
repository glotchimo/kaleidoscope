#include "lexer.hpp"

static int get_token() {
    static int last_char = ' ';

    // eat whitespace
    while (isspace(last_char)) {
        last_char = getchar();
    }

    // parse for identifier or keywords (def, extern)
    if (isalpha(last_char)) {
        ident_str = last_char;

        while (isalnum(last_char = getchar())) {
            ident_str += last_char;
        }

        if (ident_str == "def") {
            return tok_def;
        } else if (ident_str == "extern") {
            return tok_extern;
        } else {
            return tok_ident;
        }
    }

    // parse for numeric literals
    else if (isdigit(last_char || last_char == '.')) {
        std::string num_str;

        do {
            num_str += last_char;
            last_char = getchar();
        } while (isdigit(last_char || last_char == '.'));

        num_val = strtod(num_str.c_str(), 0);

        return tok_num;
    }

    // eat comments
    else if (last_char == '#') {
        do {
            last_char = getchar();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF) {
            return get_token();
        }
    }

    // parse end-of-file characters
    else if (last_char == EOF) {
        return tok_eof;
    }

    // default parse continuation
    else {
        int this_char = last_char;
        last_char = getchar();

        return this_char;
    }
}

static int get_next_token() {
    return cur_tok = get_token();
}