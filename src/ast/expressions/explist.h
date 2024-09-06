#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "./exp.h"

struct ScopedEnv;
struct FunctionEnv;
struct Exp;
struct Type;
struct Literal;

struct ExpList
{
    virtual void getOperandNames(std::vector<std::string> &operandNames) = 0;
    virtual void getReferenceNames(std::vector<std::vector<std::string>> &referenceNamesContainer) = 0;
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Literal>> &valueContainer) = 0;
    virtual void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer, std::vector<std::string> &typeErrors) = 0;
    virtual void getTypes(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer) = 0;
};

struct LastExpList : ExpList
{
    std::shared_ptr<Exp> last;

    LastExpList(Exp *last);

    void getOperandNames(std::vector<std::string> &operandNames) override;
    void getReferenceNames(std::vector<std::vector<std::string>> &referenceNamesContainer) override;
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Literal>> &valueContainer) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer, std::vector<std::string> &typeErrors) override;
    void getTypes(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer) override;
};

struct PairExpList : ExpList
{
    std::shared_ptr<Exp> head;
    std::shared_ptr<ExpList> tail;

    PairExpList(Exp *head, ExpList *tail);

    void getOperandNames(std::vector<std::string> &operandNames) override;
    void getReferenceNames(std::vector<std::vector<std::string>> &referenceNamesContainer) override;
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Literal>> &valueContainer) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer, std::vector<std::string> &typeErrors) override;
    void getTypes(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::shared_ptr<Type>> &typeContainer) override;
};