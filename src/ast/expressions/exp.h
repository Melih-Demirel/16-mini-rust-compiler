#pragma once
#include <memory>
#include <utility>
#include <vector>
#include "./operand.h"
#include "./explist.h"

struct ScopedEnv;
struct FunctionEnv;
struct Operand;
struct ExpList;
struct Literal;

enum UnaryOperator
{
    PLUS_UNARY,
    MIN_UNARY,
    NOT_UNARY
};
enum BinaryOp
{
    EQ_BINARY,
    NE_BINARY,
    LT_BINARY,
    LE_BINARY,
    GT_BINARY,
    GE_BINARY,
    PLUS_BINARY,
    MIN_BINARY,
    MUL_BINARY,
    OR_BINARY,
    AND_BINARY,
    DIV_BINARY
};

struct Exp
{
    virtual std::shared_ptr<Literal> interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
    virtual std::shared_ptr<Type> typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) = 0;
    virtual std::string getOperandName() = 0;
    virtual void getReferenceNames(std::vector<std::string> &referenceNamesContainer) = 0;
    virtual std::vector<std::shared_ptr<Type>> getType(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
};

struct FuncCall : Exp
{
    std::shared_ptr<Exp> primaryExp;
    std::shared_ptr<ExpList> funcArgs;

    FuncCall(Exp *primaryExp, ExpList *expList);
    std::string getOperandName() override;
    std::shared_ptr<Literal> interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    std::shared_ptr<Type> typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    std::vector<std::shared_ptr<Type>> getType(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void getReferenceNames(std::vector<std::string> &referenceNamesContainer) override;
    std::vector<std::shared_ptr<Literal>> runFunc(ScopedEnv &env, FunctionEnv &funcEnv);
    std::vector<std::shared_ptr<Type>> checkFunc(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors);
};

struct OperandExp : Exp
{
    std::shared_ptr<Operand> operand;

    OperandExp(Operand *operand);
    std::string getOperandName() override;
    void getReferenceNames(std::vector<std::string> &referenceNamesContainer) override;
    std::shared_ptr<Literal> interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    std::shared_ptr<Type> typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    std::vector<std::shared_ptr<Type>> getType(ScopedEnv &env, FunctionEnv &funcEnv) override;
};
struct UnaryExp : Exp
{

    UnaryOperator op;
    std::shared_ptr<Exp> unaryExp;

    UnaryExp(Exp *unaryExp, UnaryOperator op);
    std::string getOperandName() override;
    void getReferenceNames(std::vector<std::string> &referenceNamesContainer) override;
    std::shared_ptr<Literal> interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    std::shared_ptr<Type> typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    std::vector<std::shared_ptr<Type>> getType(ScopedEnv &env, FunctionEnv &funcEnv) override;
};

struct BinaryExp : Exp
{

    std::shared_ptr<Exp> left;
    std::shared_ptr<Exp> right;
    BinaryOp op;

    BinaryExp(Exp *left, Exp *right, BinaryOp op);
    std::string getOperandName() override;
    void getReferenceNames(std::vector<std::string> &referenceNamesContainer) override;
    std::shared_ptr<Literal> interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    std::shared_ptr<Type> typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
    std::vector<std::shared_ptr<Type>> getType(ScopedEnv &env, FunctionEnv &funcEnv) override;
};