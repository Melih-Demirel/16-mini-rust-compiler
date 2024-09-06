#pragma once
#include <memory>
#include <utility>
#include "./parameterdecl.h"

struct ScopedEnv;
struct FunctionEnv;

struct ParameterList
{
    virtual void getIdentifiersWithTypes(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> &identifiersWithType) = 0;
    virtual void getIdentifiers(std::vector<std::string> &identifierList) = 0;
};

struct LastParamList : ParameterList
{
    std::shared_ptr<ParameterDecl> last;

    LastParamList(ParameterDecl *last);

    void getIdentifiersWithTypes(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> &identifiersWithType) override;
    void getIdentifiers(std::vector<std::string> &identifierList) override;
};

struct PairParamList : ParameterList
{
    std::shared_ptr<ParameterDecl> head;
    std::shared_ptr<ParameterList> tail;

    PairParamList(ParameterDecl *head, ParameterList *tail);

    void getIdentifiersWithTypes(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> &identifiersWithType) override;
    void getIdentifiers(std::vector<std::string> &identifierList) override;
};