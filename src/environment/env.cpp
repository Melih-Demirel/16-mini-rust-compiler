#include "./env.h"
#include <iostream>

GlobalEnv::GlobalEnv() {}

std::shared_ptr<Literal> GlobalEnv::lookupVariable(std::string id)
{
    if (globalVariables.tableEntries.count(id))
    {
        return globalVariables.tableEntries.at(id).value;
    }
    else
    {
        return nullptr;
    }
}

std::pair<int, bool> GlobalEnv::variableExists(std::string id)
{
    if (globalVariables.tableEntries.count(id))
    {
        return std::make_pair(1, true);
    }
    else
    {
        return std::make_pair(0, true);
    }
}

bool GlobalEnv::isVarMutable(std::string id)
{
    if (globalVariables.tableEntries.count(id))
    {
        return globalVariables.tableEntries.at(id).isMutable;
    }
    else
    {
        return false;
    }
}

ScopedEnv::ScopedEnv()
{
    scopeTable.push_back(std::make_shared<SymbolTable>());
}

std::shared_ptr<SymbolTable> ScopedEnv::getCurrentScope()
{
    return scopeTable.back();
}

std::shared_ptr<Literal> ScopedEnv::lookupVariable(std::string id)
{
    int scopeDepth = scopeTable.size();

    for (int i = scopeDepth - 1; i >= 0; i--)
    {
        if (scopeTable[i]->tableEntries.count(id))
        {
            return scopeTable[i]->tableEntries.at(id).value;
        }
    }
    return nullptr;
}

// std::shared_ptr<Literal> ScopedEnv::lookupVariable(std::string id)
// {
//     int scopeDepth = scopeTable.size();

//     for (int i = scopeDepth - 1; i >= 0; i--)
//     {
//         if (scopeTable[i]->tableEntries.count(id) && (scopeTable[i]->localScopeId == scopeLevel || i == 0))
//             if (scopeTable[i]->tableEntries.count(id))
//             {
//                 return scopeTable[i]->tableEntries.at(id).value;
//             }
//     }
//     return nullptr;
// }

std::pair<int, bool> ScopedEnv::variableExists(std::string id)
{
    int scopeDepth = scopeTable.size();
    for (int i = scopeDepth - 1; i >= 0; i--)
    {
        if (scopeTable[i]->tableEntries.count(id))
        {
            if (i == 0)
            {
                return std::make_pair(scopeTable[i]->tableEntries.at(id).count, true);
            }
            else
            {
                return std::make_pair(scopeTable[i]->tableEntries.at(id).count, false);
            }
        }
    }
    return std::make_pair(0, false);
}

bool ScopedEnv::isVarMutable(std::string id)
{
    int scopeDepth = scopeTable.size();

    for (int i = scopeDepth - 1; i >= 0; i--)
    {
        if (scopeTable[i]->tableEntries.count(id))
        {
            return scopeTable[i]->tableEntries.at(id).isMutable;
        }
    }
    return false;
}

std::shared_ptr<Type> ScopedEnv::getVarType(std::string id)
{
    int scopeDepth = scopeTable.size();

    for (int i = scopeDepth - 1; i >= 0; i--)
    {
        if (scopeTable[i]->tableEntries.count(id))
        {
            return scopeTable[i]->tableEntries.at(id).type;
        }
    }
    return nullptr;
}

void ScopedEnv::updateVariable(std::string id, std::shared_ptr<Literal> newVal)
{
    for (int i = scopeTable.size() - 1; i >= 0; i++)
    {
        if (scopeTable[i]->tableEntries.count(id))
        {
            scopeTable[i]->tableEntries.at(id).value = newVal;
            return;
        }
    }
}

void ScopedEnv::popScope()
{
    scopeTable.pop_back();
}

void ScopedEnv::pushScope()
{
    scopeTable.push_back(std::make_shared<SymbolTable>());
}

std::shared_ptr<FunctionTableEntry> FunctionEnv::lookupVariable(std::string id)
{
    if (declaredFunctions.tableEntries.count(id))
    {
        return declaredFunctions.tableEntries.at(id);
    }
    else
    {
        return nullptr;
    }
}

std::string FunctionEnv::currentFunc()
{
    return callStack.back();
}

void FunctionEnv::pushFunc(std::string funcName)
{
    callStack.push_back(funcName);
}

void FunctionEnv::popFunc()
{
    callStack.pop_back();
}
