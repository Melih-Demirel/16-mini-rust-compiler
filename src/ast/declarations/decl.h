#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "../identifiers/identifier.h"
#include "../functions/specs.h"
#include "../block/block.h"
#include "../expressions/explist.h"
#include "../types/type.h"

struct ScopedEnv;
struct FunctionEnv;
struct Block;
struct ExpList;

struct Decl
{
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
    virtual void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) = 0;
    virtual void setupFuncEnv(FunctionEnv &env) = 0;
    virtual void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
};

struct VariableDecl : Decl
{

    std::shared_ptr<IdentifierList> idList;
    std::shared_ptr<Type> type;
    std::shared_ptr<ExpList> expList;
    std::shared_ptr<Block> block;
    bool isMutable;

    VariableDecl(IdentifierList *idList, Type *type, ExpList *expList, Block *block, bool isMutable);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    void setupFuncEnv(FunctionEnv &env) override;
    void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) override;
};

struct FunctionDecl : Decl
{
    std::shared_ptr<Identifier> funcName;
    std::shared_ptr<FunctionSpecs> funcSpecs;
    std::shared_ptr<Block> funcBody;

    FunctionDecl(Identifier *funcName, FunctionSpecs *sign, Block *body);
    FunctionDecl(std::shared_ptr<Identifier> funcName, std::shared_ptr<FunctionSpecs> sign, std::shared_ptr<Block> body);

    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    void setupFuncEnv(FunctionEnv &env) override;
    void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) override;
};