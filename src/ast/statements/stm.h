#pragma once
#include <memory>
#include <utility>
#include "../declarations/decl.h"
#include "../block/block.h"
#include "../expressions/exp.h"
#include "../expressions/explist.h"

struct FunctionEnv;
struct ScopedEnv;
struct ForStm;
struct Block;
struct Exp;
struct Decl;
struct ExpList;
struct Decl;

enum AssignOp
{
    PLUS_ASSIGN,
    _ASSIGN,
    MIN_ASSIGN,
    MUL_ASSIGN,
    DIV_ASSIGN
};

struct Stm
{
    virtual void interpret(ScopedEnv &env, FunctionEnv &funcEnv) = 0;
    virtual void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) = 0;
};

struct DeclarationStm : Stm
{
    std::shared_ptr<Decl> declaration;

    DeclarationStm(Decl *decl);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct BlockStm : Stm
{
    std::shared_ptr<Block> block;

    BlockStm(Block *block);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct IfStm : Stm
{
    std::shared_ptr<Block> ifBlock;
    std::shared_ptr<Exp> condition;
    std::shared_ptr<Block> elseBlock;
    std::shared_ptr<Stm> elseIfStm;

    IfStm(Block *ifBlock, Exp *cond, Block *elseBlock, Stm *elseIfStm);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct AssignmentStm : Stm
{
    std::shared_ptr<ExpList> leftExpList;
    std::shared_ptr<ExpList> rightExpList;
    AssignOp assignOp;

    AssignmentStm(ExpList *left, ExpList *right, AssignOp assign_op);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct ExpressionStm : Stm
{
    std::shared_ptr<Exp> exp;

    ExpressionStm(Exp *exp);
    std::string getIdentifierName() const;
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct PrintStm : Stm
{
    std::shared_ptr<ExpList> expressions;

    PrintStm(ExpList *expList);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct PrintLnStm : Stm
{
    std::shared_ptr<ExpList> expressions;

    PrintLnStm(ExpList *expList);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct BreakStm : Stm
{
    BreakStm();
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct WhileStm : Stm
{
    std::shared_ptr<Exp> condition;
    std::shared_ptr<Block> body;

    WhileStm(Exp *cond, Block *body);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct ForStm : Stm
{
    std::shared_ptr<Identifier> identifier;
    std::shared_ptr<Literal> lowerB;
    std::shared_ptr<Literal> upperB;
    bool isMutable;
    std::shared_ptr<Block> body;

    ForStm(Identifier *identifier, Literal *lowerB, Literal *upperB, bool isMutable, Block *body);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct LoopStm : Stm
{
    std::shared_ptr<Block> body;

    LoopStm(Block *body);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};

struct ReturnStm : Stm
{
    std::shared_ptr<ExpList> expressionList;

    ReturnStm(ExpList *expList);
    void interpret(ScopedEnv &env, FunctionEnv &funcEnv) override;
    void typeCheck(ScopedEnv &env, FunctionEnv &funcEnv, std::vector<std::string> &typeErrors) override;
};