#pragma once
#include <utility>

struct ScopedEnv;
struct FunctionEnv;

struct Type
{
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
};

struct IntegerType : Type
{
    IntegerType();
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
};

struct BooleanType : Type
{
    BooleanType();
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
};