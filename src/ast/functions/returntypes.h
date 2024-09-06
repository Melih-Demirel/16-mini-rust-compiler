#pragma once
#include <memory>
#include <utility>
#include "../types/typelist.h"
#include "../types/type.h"
#include "./paramlist.h"

struct ScopedEnv;
struct FunctionEnv;

struct ReturnType
{
    std::shared_ptr<TypeList> types;

    ReturnType(TypeList *types);
    void getTypes(std::vector<std::shared_ptr<Type>> &typesContainer);
};