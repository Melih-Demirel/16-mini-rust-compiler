#include "exp.h"
#include "../../environment/env.h"
#include <iostream>
#include <algorithm>

UnaryExp::UnaryExp(Exp *unaryExp, UnaryOperator op) : unaryExp{unaryExp}, op{op} {}

std::string UnaryExp::getOperandName()
{
    return std::string("");
}

void UnaryExp::getReferenceNames(std::vector<std::string> &refContainer)
{
    unaryExp->getReferenceNames(refContainer);
}

std::shared_ptr<Literal> UnaryExp::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::shared_ptr<Literal> literalResult = unaryExp->interpret(env, funcEnv);

    if (std::dynamic_pointer_cast<BoolLiteral>(literalResult) != nullptr)
    {
        switch (op)
        {
        case NOT_UNARY:
            std::dynamic_pointer_cast<BoolLiteral>(literalResult)->negative();
            break;
        default:
            break;
        }
    }

    if (std::dynamic_pointer_cast<IntLiteral>(literalResult) != nullptr)
    {
        switch (op)
        {
        case PLUS_UNARY:
            break;
        case MIN_UNARY:
            std::dynamic_pointer_cast<IntLiteral>(literalResult)->negative();
            break;
        default:
            break;
        }
    }
    return literalResult;
}

std::shared_ptr<Type> UnaryExp::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (std::dynamic_pointer_cast<FuncCall>(unaryExp) != nullptr)
    {
        return nullptr;
    }
    std::shared_ptr<Type> resultType = unaryExp->typeCheck(env, funcEnv, typeErrors);
    if (std::dynamic_pointer_cast<BooleanType>(resultType) != nullptr)
    {
        switch (op)
        {
        case NOT_UNARY:
            break;
        default:
            typeErrors.push_back("Type error in UnaryExp: Cant use that on boolean unary expression!");
            break;
        }
    }

    if (std::dynamic_pointer_cast<IntegerType>(resultType) != nullptr)
    {
        switch (op)
        {
        case PLUS_UNARY:
            break;
        case MIN_UNARY:
            break;
        default:
            typeErrors.push_back("Type error in UnaryExp: Cant use that on integer unary expression!");
            break;
        }
    }

    return resultType;
}

std::vector<std::shared_ptr<Type>> UnaryExp::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return unaryExp->getType(env, funcEnv);
}

FuncCall::FuncCall(Exp *primaryExp, ExpList *funcArgs) : primaryExp{primaryExp}, funcArgs{funcArgs} {}

std::vector<std::shared_ptr<Literal>> FuncCall::runFunc(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::string funcName = primaryExp->getOperandName();

    funcEnv.pushFunc(funcName);

    funcEnv.lookupVariable(funcName)->didReturn = false;

    std::vector<std::shared_ptr<Literal>> argValues;
    if (funcArgs != nullptr)
    {
        funcArgs->interpret(env, funcEnv, argValues);
    }

    env.pushScope();

    std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> argNamesAndTypes;
    if (funcEnv.lookupVariable(funcName)->funcDecl->funcSpecs->parameters != nullptr)
    {
        funcEnv.lookupVariable(funcName)->funcDecl->funcSpecs->parameters->getIdentifiersWithTypes(argNamesAndTypes);
    }

    std::reverse(argNamesAndTypes.begin(), argNamesAndTypes.end());
    int valueIndex = 0;
    valueIndex = 0;
    for (int i = 0; i < argNamesAndTypes.size(); i++)
    {
        for (int j = 0; j < argNamesAndTypes.at(i).first.size(); j++)
        {
            env.getCurrentScope()->addEntry(argNamesAndTypes.at(i).first.at(j), argNamesAndTypes.at(i).second, argValues[valueIndex], true);
            valueIndex++;
        }
    }

    funcEnv.lookupVariable(funcName)->funcDecl->funcBody->interpret(env, funcEnv);

    funcEnv.popFunc();
    env.popScope();

    std::vector<std::shared_ptr<Literal>> returnVals = funcEnv.lookupVariable(funcName)->returnValues;
    funcEnv.declaredFunctions.resetReturnValues(funcName);
    return returnVals;
};

std::vector<std::shared_ptr<Type>> FuncCall::checkFunc(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{

    std::string funcName = primaryExp->getOperandName();

    std::shared_ptr<FunctionTableEntry> functionEntry = funcEnv.lookupVariable(funcName);

    if (functionEntry == nullptr)
    {
        typeErrors.push_back("Trying to call function that doesnt exist: " + funcName);
        std::vector<std::shared_ptr<Type>> emptyReturnTypes{};
        return emptyReturnTypes;
    }

    funcEnv.pushFunc(funcName);

    std::vector<std::shared_ptr<Type>> argTypes;
    if (funcArgs != nullptr)
    {
        funcArgs->typeCheck(env, funcEnv, argTypes, typeErrors);
    }

    std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> idsAndTypes;
    if (funcEnv.lookupVariable(funcName)->funcDecl->funcSpecs->parameters != nullptr)
    {
        functionEntry->funcDecl->funcSpecs->parameters->getIdentifiersWithTypes(idsAndTypes);
    }
    funcEnv.popFunc();

    if (funcEnv.lookupVariable(funcName)->funcDecl->funcSpecs->returnTypes == nullptr)
    {
        std::vector<std::shared_ptr<Type>> emptyReturnTypes{};
        return emptyReturnTypes;
    }
    else
    {
        std::vector<std::shared_ptr<Type>> returnTypes{};
        funcEnv.lookupVariable(funcName)->funcDecl->funcSpecs->returnTypes->getTypes(returnTypes);
        return returnTypes;
    }
}

std::shared_ptr<Type> FuncCall::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    std::vector<std::shared_ptr<Type>> resultType = this->checkFunc(env, funcEnv, typeErrors);
    if (resultType.size() > 0)
    {
        return resultType[0];
    }
    return nullptr;
}

std::shared_ptr<Literal> FuncCall::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::shared_ptr<Literal>> resultLiteral = this->runFunc(env, funcEnv);
    if (resultLiteral.size() > 0)
    {
        return resultLiteral[0];
    }
    return nullptr;
}

std::string FuncCall::getOperandName()
{
    return std::string("");
}

void FuncCall::getReferenceNames(std::vector<std::string> &refContainer)
{
    refContainer.push_back("");
}

std::vector<std::shared_ptr<Type>> FuncCall::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::string funcName = primaryExp->getOperandName();

    std::shared_ptr<FunctionTableEntry> functionEntry = funcEnv.lookupVariable(funcName);

    if (functionEntry == nullptr)
    {
        std::vector<std::shared_ptr<Type>> emptyReturnTypes{};
        return emptyReturnTypes;
    }

    if (functionEntry->funcDecl->funcSpecs->returnTypes == nullptr)
    {
        std::vector<std::shared_ptr<Type>> emptyReturnTypes{};
        return emptyReturnTypes;
    }
    else
    {
        std::vector<std::shared_ptr<Type>> returnTypes{};
        functionEntry->funcDecl->funcSpecs->returnTypes->getTypes(returnTypes);
        return returnTypes;
    }
}

OperandExp::OperandExp(Operand *operand) : operand{operand} {}

std::shared_ptr<Literal> OperandExp::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return operand->interpret(env, funcEnv);
}

std::string OperandExp::getOperandName()
{
    if (std::dynamic_pointer_cast<VariableOperand>(operand) != nullptr)
        return std::dynamic_pointer_cast<VariableOperand>(operand)->operandName->name;
    return std::string("");
}

void OperandExp::getReferenceNames(std::vector<std::string> &refContainer)
{
    if (std::dynamic_pointer_cast<VariableOperand>(operand) != nullptr)
        refContainer.push_back(std::dynamic_pointer_cast<VariableOperand>(operand)->operandName->name);
}

std::shared_ptr<Type> OperandExp::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    return operand->typeCheck(env, funcEnv, typeErrors);
}

std::vector<std::shared_ptr<Type>> OperandExp::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return operand->getType(env, funcEnv);
}

BinaryExp::BinaryExp(Exp *left, Exp *right, BinaryOp op) : left{left}, right{right}, op{op} {}

std::string BinaryExp::getOperandName()
{
    return std::string("");
}

void BinaryExp::getReferenceNames(std::vector<std::string> &refContainer)
{
    left->getReferenceNames(refContainer);
    right->getReferenceNames(refContainer);
}

std::shared_ptr<Literal> BinaryExp::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    switch (op)
    {
    case EQ_BINARY:
        if (std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv)) != nullptr)
        {
            return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv))->value == std::dynamic_pointer_cast<BoolLiteral>(right->interpret(env, funcEnv))->value);
        }
        if (std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv)) != nullptr)
        {
            return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value == std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        }
        break;

    case NE_BINARY:
        if (std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv)) != nullptr)
        {
            return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv))->value != std::dynamic_pointer_cast<BoolLiteral>(right->interpret(env, funcEnv))->value);
        }
        if (std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv)) != nullptr)
        {
            return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value != std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        }
        break;

    case LT_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value < std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case LE_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value <= std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case GT_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value > std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case GE_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value >= std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case OR_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv))->value || std::dynamic_pointer_cast<BoolLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case AND_BINARY:
        return std::make_shared<BoolLiteral>(std::dynamic_pointer_cast<BoolLiteral>(left->interpret(env, funcEnv))->value && std::dynamic_pointer_cast<BoolLiteral>(right->interpret(env, funcEnv))->value);
        break;

    case MUL_BINARY:
        return std::make_shared<IntLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value * std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case DIV_BINARY:
        return std::make_shared<IntLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value / std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case PLUS_BINARY:
        return std::make_shared<IntLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value + std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;
    case MIN_BINARY:
        return std::make_shared<IntLiteral>(std::dynamic_pointer_cast<IntLiteral>(left->interpret(env, funcEnv))->value - std::dynamic_pointer_cast<IntLiteral>(right->interpret(env, funcEnv))->value);
        break;

    default:
        break;
    }
    return nullptr;
}

std::shared_ptr<Type> BinaryExp::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (std::dynamic_pointer_cast<FuncCall>(right) != nullptr)
    {

        std::vector<std::shared_ptr<Type>> returnTypes = std::dynamic_pointer_cast<FuncCall>(right)->checkFunc(env, funcEnv, typeErrors);
        if (returnTypes.size() > 1 || returnTypes.size() == 0)
        {
            typeErrors.push_back("Type error in BinaryExp: Multiple/zero value used in single value.");
            return nullptr;
        }
    }
    if (std::dynamic_pointer_cast<FuncCall>(left) != nullptr)
    {

        std::vector<std::shared_ptr<Type>> returnTypes = std::dynamic_pointer_cast<FuncCall>(left)->checkFunc(env, funcEnv, typeErrors);
        if (returnTypes.size() > 1 || returnTypes.size() == 0)
        {
            typeErrors.push_back("Type error in BinaryExp: Multiple/zero value used in single value.");
            return nullptr;
        }
    }

    switch (op)
    {
    case EQ_BINARY:
        if ((std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr) ||
            (std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr))
        {
            typeErrors.push_back("Type error in BinExp: Both should be booleans.");
        }

        if ((std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr) ||
            (std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr))
        {
            typeErrors.push_back("Type error in BinExp: Both should be integers.");
        }
        return std::make_shared<BooleanType>();
        break;

    case NE_BINARY:
        if ((std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr) ||
            (std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr))
        {
            typeErrors.push_back("Type error: Not Equal operation only for booleans.");
        }
        if ((std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr) ||
            (std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) != nullptr && std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr))
        {
            typeErrors.push_back("Type error: Not Equal operation only for integers.");
        }
        return std::make_shared<BooleanType>();
        break;

    case LT_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error : Less Than only for booleans.");
        }
        return std::make_shared<BooleanType>();
        break;
    case LE_BINARY:
        if ((std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr))
        {
            typeErrors.push_back("Type error: Divide operation only for integers.");
        }
        return std::make_shared<BooleanType>();
        break;
    case GT_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Greather Than operation only for booleans.");
        }
        return std::make_shared<BooleanType>();
        break;
    case GE_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Greater Equal operation only for booleans.");
        }
        return std::make_shared<BooleanType>();
        break;
    case OR_BINARY:
        if (std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: OR operation only for booleans.");
        }
        return std::make_shared<BooleanType>();
        break;
    case AND_BINARY:
        if (std::dynamic_pointer_cast<BooleanType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<BooleanType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: AND operation only for booleans.");
        }
        return std::make_shared<BooleanType>();
        break;

    case PLUS_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Add operation only for integers.");
        }
        return std::make_shared<IntegerType>();
        break;
    case MIN_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Minus operation only for integers.");
        }
        return std::make_shared<IntegerType>();
        break;
    case MUL_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Multiply operation only for integers.");
        }
        return std::make_shared<IntegerType>();
        break;
    case DIV_BINARY:
        if (std::dynamic_pointer_cast<IntegerType>(left->typeCheck(env, funcEnv, typeErrors)) == nullptr || std::dynamic_pointer_cast<IntegerType>(right->typeCheck(env, funcEnv, typeErrors)) == nullptr)
        {
            typeErrors.push_back("Type error: Divide operation only for integers.");
        }
        return std::make_shared<IntegerType>();
        break;

    default:
        typeErrors.push_back("Type error: Operator not known.");
        return nullptr;
        break;
    }
}

std::vector<std::shared_ptr<Type>> BinaryExp::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::shared_ptr<Type>> resultType{};
    switch (op)
    {
    case EQ_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case NE_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case LT_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case LE_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case GT_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case GE_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case OR_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case AND_BINARY:
        resultType.push_back(std::make_shared<BooleanType>());
        break;
    case MUL_BINARY:
        resultType.push_back(std::make_shared<IntegerType>());
        break;
    case DIV_BINARY:
        resultType.push_back(std::make_shared<IntegerType>());
        break;
    case PLUS_BINARY:
        resultType.push_back(std::make_shared<IntegerType>());
        break;
    case MIN_BINARY:
        resultType.push_back(std::make_shared<IntegerType>());
        break;
    default:
        break;
    }
    return resultType;
}
