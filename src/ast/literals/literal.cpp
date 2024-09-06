#include "literal.h"
#include <iostream>

BoolLiteral::BoolLiteral(bool value) : value{value} {}

void BoolLiteral::negative()
{
    value = !value;
}

void BoolLiteral::printLiteral()
{
    if (value)
    {
        std::cout << "true" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }
}

IntLiteral::IntLiteral(int value) : value{value} {};

void IntLiteral::negative()

{
    value = -value;
}

void IntLiteral::printLiteral()
{
    std::cout << value << std::endl;
}
