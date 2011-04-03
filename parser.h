// **************************************************************
// *															*
// *	P A R S E R			(Header)							*
// *															*
// *	CLASSES:	Parser										*
// *															*
// **************************************************************

#ifndef	parser_h
#define	parser_h

#include "token.h"
#include "scanner.h"
#include "symtab.h"
#include "code.h"

class Parser {

private:
	Scanner* m_lexan;		// Lexical analyzer
	SymbolTable* m_symTab;	// Symbol table
	Token* m_currentToken;	// Current token
	Code*  m_code;			// Code
	bool m_parserError;		// Has a parsing error occurred?
	int  m_totalErrors;		// Total number of errors

	bool currIs(TokenCode tc); // checks if the input tokencode matches the current token

	bool tokenCodeIn(TokenCode tc, const TokenCode *plist);	// Is the given tokencode in the list
	void recover(const TokenCode *plist);	// keep asking for tokens until token is found in the list
	void getToken();						// ask lexical analyzer for next token
	TokenCode getTokenCode() {return m_currentToken->getTokenCode();}
	void match(TokenCode tc);				// Walk over tokencode tc.  If not a match; report an error
	void setError(char *err);				// sets an appropriate error message
	void expectedTokenCode(TokenCode tc);

	void parseProgram();
	SymbolTableEntry* parseProgramDefinition();
	void parseDeclarations(bool subProgramHead);
	void parseSubprogramDeclaration();
	void parseSubprogramDeclarations();
	void parseSubprogramHead();
	void parseArguments();
	void parseParameterList();
	void parseParameterListMore();
	void parseIdentifierList(EntryList *eList);
	void parseIdentifierListMore(EntryList *eList);
	void parseIdentifierListAndType(bool subProgramHead);
	void parseType();
	void parseStandardType();
	void parseCompoundStatement();
	void parseOptionalStatement();
	void parseStatementList();
	void parseStatementListMore();
	void parseStatement();
	void parseIfStatement();
	void parseWhileStatement();
	void parseIdOrProcedureStatement(SymbolTableEntry* prevEntry);
	SymbolTableEntry* parseExpression();
	void parseExpressionList(SymbolTableEntry* prevEntry);
	void parseExpressionListMore(EntryList* eList);
	SymbolTableEntry* parseSimpleExpression();
	SymbolTableEntry* parseSimpleExpressionRelop(SymbolTableEntry* prevEntry);
	SymbolTableEntry* parseSimpleExpressionAddop(SymbolTableEntry* prevEntry);
	SymbolTableEntry* parseTerm();
	SymbolTableEntry* parseTermRest(SymbolTableEntry* prevEntry);
	SymbolTableEntry* parseFactor();
	SymbolTableEntry* parseFactorRest(SymbolTableEntry* prevEntry);
	void parseArrayReference();

	/* Code generation */
	SymbolTableEntry* newLabel();
	SymbolTableEntry* newTemp();

public:
	Parser(bool listing);
	~Parser();
	void parse();
	SymbolTable* getSymbolTable();
	Code* getCode() {return m_code;}
	int totalErrors()	{return m_totalErrors;}

};

#endif
