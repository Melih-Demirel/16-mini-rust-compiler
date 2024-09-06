#include "stmlist.h"
#include <iostream>
#include "../../environment/env.h"

LastStmList::LastStmList(Stm *last) : last{last} {};

void LastStmList::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    bool done = false;
    if (std::dynamic_pointer_cast<ExpressionStm>(last) != nullptr)
    {
        std::vector<std::shared_ptr<Literal>> returnValues;

        std::string funcName = funcEnv.currentFunc();
        std::shared_ptr<FunctionTableEntry> funcDetails = funcEnv.lookupVariable(funcName);
        if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
        {
            if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
                return;
        }
        funcEnv.lookupVariable(funcName)->didReturn = true;
        std::vector<std::string> returnValueIdentifiers;
        if (last != nullptr)
        {
            std::shared_ptr<ExpressionStm> exprStm = std::dynamic_pointer_cast<ExpressionStm>(last);
            if (exprStm && exprStm->exp != nullptr)
            {
                std::shared_ptr<Literal> literalResult = exprStm->exp->interpret(env, funcEnv);
                if (auto intLiteral = std::dynamic_pointer_cast<IntLiteral>(literalResult))
                {
                    returnValues.push_back(intLiteral);
                }
                else if (auto boolLiteral = std::dynamic_pointer_cast<IntLiteral>(literalResult))
                {
                    returnValues.push_back(boolLiteral);
                }
                done = true;
                std::reverse(returnValues.begin(), returnValues.end());
                funcEnv.declaredFunctions.addReturnValues(funcName, returnValues);
            }
        }
    }
    if (last != nullptr && !done)
    {
        last->interpret(env, funcEnv);
    }
}

void LastStmList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    last->typeCheck(env, funcEnv, typeErrors);
}

PairStmList::PairStmList(Stm *head, StmList *tail) : head{head}, tail{tail} {};

void PairStmList::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    head->interpret(env, funcEnv);
    tail->interpret(env, funcEnv);
}

void PairStmList::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    head->typeCheck(env, funcEnv, typeErrors);
    tail->typeCheck(env, funcEnv, typeErrors);
}
