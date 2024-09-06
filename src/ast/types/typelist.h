#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include "./type.h"

struct ScopedEnv;
struct FunctionEnv;
struct Type;

struct TypeList
{
    virtual void getTypes(std::vector<std::shared_ptr<Type>> &typesContainer) = 0;
};

struct LastTypeList : TypeList
{
    std::shared_ptr<Type> last;

    LastTypeList(Type *last);
    void getTypes(std::vector<std::shared_ptr<Type>> &typesContainer) override;
};

struct PairTypeList : TypeList
{
    std::shared_ptr<Type> head;
    std::shared_ptr<TypeList> tail;

    PairTypeList(Type *head, TypeList *tail);
    void getTypes(std::vector<std::shared_ptr<Type>> &typesContainer) override;
};