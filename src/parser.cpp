#include "parser.hpp"

std::map<char, int> bin_op_precedence;

int get_token_precedence() {
  if (!isascii(cur_tok)) {
    return -1;
  }

  int token_precedence = bin_op_precedence[cur_tok];
  if (token_precedence <= 0) {
    return -1;
  }

  return token_precedence;
}

std::unique_ptr<expr_ast> parse_expr() {
  auto lhs = parse_expr_entry();
  if (!lhs) {
    return nullptr;
  }

  return parse_bin_op_rhs(0, std::move(lhs));
}

std::unique_ptr<expr_ast> parse_expr_entry() {
  switch (cur_tok) {
  case tok_ident:
    return parse_expr_ident();
  case tok_num:
    return parse_expr_num();
  case '(':
    return parse_expr_paren();
  default:
    return error_expr("unknown token when expecting an expression");
  }
}

std::unique_ptr<expr_ast> parse_expr_num() {
  auto result = std::make_unique<expr_ast_number>(num_val);
  get_next_token();
  return std::move(result);
}

std::unique_ptr<expr_ast> parse_expr_paren() {
  get_next_token();
  auto expr = parse_expr();
  if (!expr) {
    return nullptr;
  }

  if (cur_tok != ')') {
    return error_expr("expected ')'");
  }

  get_next_token();
  return expr;
}

std::unique_ptr<expr_ast> parse_expr_ident() {
  std::string ident_name = ident_str;

  get_next_token();
  if (cur_tok != '(') {
    return std::make_unique<expr_ast_variable>(ident_name);
  }

  get_next_token();
  std::vector<std::unique_ptr<expr_ast>> args;
  if (cur_tok != ')') {
    while (1) {
      if (auto arg = parse_expr()) {
        args.push_back(std::move(arg));
      } else {
        return nullptr;
      }

      if (cur_tok == ')') {
        break;
      } else if (cur_tok != ',') {
        return error_expr("Expected ')' or ',' in argument list");
      }

      get_next_token();
    }
  }

  get_next_token();
  return std::make_unique<expr_ast_call>(ident_name, std::move(args));
}

std::unique_ptr<expr_ast> parse_bin_op_rhs(int expr_precedence,
                                           std::unique_ptr<expr_ast> lhs) {
  while (1) {
    int tok_precedence = get_token_precedence();
    if (tok_precedence > expr_precedence) {
      return lhs;
    }

    int bin_op = cur_tok;
    get_next_token();

    auto rhs = parse_expr_entry();
    if (!rhs) {
      return nullptr;
    }

    int next_precedence = get_token_precedence();
    if (tok_precedence < next_precedence) {
      rhs = parse_bin_op_rhs(++tok_precedence, std::move(rhs));
      if (!rhs) {
        return nullptr;
      }
    }

    lhs = std::make_unique<expr_ast_binary>(bin_op, std::move(lhs),
                                            std::move(rhs));
  }
}

std::unique_ptr<proto_ast> parse_proto() {
  if (cur_tok != tok_ident) {
    return error_proto("Expected function name in prototype");
  }

  std::string func_name = ident_str;
  get_next_token();

  if (cur_tok != '(') {
    return error_proto("Expected ')' in prototype");
  }

  std::vector<std::string> arg_names;
  while (get_next_token() == tok_ident) {
    arg_names.push_back(ident_str);
  }

  if (cur_tok != ')') {
    return error_proto("Expected ')' in prototype");
  }

  get_next_token();
  return std::make_unique<proto_ast>(func_name, std::move(arg_names));
}

std::unique_ptr<proto_ast> parse_extern() {
  get_next_token();
  return parse_proto();
}

std::unique_ptr<func_ast> parse_def() {
  get_next_token();
  auto proto = parse_proto();
  if (!proto) {
    return nullptr;
  }

  if (auto expr = parse_expr()) {
    return std::make_unique<func_ast>(std::move(proto), std::move(expr));
  }

  return nullptr;
}

std::unique_ptr<func_ast> parse_top_level_expr() {
  if (auto expr = parse_expr()) {
    auto proto = std::make_unique<proto_ast>("", std::vector<std::string>());
    return std::make_unique<func_ast>(std::move(proto), std::move(expr));
  }

  return nullptr;
}
