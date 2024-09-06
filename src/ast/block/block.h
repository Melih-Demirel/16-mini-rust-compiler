#pragma once
#include <memory>
#include <utility>
#include "../statements/stmlist.h"
#include "../expressions/exp.h"

struct StmList;
struct ScopedEnv;
struct FunctionEnv;

struct Block
{
    std::shared_ptr<StmList> statements;
    std::shared_ptr<ExpList> expressions;

    Block(StmList *stmList, ExpList *exprList);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv);
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors);
};