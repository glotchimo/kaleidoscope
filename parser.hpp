#if !defined(KALEIDOSCOPE_PARSER_H)
#define KALEIDOSCOPE_PARSER_H

#include <map>
#include <memory>

#include "ast.hpp"
#include "lexer.hpp"

static std::map<char, int> bin_op_precedence;

static std::unique_ptr<expr_ast> parse_expr();
static std::unique_ptr<expr_ast> parse_expr_num();
static std::unique_ptr<expr_ast> parse_expr_paren();
static std::unique_ptr<expr_ast> parse_expr_ident();
static std::unique_ptr<expr_ast> parse_bin_op_rhs(int expr_precedence, std::unique_ptr<expr_ast> lhs);
static std::unique_ptr<expr_ast> parse_entry();

#endif  // KALEIDOSCOPE_PARSER_H
