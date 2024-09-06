#include "operand.h"
#include "../../environment/env.h"

LiteralOperand::LiteralOperand(Literal *lit) : literal{lit} {}

std::shared_ptr<Literal> LiteralOperand::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return literal;
}

std::shared_ptr<Type> LiteralOperand::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (std::dynamic_pointer_cast<IntLiteral>(literal) != nullptr)
    {
        return std::make_shared<IntegerType>();
    }
    if (std::dynamic_pointer_cast<BoolLiteral>(literal) != nullptr)
    {
        return std::make_shared<BooleanType>();
    }
}

std::vector<std::shared_ptr<Type>> LiteralOperand::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::shared_ptr<Type>> result{};
    if (std::dynamic_pointer_cast<IntLiteral>(literal) != nullptr)
    {
        result.push_back(std::make_shared<IntegerType>());
        return result;
    }
    if (std::dynamic_pointer_cast<BoolLiteral>(literal) != nullptr)
    {
        result.push_back(std::make_shared<BooleanType>());
        return result;
    }
    return result;
}

VariableOperand::VariableOperand(Identifier *operandName) : operandName{operandName} {}

std::shared_ptr<Literal> VariableOperand::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return env.lookupVariable(operandName->name);
}

std::shared_ptr<Type> VariableOperand::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{

    std::pair<int, bool> variableExists = env.variableExists(operandName->name);
    if (variableExists.first == 0)
    {
        typeErrors.push_back("Type error: Variable " + operandName->name + " is not defined.");
    }

    return env.getVarType(operandName->name);
}

std::vector<std::shared_ptr<Type>> VariableOperand::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::shared_ptr<Type>> result;
    std::pair<int, bool> variableExists = env.variableExists(operandName->name);
    if (variableExists.first == 0)
    {
        return result;
    }
    result.push_back(env.getVarType(operandName->name));
    return result;
}

ExprOperand::ExprOperand(Exp *exp) : exp{exp} {};

std::shared_ptr<Literal> ExprOperand::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return exp->interpret(env, funcEnv);
}

std::shared_ptr<Type> ExprOperand::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    return exp->typeCheck(env, funcEnv, typeErrors);
}

std::vector<std::shared_ptr<Type>> ExprOperand::getType(ScopedEnv &env, FunctionEnv &funcEnv)
{
    return exp->getType(env, funcEnv);
}