#pragma once
#include <string>

struct ScopedEnv;
struct FunctionEnv;

struct Identifier
{
    std::string name;
    Identifier(char *name);
};