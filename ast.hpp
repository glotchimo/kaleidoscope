#if !defined(KALEIDOSCOPE_AST_H)
#define KALEIDOSCOPE_AST_H

#include <memory>
#include <string>
#include <vector>

class expr_ast {
   public:
    virtual ~expr_ast() {}
};

class expr_ast_number : public expr_ast {
    double value;

   public:
    expr_ast_number(double val) : value(val) {}
};

class expr_ast_variable : public expr_ast {
    std::string name;

   public:
    expr_ast_variable(const std::string &name) : name(name) {}
};

class expr_ast_binary : public expr_ast {
    char op;
    expr_ast *lhs, *rhs;

   public:
    expr_ast_binary(char op, expr_ast *lhs, expr_ast *rhs) : op(op), lhs(lhs), rhs(rhs) {}
};

class expr_ast_call : public expr_ast {
    std::string callee;
    std::vector<expr_ast *> args;

   public:
    expr_ast_call(const std::string &callee, std::vector<expr_ast *> &args) : callee(callee), args(args) {}
};

class proto_ast {
    std::string name;
    std::vector<std::string> args;

   public:
    proto_ast(const std::string &name, const std::vector<std::string> &args) : name(name), args(args) {}
};

class func_ast {
    proto_ast *proto;
    expr_ast *body;

   public:
    func_ast(proto_ast *proto, expr_ast *body) : proto(proto), body(body) {}
};

std::unique_ptr<expr_ast> error_expr(const char *Str);
std::unique_ptr<proto_ast> error_proto(const char *Str);
std::unique_ptr<func_ast> error_func(const char *Str);

#endif  // KALEIDOSCOPE_AST_H
