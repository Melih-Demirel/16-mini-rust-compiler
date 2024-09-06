#include "./symboltable.h"
#include <iostream>

TableEntry::TableEntry(std::shared_ptr<Type> type, std::shared_ptr<Literal> value, bool mutableFlag) : type{type}, value{value}, isMutable(mutableFlag)
{
    count = 1;
}

void SymbolTable::addEntry(std::string id, std::shared_ptr<Type> type, std::shared_ptr<Literal> value, bool isMutable)
{
    TableEntry entry(type, value, isMutable);
    auto newEntryPair = tableEntries.insert(std::pair<std::string, TableEntry>(id, entry));
    if (!newEntryPair.second)
    {
        tableEntries.at(id).type = type;
        tableEntries.at(id).value = value;
        tableEntries.at(id).isMutable = isMutable;
        tableEntries.at(id).count++;
    }
}

void SymbolTable::updateEntry(std::string id, std::shared_ptr<Literal> value)
{
    if (tableEntries.count(id))
    {
        tableEntries.at(id).value = value;
    }
}

SymbolTable::SymbolTable()
{
    tableEntries = {};
}