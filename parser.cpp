#include "parser.hpp"

static int get_token_precedence() {
  if (!isascii(cur_tok)) {
    return -1;
  }

  int token_precedence = bin_op_precedence[cur_tok];
  if (token_precedence <= 0) {
    return -1;
  }

  return token_precedence;
}

static std::unique_ptr<expr_ast> parse_expr() {
  auto lhs = parse_expr_entry();
  if (!lhs) {
    return nullptr;
  }

  return parse_bin_op_rhs(0, std::move(lhs));
}

static std::unique_ptr<expr_ast> parse_expr_entry() {
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

static std::unique_ptr<expr_ast> parse_expr_num() {
  std::unique_ptr<expr_ast> result(new expr_ast_number(num_val));
  get_next_token();
  return result;
}

static std::unique_ptr<expr_ast> parse_expr_paren() {
  get_next_token();
  std::unique_ptr<expr_ast> expr = parse_expr();

  if (!expr) {
    return 0;
  }

  if (cur_tok != ')') {
    return error_expr("expected ')'");
  }

  get_next_token();
  return expr;
}

static std::unique_ptr<expr_ast> parse_expr_ident() {
  std::string ident_name = ident_str;

  get_next_token();
  if (cur_tok != '(') {
    return std::make_unique<expr_ast_variable>(ident_name);
  }

  get_next_token();
  std::vector<std::unique_ptr<expr_ast>> args;
  if (cur_tok != ')') {
    while (1) {
      std::unique_ptr<expr_ast> arg = parse_expr();
      if (!arg) {
        return 0;
      }
      args.push_back(arg);

      if (cur_tok == ')') {
        break;
      } else if (cur_tok != ',') {
        return error_expr("Expected ')' or ',' in argument list");
      }

      get_next_token();
    }
  }

  get_next_token();

  return std::make_unique<expr_ast_call>(ident_name, args);
}

static std::unique_ptr<expr_ast>
parse_bin_op_rhs(int expr_precedence, std::unique_ptr<expr_ast> lhs) {
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

static std::unique_ptr<proto_ast> parse_proto() {
  if (cur_tok != tok_ident) {
    return error_proto("expected function name in prototype");
  }

  std::string func_name = ident_str;
  get_next_token();

  if (cur_tok != '(') {
    return error_proto("expected ')' in prototype");
  }

  std::vector<std::string> arg_names;
  while (get_next_token() == tok_ident) {
    arg_names.push_back(ident_str);
  }

  if (cur_tok != ')') {
    return error_proto("expected ')' in prototype");
  }

  get_next_token();

  return std::make_unique<proto_ast>(func_name, std::move(arg_names));
}

static std::unique_ptr<proto_ast> parse_extern() {
  get_next_token();
  return parse_proto();
}

static std::unique_ptr<func_ast> parse_def() {
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

static std::unique_ptr<func_ast> parse_top_level_expr() {
  if (auto expr = parse_expr()) {
    auto proto = std::make_unique<proto_ast>("", std::vector<std::string>());
    return std::make_unique<func_ast>(std::move(proto), std::move(expr));
  }

  return nullptr;
}
