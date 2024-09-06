#include "explist.h"
#include <iostream>
#include <algorithm>

LastExpList::LastExpList(Exp *last) : last{last} {}

void LastExpList::getOperandNames(std::vector<std::string> &operandNames)
{
    operandNames.push_back(last->getOperandName());
}

void LastExpList::getReferenceNames(std::vector<std::vector<std::string>> &referenceNamesContainer)
{
    std::vector<std::string> newReferenceNames{};
    last->getReferenceNames(newReferenceNames);
    referenceNamesContainer.push_back(newReferenceNames);
}

void LastExpList::interpret(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Literal>> &valueContainer)
{
    if (std::dynamic_pointer_cast<FuncCall>(last) == nullptr)
    {
        valueContainer.push_back(last->interpret(env, funcEnv));
    }
    else
    {
        std::vector<std::shared_ptr<Literal>> returnValues = std::dynamic_pointer_cast<FuncCall>(last)->runFunc(env, funcEnv);
        for (std::shared_ptr<Literal> r : returnValues)
        {
            valueContainer.push_back(r);
        }
    }
}

void LastExpList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer, std::vector<std::string> &typeErrors)
{
    if (std::dynamic_pointer_cast<FuncCall>(last) == nullptr)
    {
        typeContainer.push_back(last->typeCheck(env, funcEnv, typeErrors));
    }
    else
    {
        std::vector<std::shared_ptr<Type>> returnTypes = std::dynamic_pointer_cast<FuncCall>(last)->checkFunc(env, funcEnv, typeErrors);
        for (std::shared_ptr<Type> r : returnTypes)
        {
            typeContainer.push_back(r);
        }
    }
}

void LastExpList::getTypes(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer)
{
    std::vector<std::shared_ptr<Type>> types = last->getType(env, funcEnv);
    std::reverse(types.begin(), types.end());

    for (auto t : types)
    {
        typeContainer.push_back(t);
    }
}

PairExpList::PairExpList(Exp *h, ExpList *t) : head{h}, tail{t} {}

void PairExpList::getOperandNames(std::vector<std::string> &operandNames)
{
    operandNames.push_back(head->getOperandName());
    tail->getOperandNames(operandNames);
}

void PairExpList::getReferenceNames(std::vector<std::vector<std::string>> &referenceNamesContainer)
{
    std::vector<std::string> newReferenceNames{};
    head->getReferenceNames(newReferenceNames);
    referenceNamesContainer.push_back(newReferenceNames);
    tail->getReferenceNames(referenceNamesContainer);
}

void PairExpList::interpret(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Literal>> &valueContainer)
{
    if (std::dynamic_pointer_cast<FuncCall>(head) == nullptr)
    {
        valueContainer.push_back(head->interpret(env, funcEnv));
    }
    else
    {
        std::vector<std::shared_ptr<Literal>> returnValues = std::dynamic_pointer_cast<FuncCall>(head)->runFunc(env, funcEnv);
        for (std::shared_ptr<Literal> r : returnValues)
        {
            valueContainer.push_back(r);
        }
    }
    tail->interpret(env, funcEnv, valueContainer);
    std::reverse(valueContainer.begin(), valueContainer.end());
}

void PairExpList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer, std::vector<std::string> &typeErrors)
{
    if (std::dynamic_pointer_cast<FuncCall>(head) == nullptr)
    {
        typeContainer.push_back(head->typeCheck(env, funcEnv, typeErrors));
    }
    else
    {
        std::vector<std::shared_ptr<Type>> returnTypes = std::dynamic_pointer_cast<FuncCall>(head)->checkFunc(env, funcEnv, typeErrors);
        for (std::shared_ptr<Type> r : returnTypes)
        {
            typeContainer.push_back(r);
        }
    }
    tail->typeCheck(env, funcEnv, typeContainer, typeErrors);
    std::reverse(typeContainer.begin(), typeContainer.end());
}

void PairExpList::getTypes(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer)
{
    std::vector<std::shared_ptr<Type>> types = head->getType(env, funcEnv);
    std::reverse(types.begin(), types.end());

    for (auto type : types)
    {
        typeContainer.push_back(type);
    }

    tail->getTypes(env, funcEnv, typeContainer);
}
