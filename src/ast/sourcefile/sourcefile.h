#pragma once
#include <iostream>
#include <utility>
#include <memory>
#include "../declarations/decllist.h"

struct ScopedEnv;
struct FunctionEnv;

struct SourceFile
{
    std::shared_ptr<DeclList> globalDecls;

    SourceFile(DeclList *globalDecls);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv);
    void typeCheck(ScopedEnv &typeCheckingEnv, FunctionEnv &typeCheckingFuncEnv, std::vector<std::string> &typeErrors);
};