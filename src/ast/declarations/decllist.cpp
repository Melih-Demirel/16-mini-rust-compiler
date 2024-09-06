#include <iostream>
#include "decllist.h"

PairDeclList::PairDeclList(Decl *h, DeclList *t) : head{h}, tail{t} {}

void PairDeclList::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    head->interpret(env, funcEnv);
    tail->interpret(env, funcEnv);
}

void PairDeclList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    head->typeCheck(env, funcEnv, typeErrors);
    tail->typeCheck(env, funcEnv, typeErrors);
}

void PairDeclList::setupFuncEnv(FunctionEnv &env)
{
    head->setupFuncEnv(env);
    tail->setupFuncEnv(env);
}

void PairDeclList::setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv)
{
    head->setupGlobalEnv(env, funcEnv);
    tail->setupGlobalEnv(env, funcEnv);
}

LastDeclList::LastDeclList(Decl *l) : last{l} {}

void LastDeclList::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    last->interpret(env, funcEnv);
}

void LastDeclList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    last->typeCheck(env, funcEnv, typeErrors);
}

void LastDeclList::setupFuncEnv(FunctionEnv &env)
{
    last->setupFuncEnv(env);
}

void LastDeclList::setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv)
{
    last->setupGlobalEnv(env, funcEnv);
}