#include "paramlist.h"
#include <iostream>

LastParamList::LastParamList(ParameterDecl *last) : last{last} {}

void LastParamList::getIdentifiersWithTypes(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> &identifiersWithType)
{
    std::vector<std::string> idOfIdentifiers;
    last->identifiers->getIdentifierStrings(idOfIdentifiers);
    identifiersWithType.push_back(std::pair<std::vector<std::string>, std::shared_ptr<Type>>(idOfIdentifiers, last->type));
}

void LastParamList::getIdentifiers(std::vector<std::string> &identifierList)
{
    last->identifiers->getIdentifierStrings(identifierList);
}

PairParamList::PairParamList(ParameterDecl *head, ParameterList *tail) : head{head}, tail{tail} {};

void PairParamList::getIdentifiersWithTypes(std::vector<std::pair<std::vector<std::string>, std::shared_ptr<Type>>> &identifiersWithType)
{
    std::vector<std::string> idOfIdentifiers;
    head->identifiers->getIdentifierStrings(idOfIdentifiers);
    identifiersWithType.push_back(std::pair<std::vector<std::string>, std::shared_ptr<Type>>(idOfIdentifiers, head->type));

    tail->getIdentifiersWithTypes(identifiersWithType);
}

void PairParamList::getIdentifiers(std::vector<std::string> &identifierList)
{
    head->identifiers->getIdentifierStrings(identifierList);
    tail->getIdentifiers(identifierList);
}
