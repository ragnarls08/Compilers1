#include "parser.h"
#include <iostream>
#include <list>
#include "lists.h"
#include <string>

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
	std::cout << "recovering...\n";
	while( !tokenCodeIn(getTokenCode(), plist) && getTokenCode() != tc_EOF )
		getToken();

	if( getTokenCode() == tc_SEMICOL )
				getToken();

	m_parserError = false;	
}

void Parser::parse()
{
	//start the parsing
	getToken();//fetch the first token
	parseProgram();
	std::cout << "Symbol Table entries: " << '\n';
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
void Parser::expectedTokenCode(TokenCode tc)
{
	Token t = Token();
	t.setTokenCode(tc);

	std::string *str = new std::string( "Expected " );
	str->append(  t.tokenCodeToString() );

	m_lexan->setError( (char*)str->c_str() );	
//	m_lexan->setError( (char*)"raggibesti" );	
}
void Parser::match( TokenCode tc )
{
	if( !m_parserError )
	{
		if( getTokenCode() != tc )
		{
			expectedTokenCode(tc);
			m_parserError = true;
		}

		getToken();
	}
}

void Parser::parseProgram()
{
	dout << "parseProgram\n";

	parseProgramDefinition();
	if( m_parserError )
		recover( pProgramDefinition );

	parseDeclarations(false); //false?
	if( m_parserError )
		recover( pDeclarations );

	parseSubprogramDeclarations();
	if( m_parserError )
		recover( pSubprogramDeclarations );

	parseCompoundStatement();
	if( m_parserError )
		recover( pCompoundStatement );

	match( tc_DOT );
}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	dout << "parseProgramDefinition\n";

	match( tc_PROGRAM );
	match( tc_ID );
	match( tc_LPAREN );
	parseIdentifierList(0);//breyta í rétt siðar
	if( m_parserError )
		recover( pIdentifierList );
	match( tc_RPAREN );
	match( tc_SEMICOL );

	return 0; // ??
}
void Parser::parseDeclarations(bool subProgramHead)
{
	dout << "parseDeclerations\n";

	if( currIs(tc_VAR) )
	{
		match( tc_VAR );

		parseIdentifierListAndType(true);//true?
		if( m_parserError )
			recover( pIdentifierListAndType );

		match( tc_SEMICOL );

		parseDeclarations(false);
		if( m_parserError )
			recover( pDeclarations );
	}
}
void Parser::parseSubprogramDeclaration()
{
	dout << "parseSubprogramDecleration\n";

	parseSubprogramHead();
	if( m_parserError )
		recover( pSubprogramHead );

	parseDeclarations(false);///////
	if( m_parserError )
		recover( pDeclarations );

	parseCompoundStatement();
	if( m_parserError )
		recover( pCompoundStatement );
}
void Parser::parseSubprogramDeclarations()
{
	dout << "parseSubprogramDeclerations\n";

	if( currIs(tc_FUNCTION) || currIs(tc_PROCEDURE) )
	{
		parseSubprogramDeclaration();
		if( m_parserError )
			recover( pSubprogramDeclaration );
		match( tc_SEMICOL );

		parseSubprogramDeclarations();
		if( m_parserError )
			recover( pSubprogramDeclarations );
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
		if( m_parserError )
			recover( pArguments );
		match( tc_COLON );
		parseStandardType();
		if( m_parserError )
			recover( pStandardType );
	}
	else
	{
		match( tc_PROCEDURE );
		match( tc_ID );
		parseArguments();
		if( m_parserError )
			recover( pArguments );
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
		if( m_parserError )
			recover( pParameterList );
		match( tc_RPAREN );
	}
}
void Parser::parseParameterList()
{
	dout << "parseParameterList\n";

	parseIdentifierListAndType(false);
	if( m_parserError )
		recover( pIdentifierListAndType );
	parseParameterListMore();
	if( m_parserError )
		recover( pParameterListMore );
}
void Parser::parseParameterListMore()
{
	dout << "parseParameterListMore\n";

	if( currIs(tc_SEMICOL) )
	{
		match( tc_SEMICOL );
		parseIdentifierListAndType(false);
		if( m_parserError )
			recover( pIdentifierListAndType );	
		parseParameterListMore();
		if( m_parserError )	
			recover( pParameterListMore );
	}
}
void Parser::parseIdentifierList(EntryList *eList)
{
	dout << "parseIdentifierList\n";

	match( tc_ID );
	parseIdentifierListMore(0);
	if( m_parserError )
		recover( pIdentifierListMore );
}
void Parser::parseIdentifierListMore(EntryList *eList)
{
	dout << "parseIdentifierListMore\n";

	if( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		match( tc_ID );
	
		parseIdentifierListMore(0);
		if( m_parserError )
			recover( pIdentifierListMore );
	}
}

void Parser::parseIdentifierListAndType(bool subProgramHead)
{
	dout << "parseIdentifierListAndType\n";

	parseIdentifierList(0);
	if( m_parserError )
		recover( pIdentifierList );
	match( tc_COLON );
	parseType();
	if( m_parserError )
		recover( pType );
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
	}

	parseStandardType();
	if( m_parserError )
		recover( pStandardType );
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
	if( m_parserError )
		recover( pOptionalStatement );

	match( tc_END );
}
void Parser::parseOptionalStatement()
{
	dout << "parseOptionalStatement\n";

	if( currIs(tc_BEGIN) || currIs(tc_ID) || currIs(tc_IF) || currIs(tc_WHILE) )
	{
		parseStatementList();
		if( m_parserError )
			recover( pStatementList );
	}
}
void Parser::parseStatementList()
{
	dout << "parseStatementList\n";

	parseStatement();
	if( m_parserError )
		recover( pStatement );
	parseStatementListMore();
	if( m_parserError )
		recover( pStatementListMore );
}
void Parser::parseStatementListMore()
{
	dout << "parseStatementListMore\n";

	while( currIs(tc_SEMICOL) )
	{
		match( tc_SEMICOL );
		parseStatement();
		if( m_parserError )
			recover( pStatement );
	}
}
void Parser::parseStatement()
{
	dout << "parseStatement\n";

	if( currIs(tc_ID) )
	{
		match( tc_ID );
		parseIdOrProcedureStatement(0);
		if( m_parserError )
			recover( pIdOrProcedureStatement );
	}
	else if( currIs(tc_IF) )
		parseIfStatement();
		if( m_parserError )
			recover( pIfStatement );
	else if( currIs(tc_WHILE) )
		parseWhileStatement();
		if( m_parserError )
			recover( pWhileStatement );
	else
		parseCompoundStatement();
		if( m_parserError )
			recover( pCompoundStatement );
}
void Parser::parseIfStatement()
{
	dout << "parseIfStatement\n";

	match(tc_IF);
    parseExpression();
	if( m_parserError )
		recover( pExpression );
    match(tc_THEN);
    parseStatement();
	if( m_parserError )
		recover( pStatement );
    match(tc_ELSE);
    parseStatement();
	if( m_parserError )
		recover( pStatement );
}
void Parser::parseWhileStatement()
{
	dout << "parseWhileStatement\n";

	match(tc_WHILE);
    parseExpression();
	if( m_parserError )
		recover( pExpression );
    match(tc_DO);
    parseStatement();
	if( m_parserError )
		recover( pStatement );
}
void Parser::parseIdOrProcedureStatement(SymbolTableEntry* prevEntry)
{
	dout << "parseIdOrProcedureStatement\n";

	if( currIs(tc_ASSIGNOP) )
	{
		match( tc_ASSIGNOP );
		parseExpression();
		if( m_parserError )
			recover( pExpression );
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpressionList(0);
		if( m_parserError )
			recover( pExpressionList );
		match( tc_RPAREN );
	}
	else if( currIs(tc_LBRACKET) )
	{
		parseArrayReference();
		if( m_parserError )
			recover( pArrayReference );

		match( tc_ASSIGNOP );
		parseExpression();
		if( m_parserError )
			recover( pExpression );
	}
	//else empty
}

SymbolTableEntry* Parser::parseExpression()
{
	dout << "parseExpression\n";

    SymbolTableEntry* entry = parseSimpleExpression();
	if( m_parserError )
		recover( pSimpleExpression );
    parseSimpleExpressionRelop(entry);
	if( m_parserError )
		recover( pSimpleExpressionRelop );
}

void Parser::parseExpressionList(SymbolTableEntry* prevEntry)
{
	dout << "parseExpressionList\n";

	SymbolTableEntry* entry = parseExpression();
	if( m_parserError )	
		recover( pExpression );
	parseExpressionListMore(0);
	if( m_parserError )
		recover( pExpressionListMore );
}

void Parser::parseExpressionListMore(EntryList* eList)
{
	dout << "parseExpressionListMore\n";

	if( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		SymbolTableEntry* entry = parseExpression();
		if( m_parserError )
			recover( pExpression );

		parseExpressionListMore(0);
		if( m_parserError )
			recover( pExpressionListMore );
	}
}

SymbolTableEntry* Parser::parseSimpleExpression()
{
	dout << "parseSimpleExrpession\n";

    SymbolTableEntry* entry = NULL;

	if( currIs(tc_ADDOP) )
		match( tc_ADDOP );

	parseTerm();
	if( m_parserError )	
		recover( pTerm );
    parseSimpleExpressionAddop(entry);
	if( m_parserError )
		recover( pSimpleExpressionAddop );
}

SymbolTableEntry* Parser::parseSimpleExpressionRelop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrpessionRelop\n";

    SymbolTableEntry* entry = NULL;

    if( currIs(tc_RELOP) )
    {
        match(tc_RELOP);
        entry = parseSimpleExpression();
		if( m_parserError )
			recover( pSimpleExpression );
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
		if( m_parserError )
			recover( pTerm );
		parseSimpleExpressionAddop(0);
		if( m_parserError )
			recover( pExpressionAddop );
	}
}

SymbolTableEntry* Parser::parseTerm()
{
	dout << "parseTerm\n";

	parseFactor();
	if( m_parserError )
		recover( pFactor );
	parseTermRest(0);
	if( m_parserError )
		recover( pTermRest );

    return NULL;
}

void Parser::parseArrayReference()
{
	dout << "parseArrayReference\n";

	match( tc_LBRACKET );
	parseExpression();
	if( m_parserError )
		recover( pExpression );
	match( tc_RBRACKET );
}
SymbolTableEntry* Parser::parseFactor()
{
	dout << "parseFactor\n";

	if( currIs(tc_ID) )
	{
		match( tc_ID );
		parseFactorRest(0);
		if( m_parserError )
			recover( pFactorRest );
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpression();
		if( m_parserError )
			recover( pExpression );
		match( tc_RPAREN );
	}
	else if( currIs(tc_NOT) )
	{
		match( tc_NOT );
		parseFactor();
		if( m_parserError )
			recover( pFactor );
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
		if( m_parserError )
			recover( pExpressionList );
		match( tc_RPAREN );
	}
	else if( currIs(tc_LBRACKET) )
	{
		parseArrayReference();
		if( m_parserError )
			recover( pArrayReference );
	}
}
SymbolTableEntry* Parser::parseTermRest(SymbolTableEntry* prevEntry)
{
	dout << "parseTermRest\n";

	if( currIs(tc_MULOP) )
	{
		match( tc_MULOP );
		parseFactor();
		if( m_parserError )
			recover( pFactor );
		parseTermRest(0);
		if( m_parserError )
			recover( pTermRest );
	
		parseTermRest(0);
		if( m_parserError )
			recover( pTermRest );
	}
}
