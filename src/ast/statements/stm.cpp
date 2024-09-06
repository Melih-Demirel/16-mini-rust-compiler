#include "stm.h"
#include "../../environment/env.h"
#include <algorithm>
#include <iostream>

DeclarationStm::DeclarationStm(Decl *decl) : declaration{decl} {}

void DeclarationStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    declaration->interpret(env, funcEnv);
}

void DeclarationStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    declaration->typeCheck(env, funcEnv, typeErrors);
}

BlockStm::BlockStm(Block *block) : block{block} {}

void BlockStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    env.pushScope();
    block->interpret(env, funcEnv);
    env.popScope();
}

void BlockStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    env.pushScope();
    block->typeCheck(env, funcEnv, typeErrors);
    env.popScope();
}

IfStm::IfStm(Block *ifBlock, Exp *cond, Block *elseBlock, Stm *elseIfStm) : ifBlock{ifBlock}, condition{cond}, elseBlock{elseBlock}, elseIfStm{elseIfStm} {}

void IfStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    env.pushScope();
    if (std::dynamic_pointer_cast<BoolLiteral>(condition->interpret(env, funcEnv))->value)
    {
        ifBlock->interpret(env, funcEnv);
    }
    else
    {
        if (elseBlock != nullptr)
        {
            elseBlock->interpret(env, funcEnv);
        }
        if (elseIfStm != nullptr)
        {
            elseIfStm->interpret(env, funcEnv);
        }
    }
    env.popScope();
}

void IfStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    env.pushScope();
    if (std::dynamic_pointer_cast<BooleanType>(condition->typeCheck(env, funcEnv, typeErrors)) == nullptr)
    {
        typeErrors.push_back("Type error in IfStm: Condition needs to be a boolean type!");
    }

    ifBlock->typeCheck(env, funcEnv, typeErrors);
    if (elseBlock != nullptr)
    {
        elseBlock->typeCheck(env, funcEnv, typeErrors);
    }
    if (elseIfStm != nullptr)
    {
        elseIfStm->typeCheck(env, funcEnv, typeErrors);
    }

    env.popScope();
}

WhileStm::WhileStm(Exp *cond, Block *body) : condition{cond}, body{body} {}

void WhileStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    env.pushScope();
    while (std::dynamic_pointer_cast<BoolLiteral>(condition->interpret(env, funcEnv))->value)
    {
        funcEnv.clearBreakFlag();
        if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
        {
            if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
                return;
        }
        body->interpret(env, funcEnv);
        if (funcEnv.hasBreakFlag())
        {
            break;
        }
    }
    env.popScope();
}

void WhileStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    env.pushScope();
    if (std::dynamic_pointer_cast<BooleanType>(condition->typeCheck(env, funcEnv, typeErrors)) == nullptr)
    {
        typeErrors.push_back("Type error in WhileStm: Condition needs to be a boolean type!");
    }
    body->typeCheck(env, funcEnv, typeErrors);
    env.popScope();
}

ForStm::ForStm(Identifier *identifier, Literal *lower, Literal *upper, bool isMutable, Block *body) : identifier(identifier), lowerB(lower), upperB(upper), isMutable(isMutable), body{body} {}

void ForStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::string loopVarName = identifier->name;
    int lower = std::dynamic_pointer_cast<IntLiteral>(lowerB)->value;
    int upper = std::dynamic_pointer_cast<IntLiteral>(upperB)->value;
    env.pushScope();

    std::shared_ptr<Type> type = std::make_shared<IntegerType>();
    std::shared_ptr<Literal> value = std::make_shared<IntLiteral>(lower);
    env.getCurrentScope()->addEntry(loopVarName, type, value, isMutable);
    for (int i = lower; i < upper; ++i)
    {
        env.clearBreakFlag();
        funcEnv.clearBreakFlag();
        value = std::make_shared<IntLiteral>(i);
        env.getCurrentScope()->updateEntry(loopVarName, value);

        if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
        {
            if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
                return;
        }

        body->interpret(env, funcEnv);
        if (funcEnv.hasBreakFlag())
        {
            break;
        }
        if (env.hasBreakFlag())
        {
            break;
        }
    }

    env.popScope();
}

void ForStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    env.pushScope();

    std::string loopVarName = identifier->name;

    env.getCurrentScope()->addEntry(loopVarName, std::make_shared<IntegerType>(), std::make_shared<IntLiteral>(0), isMutable);
    body->typeCheck(env, funcEnv, typeErrors);

    env.popScope();
}

LoopStm::LoopStm(Block *body) : body{body}
{
}

void LoopStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    env.pushScope();
    while (true)
    {
        env.clearBreakFlag();
        funcEnv.clearBreakFlag();
        if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
        {
            if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
                return;
        }

        body->interpret(env, funcEnv);
        if (funcEnv.hasBreakFlag())
        {
            break;
        }
        if (env.hasBreakFlag())
        {
            break;
        }
    }

    env.popScope();
}

void LoopStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    env.pushScope();
    body->typeCheck(env, funcEnv, typeErrors);
    env.popScope();
}

BreakStm::BreakStm() {};
void BreakStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    funcEnv.setBreakFlag(true);
    env.setBreakFlag(true);
};
void BreakStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) {};

AssignmentStm::AssignmentStm(ExpList *left, ExpList *right, AssignOp assign_op) : leftExpList{left}, rightExpList{right}, assignOp{assign_op} {}

void AssignmentStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    std::vector<std::shared_ptr<Literal>> newValues;
    rightExpList->interpret(env, funcEnv, newValues);

    std::vector<std::string> operandNames;
    leftExpList->getOperandNames(operandNames);

    if (newValues.size() == 1)
    {
        for (std::string name : operandNames)
        {
            switch (assignOp)
            {
            case _ASSIGN:
                env.updateVariable(name, newValues[0]);
                break;
            case PLUS_ASSIGN:
                std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(name))->value += std::dynamic_pointer_cast<IntLiteral>(newValues[0])->value;
                break;
            case MIN_ASSIGN:
                std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(name))->value -= std::dynamic_pointer_cast<IntLiteral>(newValues[0])->value;
                break;
            case MUL_ASSIGN:
                std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(name))->value *= std::dynamic_pointer_cast<IntLiteral>(newValues[0])->value;
                break;
            case DIV_ASSIGN:
                std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(name))->value /= std::dynamic_pointer_cast<IntLiteral>(newValues[0])->value;
                break;
            default:
                break;
            }
        }
    }
    else
    {
        if (operandNames.size() == newValues.size())
        {
            for (int i = 0; i < operandNames.size(); i++)
            {
                switch (assignOp)
                {
                case _ASSIGN:
                    env.updateVariable(operandNames[i], newValues[i]);
                    break;
                case PLUS_ASSIGN:
                    std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(operandNames[i]))->value += std::dynamic_pointer_cast<IntLiteral>(newValues[i])->value;
                    break;
                case MIN_ASSIGN:
                    std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(operandNames[i]))->value -= std::dynamic_pointer_cast<IntLiteral>(newValues[i])->value;
                    break;
                case MUL_ASSIGN:
                    std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(operandNames[i]))->value *= std::dynamic_pointer_cast<IntLiteral>(newValues[i])->value;
                    break;
                case DIV_ASSIGN:
                    std::dynamic_pointer_cast<IntLiteral>(env.lookupVariable(operandNames[i]))->value /= std::dynamic_pointer_cast<IntLiteral>(newValues[i])->value;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void AssignmentStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    std::vector<std::shared_ptr<Type>> rightHandTypes;
    rightExpList->typeCheck(env, funcEnv, rightHandTypes, typeErrors);

    std::vector<std::string> leftHandOperandNames;
    leftExpList->getOperandNames(leftHandOperandNames);
    for (auto name : leftHandOperandNames)
    {
        std::pair<int, bool> variableExists = env.variableExists(name);
        if (variableExists.first == 0)
        {
            typeErrors.push_back("Type error in AssignmentStm: variable " + name + " was not defined.");
        }
        else
        {
            if (!env.isVarMutable(name))
            {
                typeErrors.push_back("Type error in AssignmentStm: variable " + name + " is not mutable and cannot be assigned to.");
            }
        }
    }

    switch (assignOp)
    {
    case _ASSIGN:
    {
        if (rightHandTypes.size() != leftHandOperandNames.size())
        {
            typeErrors.push_back("Type error in AssignmentStm: Amount of left hand operands is not equal to number of right hand operands.");
        }

        int typeCounter = 0;
        for (auto name : leftHandOperandNames)
        {
            if (std::dynamic_pointer_cast<BooleanType>(env.getVarType(name)) != nullptr && std::dynamic_pointer_cast<BooleanType>(rightHandTypes[typeCounter]) == nullptr)
            {
                typeErrors.push_back("Type error in AssignmentStm: Trying to assign another type to variable " + name + " that is a bool.");
            }

            if (std::dynamic_pointer_cast<IntegerType>(env.getVarType(name)) != nullptr && std::dynamic_pointer_cast<IntegerType>(rightHandTypes[typeCounter]) == nullptr)
            {
                typeErrors.push_back("Type error in AssignmentStm: Trying to assign another type to variable " + name + " that is an int.");
            }

            typeCounter++;
        }
        break;
    }
    case PLUS_ASSIGN:
    {

        if (rightHandTypes.size() != 1 || leftHandOperandNames.size() != 1)
        {
            typeErrors.push_back("Type error in AssignmentStm: Only 1 operand on both sides is allowed.");
        }

        if (std::dynamic_pointer_cast<IntegerType>(env.getVarType(leftHandOperandNames[0])) == nullptr || std::dynamic_pointer_cast<IntegerType>(rightHandTypes[0]) == nullptr)
        {
            typeErrors.push_back("Type error in AssignmentStm: One of the sides is not an integer.");
        }

        break;
    }

    case MIN_ASSIGN:
    {
        if (rightHandTypes.size() != 1 || leftHandOperandNames.size() != 1)
        {
            typeErrors.push_back("Type error in AssignmentStm: Only 1 operand on both sides is allowed.");
        }

        if (std::dynamic_pointer_cast<IntegerType>(env.getVarType(leftHandOperandNames[0])) == nullptr || std::dynamic_pointer_cast<IntegerType>(rightHandTypes[0]) == nullptr)
        {
            typeErrors.push_back("Type error in AssignmentStm: One of the sides is not an integer.");
        }
        break;
    }
    case MUL_ASSIGN:
    {
        if (rightHandTypes.size() != 1 || leftHandOperandNames.size() != 1)
        {
            typeErrors.push_back("Type error in AssignmentStm: Only 1 operand on both sides is allowed.");
        }

        if (std::dynamic_pointer_cast<IntegerType>(env.getVarType(leftHandOperandNames[0])) == nullptr || std::dynamic_pointer_cast<IntegerType>(rightHandTypes[0]) == nullptr)
        {
            typeErrors.push_back("Type error in AssignmentStm: One of the sides is not an integer.");
        }
        break;
    }

    case DIV_ASSIGN:
    {
        if (rightHandTypes.size() != 1 || leftHandOperandNames.size() != 1)
        {
            typeErrors.push_back("Type error in AssignmentStm: Only 1 operand on both sides is allowed.");
        }

        if (std::dynamic_pointer_cast<IntegerType>(env.getVarType(leftHandOperandNames[0])) == nullptr || std::dynamic_pointer_cast<IntegerType>(rightHandTypes[0]) == nullptr)
        {
            typeErrors.push_back("Type error in AssignmentStm: One of the sides is not an integer.");
        }
        break;
    }
    default:
    {
        typeErrors.push_back("Type error in AssignmentStm: Operator not known.");
        break;
    }
    }
}

ExpressionStm::ExpressionStm(Exp *exp) : exp{exp} {}

void ExpressionStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    exp->interpret(env, funcEnv);
}

void ExpressionStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    exp->typeCheck(env, funcEnv, typeErrors);
}

std::string ExpressionStm::getIdentifierName() const
{
    if (auto operandExp = std::dynamic_pointer_cast<OperandExp>(exp))
    {
        if (auto varOperand = std::dynamic_pointer_cast<VariableOperand>(operandExp->operand))
            if (auto varOperand = std::dynamic_pointer_cast<VariableOperand>(operandExp->operand))
            {
                return varOperand->operandName->name;
            }
    }
}

PrintStm::PrintStm(ExpList *expList) : expressions{expList} {}

void PrintStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;
    if (expressions != nullptr)
    {
        std::vector<std::shared_ptr<Literal>> values;
        expressions->interpret(env, funcEnv, values);

        for (std::shared_ptr<Literal> v : values)
        {
            if (std::dynamic_pointer_cast<BoolLiteral>(v) != nullptr)
            {
                if (std::dynamic_pointer_cast<BoolLiteral>(v)->value)
                {
                    std::cout << "true ";
                }
                else
                {
                    std::cout << "false ";
                }
            }

            if (std::dynamic_pointer_cast<IntLiteral>(v) != nullptr)
            {
                std::cout << std::dynamic_pointer_cast<IntLiteral>(v)->value << " ";
            }
        }
    }
}

void PrintStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (expressions != nullptr)
    {
        std::vector<std::shared_ptr<Type>> types;
        expressions->typeCheck(env, funcEnv, types, typeErrors);
    }
}

PrintLnStm::PrintLnStm(ExpList *expList) : expressions{expList} {}

void PrintLnStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }

    if (env.breakFlag)
        return;
    if (expressions != nullptr)
    {
        std::vector<std::shared_ptr<Literal>> values;
        expressions->interpret(env, funcEnv, values);

        for (std::shared_ptr<Literal> v : values)
        {
            if (std::dynamic_pointer_cast<BoolLiteral>(v) != nullptr)
            {
                if (std::dynamic_pointer_cast<BoolLiteral>(v)->value)
                {
                    std::cout << "true ";
                }
                else
                {
                    std::cout << "false ";
                }
            }

            if (std::dynamic_pointer_cast<IntLiteral>(v) != nullptr)
            {
                std::cout << std::dynamic_pointer_cast<IntLiteral>(v)->value << " ";
            }
        }
    }

    std::cout << std::endl;
}

void PrintLnStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (expressions != nullptr)
    {
        std::vector<std::shared_ptr<Type>> types;
        expressions->typeCheck(env, funcEnv, types, typeErrors);
    }
}
ReturnStm::ReturnStm(ExpList *expList) : expressionList{expList} {}

void ReturnStm::interpret(ScopedEnv &env, FunctionEnv &funcEnv)
{
    std::vector<std::shared_ptr<Literal>> returnValues;

    std::string funcName = funcEnv.currentFunc();
    std::shared_ptr<FunctionTableEntry> funcSpecs = funcEnv.lookupVariable(funcName);

    if (funcEnv.lookupVariable(funcEnv.currentFunc()) != nullptr)
    {
        if (funcEnv.lookupVariable(funcEnv.currentFunc())->didReturn)
            return;
    }
    if (env.breakFlag)
        return;

    funcEnv.lookupVariable(funcName)->didReturn = true;

    if (expressionList != nullptr)
    {
        expressionList->interpret(env, funcEnv, returnValues);

        std::reverse(returnValues.begin(), returnValues.end());
        funcEnv.declaredFunctions.addReturnValues(funcName, returnValues);
    }
}

void ReturnStm::typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors)
{
    if (expressionList != nullptr)
    {
        std::vector<std::shared_ptr<Type>> expTypes;
        expressionList->typeCheck(env, funcEnv, expTypes, typeErrors);
    }
}
