#pragma once
#include <utility>

struct ScopedEnv;
struct FunctionEnv;

struct Literal
{
    virtual void negative() = 0;
    virtual void printLiteral() = 0;
    virtual ~Literal() = default;
};

struct IntLiteral : Literal
{
    int value;

    IntLiteral(int value);

    void negative() override;
    void printLiteral() override;
};

struct BoolLiteral : Literal
{
    bool value;

    BoolLiteral(bool value);

    void negative() override;
    void printLiteral() override;
};
