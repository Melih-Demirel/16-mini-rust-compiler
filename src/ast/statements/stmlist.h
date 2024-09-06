#pragma once
#include <memory>
#include <utility>
#include "./stm.h"

struct ScopedEnv;
struct FunctionEnv;
struct Stm;

struct StmList
{
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
    virtual void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) = 0;
};

struct LastStmList : StmList
{
    std::shared_ptr<Stm> last;

    LastStmList(Stm *last);

    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct PairStmList : StmList
{
    std::shared_ptr<Stm> head;
    std::shared_ptr<StmList> tail;

    PairStmList(Stm *head, StmList *tail);

    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};