// **************************************************************
// *															*
// *	S Y M T A B				(Header)						*
// *															*
// *	CLASSES:	SymbolTableEntry, SymbolTable				*
// *															*
// **************************************************************

#ifndef	symtab_h
#define	symtab_h

#include <stdio.h>
#include <vector>

#define SYMMAX  500			// maximum size of symbol table

//----------------------------------------------------------------
// SymbolTableEntry class
//----------------------------------------------------------------

class SymbolTableEntry {
private:	
	char*  m_lexeme;		// the lexeme
							
public:
	SymbolTableEntry(const char* lexeme);
	// Get operations
	char *getLexeme(void);
};

//----------------------------------------------------------------
// List of SymbolTableEntries 
//----------------------------------------------------------------

typedef std::vector<SymbolTableEntry> EntryList;

//----------------------------------------------------------------
// SymbolTable class
//----------------------------------------------------------------

class SymbolTable {
private:
	SymbolTableEntry *m_entry[SYMMAX];
	int lastEntry;
public:
	SymbolTable(void);
	SymbolTableEntry* insert(const char* lexeme);
	SymbolTableEntry* lookup(const char* lexeme);
	void print(void);
};

#endif
