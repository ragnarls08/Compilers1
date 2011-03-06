#include "parser.h"
#include <iostream>
#include <list>
//#define debugOutput

#ifdef debugOutput
	#define dout  std::cout << "\t"
#else
	#define dout  0 && std::cout
#endif
Parser::Parser(bool listing)
{
	m_lexan = new Scanner();
	m_symTab = new SymbolTable();

	m_parserError = false;
	m_totalErrors = 0;

}

Parser::~Parser()
{
    delete m_lexan;
    delete m_symTab;
}

bool Parser::currIs(TokenCode tc)
{
	return getTokenCode() == tc;
}
bool Parser::tokenCodeIn(TokenCode tc, const TokenCode *plist)
{
	for (int i=0; plist[i] != '\0'; ++i)
		if( plist[i] == tc )
			return true;

	return false;
}
void Parser::recover(const TokenCode* plist)
{
	while( !tokenCodeIn(getTokenCode(), plist) )
		getToken();
}

void Parser::parse()
{
	//start the parsing
	getToken();//fetch the first token
	parseProgram();
	m_symTab->print();
}

void Parser::getToken()
{
	m_currentToken = m_lexan->nextToken();

	if( currIs( tc_ID ) || currIs( tc_NUMBER ) )
    {
        SymbolTableEntry *entry = m_symTab->lookup( m_currentToken->getDataValue().lexeme );

        if(!entry)
        {
            entry = m_symTab->insert( m_currentToken->getDataValue().lexeme );
            //m_currentToken->setSymTabEntry( entry );
        }

        m_currentToken->setSymTabEntry( entry );
    }
}
void Parser::match( TokenCode tc )
{
	if( !m_parserError )
	{
		Token t = Token();
		t.setTokenCode(tc);

		if( getTokenCode() != tc )
		{
					std::cout << "expected: " << t.tokenCodeToString() <<
			" Found: " << m_currentToken->tokenCodeToString() << "\n";

			//create sourceline error...
			m_parserError = true;
		}
		else
		{
			std::cout << "matched: " << m_currentToken->tokenCodeToString() << "\n";
		}

		getToken();
	}
}

void Parser::parseProgram()
{
	dout << "parseProgram\n";

	parseProgramDefinition();
	parseDeclarations(false); //false?
	parseSubprogramDeclarations();
	parseCompoundStatement();
	match( tc_DOT );
}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	dout << "parseProgramDefinition\n";

	match( tc_PROGRAM );
	match( tc_ID );
	match( tc_LPAREN );
	parseIdentifierList(0);//breyta í rétt siðar
	match( tc_RPAREN );
	match( tc_SEMICOL );

	return 0; // ??
}
void Parser::parseDeclarations(bool subProgramHead)
{
	dout << "parseDeclerations\n";

	while( currIs(tc_VAR) )
	{
		match( tc_VAR );

		parseIdentifierListAndType(true);//true?

		match( tc_SEMICOL );
	}
}
void Parser::parseSubprogramDeclaration()
{
	dout << "parseSubprogramDecleration\n";

	parseSubprogramHead();
	parseDeclarations(false);///////
	parseCompoundStatement();
}
void Parser::parseSubprogramDeclarations()
{
	dout << "parseSubprogramDeclerations\n";

	while( currIs(tc_FUNCTION) || currIs(tc_PROCEDURE) )
	{
		parseSubprogramDeclaration();
		match( tc_SEMICOL );
	}
}
void Parser::parseSubprogramHead()
{
	dout << "parseSubprogramHead\n";

	if( currIs(tc_FUNCTION) )
	{
		match( tc_FUNCTION );
		match( tc_ID );
		parseArguments();
		match( tc_COLON );
		parseStandardType();
	}
	else
	{
		match( tc_PROCEDURE );
		match( tc_ID );
		parseArguments();
	}

	match( tc_SEMICOL );
}
void Parser::parseArguments()
{
	dout << "parseArguements\n";

	if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseParameterList();
		match( tc_RPAREN );
	}
}
void Parser::parseParameterList()
{
	dout << "parseParameterList\n";

	parseIdentifierListAndType(false);
	parseParameterListMore();
}
void Parser::parseParameterListMore()
{
	dout << "parseParameterListMore\n";

	while( currIs(tc_SEMICOL) )
	{
		match( tc_SEMICOL );
		parseIdentifierListAndType(false);
	}
}
void Parser::parseIdentifierList(EntryList *eList)
{
	dout << "parseIdentifierList\n";

	match( tc_ID );
	parseIdentifierListMore(0);// breyta
	if( m_parserError)
		dout << "recover meeee....\n";
}
void Parser::parseIdentifierListMore(EntryList *eList)
{
	dout << "parseIdentifierListMore\n";

	while( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		match( tc_ID );
	}
}

void Parser::parseIdentifierListAndType(bool subProgramHead)
{
	dout << "parseIdentifierListAndType\n";

	parseIdentifierList(0);
	match( tc_COLON );
	parseType();
}

void Parser::parseType()
{
	dout << "parseType\n";

	if( currIs(tc_ARRAY) )
	{
		match( tc_ARRAY );
		match( tc_LBRACKET );
		match( tc_NUMBER);
		match( tc_DOTDOT );
		match( tc_NUMBER );
		match( tc_RBRACKET );
		match( tc_OF );

		parseStandardType();
	}
	else
		parseStandardType();
}
void Parser::parseStandardType()
{
	dout << "parseStandardType\n";

	if( currIs(tc_INTEGER) )
		match( tc_INTEGER );
	else
		match( tc_REAL );
}
void Parser::parseCompoundStatement()
{
	dout << "parseCompoundStatement\n";

	match( tc_BEGIN );

	parseOptionalStatement();

	match( tc_END );
}
void Parser::parseOptionalStatement()
{
	dout << "parseOptionalStatement\n";

	if( currIs(tc_BEGIN) || currIs(tc_ID) || currIs(tc_IF) || currIs(tc_WHILE) )
		parseStatementList();
}
void Parser::parseStatementList()
{
	dout << "parseStatementList\n";

	parseStatement();
	parseStatementListMore();
}
void Parser::parseStatementListMore()
{
	dout << "parseStatementListMore\n";

	while( currIs(tc_SEMICOL) )
	{
		match( tc_SEMICOL );
		parseStatement();
	}
}
void Parser::parseStatement()
{
	dout << "parseStatement\n";

	if( currIs(tc_ID) )
	{
		match( tc_ID );
		parseIdOrProcedureStatement(0);
	}
	else if( currIs(tc_IF) )
		parseIfStatement();
	else if( currIs(tc_WHILE) )
		parseWhileStatement();
	else
		parseCompoundStatement();
}
void Parser::parseIfStatement()
{
	dout << "parseIfStatement\n";

	match(tc_IF);
    parseExpression();
    match(tc_THEN);
    parseStatement();
    match(tc_ELSE);
    parseStatement();
}
void Parser::parseWhileStatement()
{
	dout << "parseWhileStatement\n";

	match(tc_WHILE);
    parseExpression();
    match(tc_DO);
    parseStatement();
}
void Parser::parseIdOrProcedureStatement(SymbolTableEntry* prevEntry)
{
	dout << "parseIdOrProcedureStatement\n";

	if( currIs(tc_ASSIGNOP) )
	{
		match( tc_ASSIGNOP );
		parseExpression();
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpressionList(0);
		match( tc_RPAREN );
	}
	else if( currIs(tc_LBRACKET) )
	{
		parseArrayReference();
		match( tc_ASSIGNOP );
		parseExpression();
	}
	//else empty
}

SymbolTableEntry* Parser::parseExpression()
{
	dout << "parseExpression\n";

    SymbolTableEntry* entry = parseSimpleExpression();
    parseSimpleExpressionRelop(entry);
}

void Parser::parseExpressionList(SymbolTableEntry* prevEntry)
{
	dout << "parseExpressionList\n";

	SymbolTableEntry* entry = parseExpression();
	parseExpressionListMore(0);
}

void Parser::parseExpressionListMore(EntryList* eList)
{
	dout << "parseExpressionListMore\n";

	while( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		SymbolTableEntry* entry = parseExpression();
	}
}

SymbolTableEntry* Parser::parseSimpleExpression()
{
	dout << "parseSimpleExrpession\n";

    SymbolTableEntry* entry = NULL;

	if( currIs(tc_ADDOP) )
		match( tc_ADDOP );

	parseTerm();
    parseSimpleExpressionAddop(entry);
}

SymbolTableEntry* Parser::parseSimpleExpressionRelop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrpessionRelop\n";

    SymbolTableEntry* entry = NULL;

    if( currIs(tc_RELOP) )
    {
        match(tc_RELOP);
        entry = parseSimpleExpression();
    }

    return entry;
}

SymbolTableEntry* Parser::parseSimpleExpressionAddop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrepssionAddop\n";

	if( currIs(tc_ADDOP) )
	{
		match( tc_ADDOP );

		parseTerm();
		parseSimpleExpressionAddop(0);
	}
}

SymbolTableEntry* Parser::parseTerm()
{
	dout << "parseTerm\n";

	parseFactor();
	parseTermRest(0);

    return NULL;
}

void Parser::parseArrayReference()
{
	dout << "parseArrayReference\n";

	match( tc_LBRACKET );
	parseExpression();
	match( tc_RBRACKET );
}
SymbolTableEntry* Parser::parseFactor()
{
	dout << "parseFactor\n";

	if( currIs(tc_ID) )
	{
		match( tc_ID );
		parseFactorRest(0);
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpression();
		match( tc_RPAREN );
	}
	else if( currIs(tc_NOT) )
	{
		match( tc_NOT );
		parseFactor();
	}
	else
		match( tc_NUMBER );
}
SymbolTableEntry* Parser::parseFactorRest(SymbolTableEntry* prevEntry)
{
	dout << "parseFactorRest\n";

	if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpressionList(0);
		match( tc_RPAREN );
	}
	else if( currIs(tc_LBRACKET) )
		parseArrayReference();
}
SymbolTableEntry* Parser::parseTermRest(SymbolTableEntry* prevEntry)
{
	dout << "parseTermRest\n";

	while( currIs(tc_MULOP) )
	{
		match( tc_MULOP );
		parseFactor();
		parseTermRest(0);
	}
}
