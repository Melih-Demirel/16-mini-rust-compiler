#pragma once
#include <memory>
#include <utility>
#include "./decl.h"

struct ScopedEnv;
struct FunctionEnv;

struct DeclList
{
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
    virtual void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) = 0;
    virtual void setupFuncEnv(FunctionEnv &env) = 0;
    virtual void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
};

struct PairDeclList : DeclList
{
    std::shared_ptr<Decl> head;
    std::shared_ptr<DeclList> tail;

    PairDeclList(Decl *head, DeclList *tail);

    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    void setupFuncEnv(FunctionEnv &env) override;
    void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) override;
};

struct LastDeclList : DeclList
{
    std::shared_ptr<Decl> last;

    LastDeclList(Decl *l);

    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    void setupFuncEnv(FunctionEnv &env) override;
    void setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv) override;
};