#pragma once
#include <memory>
#include <utility>
#include "./paramlist.h"
#include "./returntypes.h"

struct ScopedEnv;
struct FunctionEnv;

struct FunctionSpecs
{
    std::shared_ptr<ParameterList> parameters;
    std::shared_ptr<ReturnType> returnTypes;

    FunctionSpecs(ParameterList *params, ReturnType *returnTypes);
};