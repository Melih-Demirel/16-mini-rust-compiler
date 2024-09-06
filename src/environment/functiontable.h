#pragma once
#include <map>
#include <string>
#include <memory>
#include "../ast/declarations/decl.h"

struct FunctionDecl;

struct FunctionTableEntry
{
    std::shared_ptr<FunctionDecl> funcDecl;
    std::vector<std::shared_ptr<Literal>> returnValues;
    int count;
    bool didReturn;

    FunctionTableEntry(std::shared_ptr<FunctionDecl> function);
};

struct FunctionTable
{
    std::map<std::string, std::shared_ptr<FunctionTableEntry>> tableEntries;

    FunctionTable();
    void addEntry(std::string id, std::shared_ptr<FunctionDecl> function);
    void addReturnValues(std::string id, std::vector<std::shared_ptr<Literal>> values);
    void resetReturnValues(std::string id);
    void getReturnTypes(std::string id, std::vector<std::shared_ptr<Type>> &typeContainer);
};
