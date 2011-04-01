// **************************************************************
// *															*
// *	C O D E		(Header)									*
// *															*
// *	CLASSES:	Quadruple, QuadrupleList, Code				*
// *															*
// **************************************************************

#ifndef	code_h
#define	code_h

#include <stdlib.h>
#include "symtab.h"
#include <vector>
#include <string>

enum CodeOp  {	cd_LABEL, cd_UMINUS, cd_ASSIGN, cd_ADD, cd_SUB, cd_MULT, cd_DIVIDE, cd_DIV, cd_MOD,
				cd_OR, cd_AND, cd_NOT, cd_LT, cd_LE, cd_GT, cd_GE, cd_EQ, cd_NE,
				cd_GOTO, cd_CALL, cd_APARAM, cd_FPARAM, cd_VAR, cd_RETURN, cd_NOOP};

class Quadruple {				/* A record for intermediate code  */
private:
	CodeOp m_op;				/* operator */
	SymbolTableEntry* m_arg1;	/* arg1 is in symbol table */
	SymbolTableEntry* m_arg2;	/* arg2 is in symbol table */
	SymbolTableEntry* m_result;	/* result is in symbol table */
public:
	Quadruple();
	CodeOp getOp() { return m_op;}
	void set(CodeOp op, SymbolTableEntry* arg1, SymbolTableEntry* arg2, SymbolTableEntry* result);
	void print();
};

class Code {
private:
	int m_tempCount;
	int m_labelCount;
	std::vector<Quadruple> m_QdrList;
public:
	Code();
	void generate(CodeOp op, SymbolTableEntry* arg1, SymbolTableEntry* arg2, SymbolTableEntry* result);
	void generateCall(SymbolTableEntry* entry, EntryList* eList);
	void generateFormals(EntryList* eList);
	void generateVariables(EntryList* eList);
	std::string newLabel();
	std::string newTemp();
	void print();
};

#endif
