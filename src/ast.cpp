#include "ast.hpp"

std::unique_ptr<llvm::LLVMContext> context;
std::unique_ptr<llvm::IRBuilder<>> builder;
std::unique_ptr<llvm::Module> module;
std::map<std::string, llvm::Value *> namedValues;

llvm::Value *NumberExprAST::codegen() {
  return llvm::ConstantFP::get(*context, llvm::APFloat(value));
}

llvm::Value *VariableExprAST::codegen() {
  llvm::Value *val = namedValues[name];
  if (!val)
    logValErr("unknown variable name");

  return val;
}

llvm::Value *BinaryExprAST::codegen() {
  llvm::Value *l = lhs->codegen();
  llvm::Value *r = rhs->codegen();
  if (!l || !r)
    return nullptr;

  switch (op) {
  case '+':
    return builder->CreateFAdd(l, r, "addtmp");
  case '-':
    return builder->CreateFSub(l, r, "subtmp");
  case '*':
    return builder->CreateFMul(l, r, "multmp");
  case '<':
    l = builder->CreateFCmpULT(l, r, "cmptmp");
    return builder->CreateUIToFP(l, llvm::Type::getDoubleTy(*context),
                                 "booltmp");
  default:
    return logValErr("invalid binary operator");
  }
}

llvm::Value *CallExprAST::codegen() {
  llvm::Function *calleeF = module->getFunction(callee);
  if (!calleeF)
    return logValErr("unknown function referenced");

  if (calleeF->arg_size() != args.size())
    return logValErr("incorrect # of arguments passed");

  std::vector<llvm::Value *> argsV;
  for (unsigned i = 0, e = args.size(); i != e; ++i) {
    argsV.push_back(args[i]->codegen());
    if (!argsV.back())
      return nullptr;
  }

  return builder->CreateCall(calleeF, argsV, "calltmp");
}

llvm::Function *ProtoAST::codegen() {
  std::vector<llvm::Type *> doubles(args.size(),
                                    llvm::Type::getDoubleTy(*context));
  llvm::FunctionType *ft = llvm::FunctionType::get(
      llvm::Type::getDoubleTy(*context), doubles, false);
  llvm::Function *f = llvm::Function::Create(
      ft, llvm::Function::ExternalLinkage, name, module.get());

  unsigned idx = 0;
  for (auto &arg : f->args())
    arg.setName(args[idx++]);

  return f;
}

llvm::Function *FuncAST::codegen() {
  llvm::Function *funcF = module->getFunction(proto->getName());

  if (!funcF)
    funcF = proto->codegen();

  if (!funcF)
    return nullptr;

  if (!funcF->empty())
    return (llvm::Function *)logValErr("function cannot be redefined");

  llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", funcF);
  builder->SetInsertPoint(bb);

  namedValues.clear();
  for (auto &arg : funcF->args())
    namedValues[std::string(arg.getName())] = &arg;

  if (llvm::Value *retVal = body->codegen()) {
    builder->CreateRet(retVal);
    llvm::verifyFunction(*funcF);
    return funcF;
  }

  funcF->eraseFromParent();
  return nullptr;
}

std::unique_ptr<ExprAST> logErr(const char *str) {
  fprintf(stderr, "Error: %s\n", str);
  return nullptr;
}

std::unique_ptr<ProtoAST> logProtoErr(const char *str) {
  logErr(str);
  return nullptr;
}

std::unique_ptr<FuncAST> logFuncErr(const char *str) {
  logErr(str);
  return nullptr;
}

llvm::Value *logValErr(const char *str) {
  logErr(str);
  return nullptr;
}
