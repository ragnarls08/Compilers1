// **************************************************************
// *															*
// *	T O K E N				(Header)						*
// *															*
// *	CLASSES:	Token										*
// *															*
// **************************************************************

#ifndef	token_h
#define	token_h

#include "symtab.h"
#include <vector>

#define MaxLexemeSize 32


enum TokenCode {
	tc_ID=1,
	tc_NUMBER,
	tc_ASSIGNOP,
	tc_RELOP,
	tc_ADDOP,
	tc_MULOP,
	tc_SEMICOL,
	tc_COLON,
	tc_COMMA,
	tc_DOT,
	tc_DOTDOT,
	tc_LPAREN,
	tc_LBRACKET,
	tc_RPAREN,
	tc_RBRACKET,
	tc_EOF, //16
	tc_ERROR,

	tc_PROGRAM,
    tc_VAR,
    tc_ARRAY,
    tc_OF,
	tc_INTEGER,
	tc_REAL,
    tc_FUNCTION,
    tc_PROCEDURE,
    tc_BEGIN,
    tc_END,
    tc_IF,
    tc_THEN,
    tc_ELSE,//30
	tc_WHILE,
    tc_DO,
    tc_NOT,
	tc_NONE,
	tc_COMMENT,
	tc_NEWLINE,
	tc_SPACE
};

enum DataType {
	dt_INTEGER,
	dt_REAL,
	dt_ID,
	dt_KEYWORD,
	dt_OP,
	dt_NONE
};

enum OpType {
	op_PLUS,
	op_MINUS,
	op_OR,
	op_MULT,
	op_DIVIDE,
	op_AND,
	op_DIV,
	op_MOD,
	op_LT,
	op_GT,
	op_LE,
	op_GE,
	op_EQ,
	op_NE,
	op_NONE
};

union DataValue {char lexeme[MaxLexemeSize]; OpType op;};

class Token {
private:
	TokenCode	m_tokenCode;		// the token code
	DataType	m_dataType;			// the data type
	DataValue	m_dataValue;		// the attribute the token
	SymbolTableEntry*	m_symtabEntry;	// the corresponding entry in symbol table (if identifier)

	std::vector<char*> TokenString;
	std::vector<char*> OpCodes;

public:
	Token(void);
	// Get operations
	TokenCode getTokenCode(void) const;
	DataType getDataType(void) const;
	DataValue getDataValue(void) const;
	SymbolTableEntry* getSymTabEntry(void) const;
	// Set operations
	void setTokenCode(const TokenCode tc);
	void setDataType(const DataType type);
	void setDataValue(const char *val);
	void setDataValue(const OpType oper);
	void setSymTabEntry(SymbolTableEntry* entry);

	char* tokenCodeToString(void);
	char* opCodeToString(void);
};

#endif

