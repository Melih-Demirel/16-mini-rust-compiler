#include "returntypes.h"
#include <iostream>

ReturnType::ReturnType(TypeList *types) : types{types}
{
}

void ReturnType::getTypes(std::vector<std::shared_ptr<Type>> &typesContainer)
{
    types->getTypes(typesContainer);
}
