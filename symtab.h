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

class EntryList {
private:
	SymbolTableEntry* m_entry;
	EntryList* m_next;

	void setEntry(SymbolTableEntry* entry)	{m_entry = entry;}
public:
	EntryList() {m_entry = NULL; m_next = NULL;}
	SymbolTableEntry* getEntry() {return m_entry;}
	EntryList* getNext() { return m_next;}
	void setNext(EntryList* eList) {m_next = eList;}
	void addEntry(SymbolTableEntry* entry);    // adds an entry 
	int count();					// count of items in list
};

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
