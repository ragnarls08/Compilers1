#include "parser.h"
#include <iostream>

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

void Parser::parse()
{
	//start the parsing
	getToken();//fetch the first token
	parseProgram();
}

void Parser::getToken()
{
	m_currentToken = m_lexan->nextToken();

	if( m_currentToken->getTokenCode() == tc_ID || m_currentToken->getTokenCode() == tc_NUMBER )
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
	Token t = Token();
	t.setTokenCode(tc);

	if( getTokenCode() != tc )
	{
				std::cout << "expected: " << t.tokenCodeToString() <<
		" Found: " << m_currentToken->tokenCodeToString() << "\n";
	}
	else
	{
		std::cout << "matched: " << m_currentToken->tokenCodeToString() << "\n";
	}
	
	getToken();
}
//vantar

void Parser::parseProgram()
{
	std::cout << "parseProgram\n";

	parseProgramDefinition();
	parseDeclarations(false); //false?
	parseSubprogramDeclarations();
	parseCompoundStatement();

}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	std::cout << "parseProgramDefinition\n";

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
	std::cout << "parseDeclerations\n";

	while( getTokenCode() == tc_VAR )
	{
		match( tc_VAR );

		parseIdentifierListAndType(true);//true?

		match( tc_SEMICOL );
	}
}
void Parser::parseSubprogramDeclaration()
{
	std::cout << "parseSubprogramDecleration\n";

	parseSubprogramHead();
	parseDeclarations(false);///////
	parseCompoundStatement();
}
void Parser::parseSubprogramDeclarations()
{
	std::cout << "parseSubprogramDeclerations\n";

	while( getTokenCode() == tc_FUNCTION || getTokenCode() == tc_PROCEDURE )
	{
		parseSubprogramDeclaration();

		match( tc_SEMICOL );
	}
}
void Parser::parseSubprogramHead()
{
	std::cout << "parseSubprogramHead\n";

	if( getTokenCode() == tc_FUNCTION )
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
	std::cout << "parseArguements\n";

	if( getTokenCode() == tc_LPAREN )
	{
		match( tc_LPAREN );

		parseParameterList();

		match( tc_RPAREN );
	}
}
void Parser::parseParameterList()
{
	std::cout << "parseParameterList\n";

	parseIdentifierListAndType(false);
	parseParameterListMore();
}
void Parser::parseParameterListMore()
{
	std::cout << "parseParameterListMore\n";

	while( getTokenCode() == tc_SEMICOL )
	{
		match( tc_SEMICOL );

		parseIdentifierListAndType(false);
	}
}
void Parser::parseIdentifierList(EntryList *eList)
{
	std::cout << "parseIdentifierList\n";

	match( tc_ID );

	parseIdentifierListMore(0);// breyta
}
void Parser::parseIdentifierListMore(EntryList *eList)
{
	std::cout << "parseIdentifierListMore\n";

	while( getTokenCode() == tc_COMMA )
	{
		match( tc_COMMA );

		match( tc_ID );
	}
}

void Parser::parseIdentifierListAndType(bool subProgramHead)
{
	std::cout << "parseIdentifierListAndType\n";

	parseIdentifierList(0);
std::cout << "201\n";
	match( tc_COLON );

	parseType();
}

void Parser::parseType()
{
	std::cout << "parseType\n";

	if( getTokenCode() == tc_ARRAY )
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
	std::cout << "parseStandardType\n";

	if( getTokenCode() == tc_INTEGER )
		match( tc_INTEGER );
	else
		match( tc_REAL );
}
void Parser::parseCompoundStatement()
{
	std::cout << "parseCompoundStatement\n";

	match( tc_BEGIN );

	parseOptionalStatement();

	match( tc_END );
}
void Parser::parseOptionalStatement()
{
	std::cout << "parseOptionalStatement\n";

	if( getTokenCode() == tc_BEGIN || getTokenCode() == tc_ID 
		|| getTokenCode() == tc_IF || getTokenCode() == tc_WHILE )
		parseStatementList();
}
void Parser::parseStatementList()
{
	std::cout << "parseStatementList\n";

	parseStatement();
	parseStatementListMore();
}
void Parser::parseStatementListMore()
{
	std::cout << "parseStatementListMore\n";

	while( getTokenCode() == tc_SEMICOL )
	{
		match( tc_SEMICOL );

		parseIdentifierListAndType(false);
	}
}
void Parser::parseStatement()
{
	std::cout << "parseStatement\n";

	if( getTokenCode() == tc_ID )
	{	
		match( tc_ID );
		
		parseIdOrProcedureStatement(0);
	}
	else if( getTokenCode() == tc_IF )
		parseIfStatement();
	else if( getTokenCode() == tc_WHILE )
		parseWhileStatement();
	else
		parseCompoundStatement();
}
void Parser::parseIfStatement()
{
	std::cout << "parseIfStatement\n";
    //if expression then statement else statement
    match(tc_IF);
    parseExpression();
    match(tc_THEN);
    parseStatement();
    match(tc_ELSE);
    parseStatement();
}
void Parser::parseWhileStatement()
{
	std::cout << "parseWhileStatement\n";
    //while expression do statement
    match(tc_WHILE);
    parseExpression();
    match(tc_DO);
    parseStatement();
}
void Parser::parseIdOrProcedureStatement(SymbolTableEntry* prevEntry)
{
	std::cout << "parseIdOrProcedureStatement\n";

	if( getTokenCode() == tc_ASSIGNOP )
	{
		match( tc_ASSIGNOP );
		parseExpression();
	}
	else if( getTokenCode() == tc_LPAREN )
	{
		match( tc_LPAREN );
		parseExpressionList(0);
		match( tc_RPAREN );
	}
	else if( getTokenCode() == tc_LBRACKET )
	{
		parseArrayReference();
		match( tc_ASSIGNOP );
		parseExpression();
	}
	//else empty
}

SymbolTableEntry* Parser::parseExpression()
{
	std::cout << "parseExpression\n";

    SymbolTableEntry* entry = parseSimpleExpression();

    parseSimpleExpressionRelop(entry);
}

void Parser::parseExpressionList(SymbolTableEntry* prevEntry)
{
	std::cout << "parseExpressionList\n";

	SymbolTableEntry* entry = parseExpression();
	parseExpressionListMore(0);
}

void Parser::parseExpressionListMore(EntryList* eList)
{
	std::cout << "parseExpressionListMore\n";

	while( getTokenCode() == tc_COMMA ) 
	{
		match( tc_COMMA );
		SymbolTableEntry* entry = parseExpression();
	}
}

SymbolTableEntry* Parser::parseSimpleExpression()
{
	std::cout << "parseSimpleExrpession\n";

    SymbolTableEntry* entry = NULL;

	if( getTokenCode() == tc_ADDOP )
		match( tc_ADDOP );

	parseTerm();
    parseSimpleExpressionAddop(entry);
}

SymbolTableEntry* Parser::parseSimpleExpressionRelop(SymbolTableEntry* prevEntry)
{
	std::cout << "parseSimpleExrpessionRelop\n";

    SymbolTableEntry* entry = NULL;

    if(getTokenCode() == tc_RELOP)
    {
        match(tc_RELOP);
        entry = parseSimpleExpression();
    }

    return entry;
}

SymbolTableEntry* Parser::parseSimpleExpressionAddop(SymbolTableEntry* prevEntry)
{
	std::cout << "parseSimpleExrepssionAddop\n";

	if( getTokenCode() == tc_ADDOP )
	{
		match( tc_ADDOP );

		parseTerm();
		parseSimpleExpressionAddop(0);
	}
}

SymbolTableEntry* Parser::parseTerm()
{
	std::cout << "parseTerm\n";

	parseFactor();
	parseTermRest(0);

    return NULL;
}

void Parser::parseArrayReference()
{
	std::cout << "parseArrayReference\n";

	match( tc_LBRACKET );
	parseExpression();
	match( tc_RBRACKET );
}
SymbolTableEntry* Parser::parseFactor()
{
	std::cout << "parseFactor\n";

	if( getTokenCode() == tc_ID )
	{
		match( tc_ID );
		parseFactorRest(0);
	}
	else if( getTokenCode() == tc_LPAREN )
	{
		match( tc_LPAREN );
		parseExpression();
		match( tc_RPAREN );
	}
	else if( getTokenCode() == tc_NOT )
	{
		match( tc_NOT );
		parseFactor();
	}
	else
		match( tc_NUMBER );
}
SymbolTableEntry* Parser::parseFactorRest(SymbolTableEntry* prevEntry)
{
	std::cout << "parseFactorRest\n";

	if( getTokenCode() == tc_LPAREN )
	{
		match( tc_LPAREN );
		parseExpressionList(0);
		match( tc_RPAREN );
	}
	else if( getTokenCode() == tc_LBRACKET )
		parseArrayReference();
}
SymbolTableEntry* Parser::parseTermRest(SymbolTableEntry* prevEntry)
{
	std::cout << "parseTermRest\n";

	while( getTokenCode() == tc_MULOP )
	{
		match( tc_MULOP );
		parseFactor();
		parseTermRest(0);
	}
}
