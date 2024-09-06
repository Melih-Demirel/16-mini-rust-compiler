#include "block.h"
#include "../../environment/env.h"
#include <iostream>

Block::Block(StmList *stmList, ExpList *exprList) : statements{stmList}, expressions{exprList} {};

void Block::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{

    if (statements != nullptr)
    {
        statements->interpret(env, funcEnv);
    }
    if (expressions != nullptr)
    {
        std::vector<std::shared_ptr<Literal>> returnValues;

        std::string funcName = funcEnv.currentFunc();
        if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
        {
            if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
                return;
        }
        funcEnv.lookupVariable(funcName)->didReturn = true;
        {
            if (expressions != nullptr)
            {
                expressions->interpret(env, funcEnv, returnValues);

                std::reverse(returnValues.begin(), returnValues.end());
                funcEnv.declaredFunctions.addReturnValues(funcName, returnValues);
            }
        }
    }
};

void Block::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (statements != nullptr)
    {
        statements->typeCheck(env, funcEnv, typeErrors);
    }
}