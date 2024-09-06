#pragma once
#include <memory>
#include <utility>
#include <vector>
#include <string>
#include "./identifier.h"

struct ScopedEnv;
struct FunctionEnv;
struct Identifier;

struct IdentifierList
{
    virtual void getIdentifierStrings(std::vector<std::string> &container) = 0;
};

struct LastIdentifierList : IdentifierList
{
    std::shared_ptr<Identifier> last;

    LastIdentifierList(Identifier *last);

    void getIdentifierStrings(std::vector<std::string> &identifiersContainer) override;
};

struct PairIdentifierList : IdentifierList
{
    std::shared_ptr<Identifier> head;
    std::shared_ptr<IdentifierList> tail;

    PairIdentifierList(Identifier *head, IdentifierList *tail);

    void getIdentifierStrings(std::vector<std::string> &identifiersContainer) override;
};