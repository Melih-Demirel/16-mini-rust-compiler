#pragma once
#include <map>
#include <string>
#include <memory>
#include "../ast/types/type.h"
#include "../ast/literals/literal.h"

struct TableEntry
{
    std::shared_ptr<Type> type;
    std::shared_ptr<Literal> value;
    bool isMutable;
    int count;

    TableEntry(std::shared_ptr<Type> type, std::shared_ptr<Literal> value, bool mutableFlag);
};

struct SymbolTable
{
    std::map<std::string, TableEntry> tableEntries;

    SymbolTable();
    void addEntry(std::string id, std::shared_ptr<Type> type, std::shared_ptr<Literal> value, bool mutableFlag);
    void updateEntry(std::string id, std::shared_ptr<Literal> value);
};
