#include "typelist.h"
#include <iostream>

LastTypeList::LastTypeList(Type *last) : last{last} {};

void LastTypeList::getTypes(std::vector<std::shared_ptr<Type>> &typesContainer)
{
    typesContainer.push_back(last);
}

PairTypeList::PairTypeList(Type *head, TypeList *tail) : head{head}, tail{tail} {};

void PairTypeList::getTypes(std::vector<std::shared_ptr<Type>> &typesContainer)
{
    typesContainer.push_back(head);
    tail->getTypes(typesContainer);
}
