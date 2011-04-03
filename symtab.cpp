#include "symtab.h"
#include <string.h>
#include <iostream>

#ifndef WIN32
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

using namespace std;

EntryList::EntryList()
{
	m_elist = std::vector<SymbolTableEntry*>();
}
void EntryList::push_back( SymbolTableEntry* entry )
{
	m_elist.push_back( entry );
}
//SymbolTableEntry
SymbolTableEntry::SymbolTableEntry(const char* lexeme)
{
	m_lexeme = new char[strlen(lexeme)];
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
		return NULL;
	}
}
SymbolTableEntry* SymbolTable::lookup(const char* lexeme)
{
	int i = 0;
	while(i <= lastEntry)
	{
	    //this is a hakk, ! is simply because strcomp returns 0 if it is a match
		if (!(stricmp(m_entry[i]->getLexeme(),lexeme)))
			return m_entry[i];
		i++;
	}
	return insert( lexeme );
}
void SymbolTable::print(void){
	int i = 0;
	while(i <= lastEntry)
	{
		cout << m_entry[i]->getLexeme() << endl;
		i++;
	}
}



