#pragma once
#include <vector>
#include "./symboltable.h"
#include "./functiontable.h"

struct Env
{
    virtual std::shared_ptr<Literal> lookupVariable(std::string id) = 0;
    virtual std::pair<int, bool> variableExists(std::string id) = 0;
    virtual bool isVarMutable(std::string id) = 0;
    bool breakFlag = false;
    void setBreakFlag(bool flag)
    {
        breakFlag = flag;
    }

    bool hasBreakFlag() const
    {
        return breakFlag;
    }

    void clearBreakFlag()
    {
        breakFlag = false;
    }
};

struct GlobalEnv : Env
{
    SymbolTable globalVariables{};

    GlobalEnv();
    std::shared_ptr<Literal> lookupVariable(std::string id) override;
    std::pair<int, bool> variableExists(std::string id) override;
    bool isVarMutable(std::string id) override;
};

struct ScopedEnv : Env
{
    std::vector<std::shared_ptr<SymbolTable>> scopeTable;
    ScopedEnv();
    std::shared_ptr<Literal> lookupVariable(std::string id) override;
    std::pair<int, bool> variableExists(std::string id) override;
    bool isVarMutable(std::string id) override;
    std::shared_ptr<Type> getVarType(std::string id);
    std::shared_ptr<SymbolTable> getCurrentScope();
    void updateVariable(std::string id, std::shared_ptr<Literal> newVal);
    void popScope();
    void pushScope();
};

struct FunctionEnv
{
    FunctionTable declaredFunctions;
    std::vector<std::string> callStack;

    std::shared_ptr<FunctionTableEntry> lookupVariable(std::string id);
    std::string currentFunc();

    bool breakFlag = false;
    void setBreakFlag(bool flag)
    {
        breakFlag = flag;
    }

    bool hasBreakFlag() const
    {
        return breakFlag;
    }

    void clearBreakFlag()
    {
        breakFlag = false;
    }
    void pushFunc(std::string funcName);
    void popFunc();
};