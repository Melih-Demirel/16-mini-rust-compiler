#include "./functiontable.h"
#include <iostream>

FunctionTableEntry::FunctionTableEntry(std::shared_ptr<FunctionDecl> function) : funcDecl{function}
{
    count = 1;
    didReturn = false;
}

FunctionTable::FunctionTable()
{
    tableEntries = {};
}

void FunctionTable::addEntry(std::string id, std::shared_ptr<FunctionDecl> function)
{
    std::pair<std::map<std::string, std::shared_ptr<FunctionTableEntry>>::iterator, bool> newEntryPair;
    newEntryPair = tableEntries.insert(std::pair<std::string, std::shared_ptr<FunctionTableEntry>>(id, std::make_shared<FunctionTableEntry>(function)));
    if (!newEntryPair.second)
    {
        tableEntries.at(id)->count++;
    }
}

void FunctionTable::addReturnValues(std::string id, std::vector<std::shared_ptr<Literal>> values)
{
    tableEntries.at(id)->returnValues = values;
}

void FunctionTable::resetReturnValues(std::string id)
{
    tableEntries.at(id)->returnValues.clear();
}

void FunctionTable::getReturnTypes(std::string id, std::vector<std::shared_ptr<Type>> &typesContainer)
{
    for (auto const &[key, value] : tableEntries)
    {
        if (key == id)
        {
            if (value->funcDecl->funcSpecs->returnTypes != nullptr)
            {
                std::vector<std::shared_ptr<Type>> returnTypes{};
                value->funcDecl->funcSpecs->returnTypes->getTypes(typesContainer);
            }
            else
            {
                return;
            }
        }
    }
}
