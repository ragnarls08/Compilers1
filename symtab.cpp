#include "symtab.h"


//SymbolTableEntry
SymbolTableEntry::SymbolTableEntry(const char* lexeme)
{}
// Get operations
char *SymbolTableEntry::getLexeme(void)
{}



//EntryList
void EntryList::addEntry(SymbolTableEntry* entry)
{}
int EntryList::count()
{}


//SymbolTable
SymbolTable::SymbolTable(void)
{}
SymbolTableEntry* SymbolTable::insert(const char* lexeme)
{}
SymbolTableEntry* SymbolTable::lookup(const char* lexeme)
{}
void SymbolTable::print(void)
{}



