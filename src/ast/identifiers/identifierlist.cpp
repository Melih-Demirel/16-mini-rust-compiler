#include "identifierlist.h"
#include <iostream>
#include <algorithm>

LastIdentifierList::LastIdentifierList(Identifier *last) : last{last} {}

void LastIdentifierList::getIdentifierStrings(std::vector<std::string> &identifiersContainer)
{
    identifiersContainer.push_back(std::string(last->name));
}

PairIdentifierList::PairIdentifierList(Identifier *head, IdentifierList *tail) : head{head}, tail{tail} {}

void PairIdentifierList::getIdentifierStrings(std::vector<std::string> &identifiersContainer)
{
    identifiersContainer.push_back(head->name);
    tail->getIdentifierStrings(identifiersContainer);
}
