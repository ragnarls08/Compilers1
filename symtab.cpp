#include "symtab.h"
#include <string.h>
#include <iostream>

#ifndef WIN32 
#define stricmp strcasecmp 
#define strnicmp strncasecmp 
#endif

using namespace std;

//SymbolTableEntry
SymbolTableEntry::SymbolTableEntry(const char* lexeme)
{
	strcpy(m_lexeme, lexeme);

	int i = 0;
	while( m_lexeme[i] )
	{
		m_lexeme[i] = tolower( m_lexeme[i] );
		i++;
	}
}
// Get operations
char *SymbolTableEntry::getLexeme(void)
{
	return m_lexeme;
}



//EntryList
void EntryList::addEntry(SymbolTableEntry* entry)
{
	m_next = new EntryList();
	m_next->setEntry(entry);

}
int EntryList::count()
{
	int counter = 0;

	while(m_next)
		counter++;

	return counter;
}


//SymbolTable
SymbolTable::SymbolTable(void)
{
	lastEntry = -1;
}
SymbolTableEntry* SymbolTable::insert(const char* lexeme)
{
	if (lastEntry < SYMMAX)
	{
		lastEntry++;
		return m_entry[lastEntry] = new SymbolTableEntry(lexeme);
	}
	else
	{
		cout << "My Symbol Table's to big!" << endl;
	}
}
SymbolTableEntry* SymbolTable::lookup(const char* lexeme)
{
	int i = 0;
	while(i <= lastEntry)
	{
		if (!(stricmp(m_entry[i]->getLexeme(),lexeme)))
			return m_entry[i];
		i++;
	}
}
void SymbolTable::print(void)
{
	int i = 0;
	while(i <= lastEntry)
	{
		cout << m_entry[i]->getLexeme() << endl;
		i++;
	}	
}



