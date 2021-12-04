#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include <map>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

extern std::map<char, int> bin_op_precedence;

std::unique_ptr<expr_ast> parse_expr();
std::unique_ptr<expr_ast> parse_expr_entry();
std::unique_ptr<expr_ast> parse_expr_num();
std::unique_ptr<expr_ast> parse_expr_paren();
std::unique_ptr<expr_ast> parse_expr_ident();

std::unique_ptr<expr_ast> parse_bin_op_rhs(int expr_precedence,
                                           std::unique_ptr<expr_ast> lhs);

std::unique_ptr<proto_ast> parse_proto();
std::unique_ptr<proto_ast> parse_extern();

std::unique_ptr<func_ast> parse_def();
std::unique_ptr<func_ast> parse_top_level_expr();

#endif // KALEIDOSCOPE_PARSER_H
