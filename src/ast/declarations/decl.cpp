#include <iostream>
#include "decl.h"
#include "../../environment/env.h"

VariableDecl::VariableDecl(IdentifierList *idList, Type *type, ExpList *expList, Block *block, bool isMutable) : idList{idList}, type{type}, expList{expList}, block{block}, isMutable{isMutable}
{
}
void VariableDecl::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::string> identifiers;
    idList->getIdentifierStrings(identifiers);

    std::vector<std::shared_ptr<Literal>> values;
    if (expList != nullptr)
    {
        expList->interpret(env, funcEnv, values);
    }
    else
    {
        if (std::dynamic_pointer_cast<BooleanType>(type) != nullptr)
        {
            for (int i = 0; i < identifiers.size(); i++)
            {
                values.push_back(std::make_shared<BoolLiteral>(false));
            }
        }
        if (std::dynamic_pointer_cast<IntegerType>(type) != nullptr)
        {
            for (int i = 0; i < identifiers.size(); i++)
            {
                values.push_back(std::make_shared<IntLiteral>(0));
            }
        }
    }
    for (int i = 0; i < identifiers.size(); i++)
    {
        env.getCurrentScope()->addEntry(identifiers[i], type, values[i], isMutable);
    }
}

void VariableDecl::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    std::vector<std::string> identifiers;
    idList->getIdentifierStrings(identifiers);
    std::vector<std::shared_ptr<Type>> identifierTypes;
    if (expList != nullptr)
    {
        expList->typeCheck(env, funcEnv, identifierTypes, typeErrors);
    }

    if (identifiers.size() != identifierTypes.size() && identifierTypes.size() != 0)
    {
        typeErrors.push_back("Type error in VariableDecl: Amount variables left side not same as right.");
    }

    for (std::shared_ptr<Type> t : identifierTypes)
    {
        if (type != nullptr)
        {
            if (std::dynamic_pointer_cast<BooleanType>(type) != nullptr && std::dynamic_pointer_cast<BooleanType>(t) == nullptr)
            {
                typeErrors.push_back("Type error in VariableDecl: variable " + identifiers[0] + " is of type boolean, you gave something else.");
            }
            if (std::dynamic_pointer_cast<IntegerType>(type) != nullptr && std::dynamic_pointer_cast<IntegerType>(t) == nullptr)
            {
                typeErrors.push_back("Type error in VariableDecl: variable " + identifiers[0] + " is of type integer, you gave something else.");
            }
        }
    }
    for (int i = 0; i < identifiers.size(); i++)
    {
        if (type != nullptr)
        {
            env.getCurrentScope()->addEntry(identifiers[i], type, nullptr, isMutable);
        }
        else
        {
            if (i < identifierTypes.size())
            {
                env.getCurrentScope()->addEntry(identifiers[i], identifierTypes[i], nullptr, isMutable);
            }
            else
            {
                env.getCurrentScope()->addEntry(identifiers[i], nullptr, nullptr, isMutable);
            }
        }
    }
}

void VariableDecl::setupFuncEnv(FunctionEnv &env)
{
    return;
}

void VariableDecl::setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::string> identifiers;
    idList->getIdentifierStrings(identifiers);

    std::vector<std::shared_ptr<Type>> identifierTypes;
    if (expList != nullptr)
    {
        expList->getTypes(env, funcEnv, identifierTypes);
    }
    for (int i = 0; i < identifiers.size(); i++)
    {
        if (type != nullptr)
        {
            env.getCurrentScope()->addEntry(identifiers[i], type, nullptr, isMutable);
        }
        else
        {
            if (i < identifierTypes.size())
            {
                env.getCurrentScope()->addEntry(identifiers[i], identifierTypes[i], nullptr, isMutable);
            }
            else
            {
                env.getCurrentScope()->addEntry(identifiers[i], nullptr, nullptr, isMutable);
            }
        }
    }
}
FunctionDecl::FunctionDecl(Identifier *funcName, FunctionSpecs *funcSpecs, Block *body) : funcName{funcName}, funcSpecs{funcSpecs}, funcBody{body} {}
FunctionDecl::FunctionDecl(std::shared_ptr<Identifier> funcName, std::shared_ptr<FunctionSpecs> funcSpecs, std::shared_ptr<Block> body) : funcName{funcName}, funcSpecs{funcSpecs}, funcBody{body} {}

void FunctionDecl::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    funcEnv.declaredFunctions.addEntry(funcName->name, std::make_shared<FunctionDecl>(funcName, funcSpecs, funcBody));
}

void FunctionDecl::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> idsAndTypes;
    if (funcSpecs->parameters != nullptr)
    {
        funcSpecs->parameters->getIdentifiersWithTypes(idsAndTypes);
    }
    funcEnv.pushFunc(funcName->name);
    env.pushScope();

    int valueIndex = 0;
    for (int i = 0; i < idsAndTypes.size(); i++)
    {
        for (int j = 0; j < idsAndTypes.at(i).first.size(); j++)
        {
            env.getCurrentScope()->addEntry(idsAndTypes.at(i).first.at(j), idsAndTypes.at(i).second, nullptr, NULL);
            valueIndex++;
        }
    }

    funcEnv.declaredFunctions.addEntry(funcName->name, std::make_shared<FunctionDecl>(funcName, funcSpecs, funcBody));
    funcBody->typeCheck(env, funcEnv, typeErrors);

    env.popScope();
    funcEnv.popFunc();
}

void FunctionDecl::setupFuncEnv(FunctionEnv &env)
{
    env.declaredFunctions.addEntry(funcName->name, std::make_shared<FunctionDecl>(funcName, funcSpecs, funcBody));
}

void FunctionDecl::setupGlobalEnv(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return;
}
