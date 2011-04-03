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
	m_code = new Code();

	//initialize symbol table for boolean expressions.
	m_symTab->insert("0");
	m_symTab->insert("1");

	m_parserError = false;
	m_totalErrors = 0;
}

Parser::~Parser()
{
    delete m_lexan;
    delete m_symTab;
}

SymbolTableEntry* Parser::newLabel()
{
	SymbolTableEntry *entry = m_symTab->insert( m_code->newLabel().c_str() );
	return entry;
}
SymbolTableEntry* Parser::newTemp()
{
	SymbolTableEntry *entry = m_symTab->insert( m_code->newTemp().c_str() );
	m_code->generate(cd_VAR, NULL, NULL, entry);

	return entry;
}


bool Parser::currIs(TokenCode tc)
{
	return getTokenCode() == tc;
}
bool Parser::tokenCodeIn(TokenCode tc, const TokenCode *plist)
{
	for (int i=0; plist[i] != '\0'; ++i)
	{
		if( plist[i] == tc )
			return true;
	}

	return false;
}
void Parser::recover(const TokenCode* plist)
{
	while( !tokenCodeIn(getTokenCode(), plist) )
	{
		if( getTokenCode() == tc_EOF )
		{
			return;
		}

		getToken();
	}

	if( getTokenCode() == tc_SEMICOL )
				getToken();
	m_parserError = false;
}

void Parser::parse()
{
	//start the parsing
	getToken();//fetch the first token
	parseProgram();
	match( tc_EOF );

//	std::cout << "Symbol Table entries: " << '\n';
//	m_symTab->print();
//	m_code->print();
}

void Parser::getToken()
{
	m_currentToken = m_lexan->nextToken();
}
void Parser::expectedTokenCode(TokenCode tc)
{
	Token t = Token();
	t.setTokenCode(tc);

	std::string *str = new std::string( "Expected " );
	str->append(  t.tokenCodeToString() );

	m_lexan->setError( (char*)str->c_str() );

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
		else
			getToken();
	}
}

void Parser::parseProgram()
{
	dout << "parseProgram\n";

	SymbolTableEntry* progName = parseProgramDefinition();
	if( m_parserError )
		recover( pProgramDefinition );

	parseDeclarations(false);//program decleration
	if( m_parserError )
		recover( pDeclarations );

	m_code->generate(cd_GOTO, NULL, NULL, progName);

	parseSubprogramDeclarations();
	if( m_parserError )
		recover( pSubprogramDeclarations );

	m_code->generate(cd_LABEL, NULL, NULL, progName);

	parseCompoundStatement();
	if( m_parserError )
		recover( pCompoundStatement );

	match( tc_DOT );
}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	dout << "parseProgramDefinition\n";

	EntryList *eList = new EntryList();

	match( tc_PROGRAM );
	SymbolTableEntry* progName = m_symTab->insert( m_currentToken->getDataValue().lexeme );
	match( tc_ID );
	match( tc_LPAREN );
	parseIdentifierList(eList);
	if( m_parserError )
		recover( pIdentifierList );
	match( tc_RPAREN );
	match( tc_SEMICOL );

	return progName;
}
void Parser::parseDeclarations(bool subProgramHead)
{
	dout << "parseDeclerations\n";

	if( currIs(tc_VAR) )
	{
		match( tc_VAR );

		parseIdentifierListAndType(false);///subprogramhead
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

	parseDeclarations(true);
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

		SymbolTableEntry *funcEntry =  m_symTab->lookup( m_currentToken->getDataValue().lexeme );
		m_code->generate(cd_LABEL, NULL,NULL, funcEntry );

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

	parseIdentifierListAndType(true);
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
	//look for identifier if it doesnt exist it is inserted automatically in the lookup function
	SymbolTableEntry* identifier = m_symTab->lookup( m_currentToken->getDataValue().lexeme );
	eList->push_back(identifier);

	match( tc_ID );

	parseIdentifierListMore(eList);
	if( m_parserError )
		recover( pIdentifierListMore );
}
void Parser::parseIdentifierListMore(EntryList *eList)
{
	dout << "parseIdentifierListMore\n";

	if( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		SymbolTableEntry* identifier = m_symTab->lookup( m_currentToken->getDataValue().lexeme );
		eList->push_back(identifier);

		match( tc_ID );

		parseIdentifierListMore(eList);
		if( m_parserError )
			recover( pIdentifierListMore );
	}
}

void Parser::parseIdentifierListAndType(bool subProgramHead)
{
	dout << "parseIdentifierListAndType\n";
	EntryList *eList = new EntryList();

	parseIdentifierList(eList);
	if( m_parserError )
		recover( pIdentifierList );
	match( tc_COLON );
	parseType();

	if( subProgramHead )
	{
		m_code->generateFormals(eList);
	}
	else
	{
		m_code->generateVariables(eList);
	}
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
	m_code->generate(cd_RETURN, NULL,NULL,NULL);
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

	if( currIs(tc_SEMICOL) )
	{
		match( tc_SEMICOL );
		parseStatement();
		if( m_parserError )
			recover( pStatement );

		parseStatementListMore();
	}
}
void Parser::parseStatement()
{
	dout << "parseStatement\n";

	if( currIs(tc_ID) )
	{
		SymbolTableEntry* entry =  m_symTab->lookup( m_currentToken->getDataValue().lexeme );

		match( tc_ID );
		parseIdOrProcedureStatement(entry);
		if( m_parserError )
			recover( pIdOrProcedureStatement );
	}
	else if( currIs(tc_IF) )
	{
		parseIfStatement();
		if( m_parserError )
			recover( pIfStatement );
	}
	else if( currIs(tc_WHILE) )
	{
		parseWhileStatement();
		if( m_parserError )
			recover( pWhileStatement );
	}
	else
	{
		parseCompoundStatement();
		if( m_parserError )
			recover( pCompoundStatement );
	}
}
void Parser::parseIfStatement()
{
	dout << "parseIfStatement\n";

	match(tc_IF);
    SymbolTableEntry *boolValue = parseExpression();
	SymbolTableEntry *falseLabel = newLabel();
	SymbolTableEntry *endLabel = newLabel();

	m_code->generate(cd_EQ, boolValue, m_symTab->lookup("0"), falseLabel);
	if( m_parserError )
		recover( pExpression );
    match(tc_THEN);
    parseStatement();

	m_code->generate(cd_GOTO, NULL,NULL, endLabel);
	m_code->generate(cd_LABEL, NULL,NULL, falseLabel);

	if( m_parserError )
		recover( pStatement );
    match(tc_ELSE);
    parseStatement();

	m_code->generate(cd_LABEL, NULL,NULL, endLabel);

	if( m_parserError )
		recover( pStatement );
}
void Parser::parseWhileStatement()
{
	dout << "parseWhileStatement\n";
	
	SymbolTableEntry *labelEnd = newLabel();
	SymbolTableEntry *labelTop = newLabel();

	m_code->generate(cd_LABEL, NULL,NULL, labelTop);

	match(tc_WHILE);
    SymbolTableEntry *whileEntry = parseExpression();

	m_code->generate(cd_EQ, whileEntry, m_symTab->lookup("0"), labelEnd);

	if( m_parserError )
		recover( pExpression );
    match(tc_DO);
    parseStatement();

	m_code->generate(cd_GOTO, NULL, NULL, labelTop);
	m_code->generate(cd_LABEL, NULL, NULL, labelEnd);

	if( m_parserError )
		recover( pStatement );
}
void Parser::parseIdOrProcedureStatement(SymbolTableEntry* prevEntry)
{
	dout << "parseIdOrProcedureStatement\n";

	SymbolTableEntry* entry;

	if( currIs(tc_ASSIGNOP) )
	{
		match( tc_ASSIGNOP );
		entry = parseExpression();

		m_code->generate(cd_ASSIGN, entry, NULL, prevEntry);
		if( m_parserError )
			recover( pExpression );
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpressionList(prevEntry);
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
    entry = parseSimpleExpressionRelop(entry);
	if( m_parserError )
		recover( pSimpleExpressionRelop );

	return entry;
}

void Parser::parseExpressionList(SymbolTableEntry* prevEntry)
{
	dout << "parseExpressionList\n";

	//create a new entry list, insert the first identifier to it and pass it down to exprListMore
	EntryList *eList = new EntryList();
	SymbolTableEntry* entry = parseExpression();
	eList->push_back( entry );
	if( m_parserError )
		recover( pExpression );
	parseExpressionListMore(eList);
	if( m_parserError )
		recover( pExpressionListMore );

	//generate call
	m_code->generateCall( prevEntry, eList );
}

void Parser::parseExpressionListMore(EntryList* eList)
{
	dout << "parseExpressionListMore\n";

	if( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		SymbolTableEntry* entry = parseExpression();
		eList->push_back( entry );
		if( m_parserError )
			recover( pExpression );

		parseExpressionListMore(eList);
		if( m_parserError )
			recover( pExpressionListMore );
	}
}

SymbolTableEntry* Parser::parseSimpleExpression()
{
	dout << "parseSimpleExrpession\n";
	SymbolTableEntry* temp;

	bool isUnary = false;

	if( currIs(tc_ADDOP) )
	{
		match( tc_ADDOP );
		isUnary = true;
	}

	SymbolTableEntry* firstEntry = parseTerm();
	if( m_parserError )
		recover( pTerm );

	// ef unary minus tha setja tha minus gildid sem faeribreytu i kallid
	if( isUnary )
	{
		temp = newTemp();

		m_code->generate(cd_UMINUS, firstEntry, NULL, temp);
		//m_code->generate(cd_ASSIGN, temp, NULL, firstEntry);
		firstEntry = parseSimpleExpressionAddop(temp);
	}
	else
	{
		firstEntry = parseSimpleExpressionAddop(firstEntry);
	}

	if( m_parserError )
		recover( pSimpleExpressionAddop );

	return firstEntry;
}

SymbolTableEntry* Parser::parseSimpleExpressionRelop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrpessionRelop\n";

    if( currIs(tc_RELOP) )
    {

		SymbolTableEntry* temp = newTemp();
		SymbolTableEntry* labelTrue = newLabel();
		SymbolTableEntry* labelEnd = newLabel();
		CodeOp oper = m_code->getCodeOpFromOpCode( m_currentToken->getDataValue().op );

        match(tc_RELOP);
        SymbolTableEntry* entry = parseSimpleExpression();

		m_code->generate(oper, prevEntry, entry, labelTrue);
		m_code->generate(cd_ASSIGN, m_symTab->lookup("0"), NULL, temp);
		m_code->generate(cd_GOTO, NULL, NULL, labelEnd);
		m_code->generate(cd_LABEL, NULL, NULL, labelTrue);
		m_code->generate(cd_ASSIGN, m_symTab->lookup("1"), NULL, temp);
		m_code->generate(cd_LABEL, NULL, NULL, labelEnd);

		if( m_parserError )
			recover( pSimpleExpression );

		return temp;
    }

    return prevEntry;
}

SymbolTableEntry* Parser::parseSimpleExpressionAddop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrepssionAddop\n";

	if( currIs(tc_ADDOP) )
	{
		CodeOp oper = m_code->getCodeOpFromOpCode( m_currentToken->getDataValue().op );
		match( tc_ADDOP );

		SymbolTableEntry* rhs = parseTerm();
		if( m_parserError )
			recover( pTerm );

		SymbolTableEntry* temp = newTemp();
		temp = parseSimpleExpressionAddop(temp);

		m_code->generate(oper, prevEntry, rhs, temp);

//------------------------
		if( m_parserError )
			recover( pSimpleExpressionAddop );

		return temp;
	}

	return prevEntry;
}

SymbolTableEntry* Parser::parseTerm()
{
	dout << "parseTerm\n";

	SymbolTableEntry* entry = parseFactor();
	if( m_parserError )
		recover( pFactor );
	entry = parseTermRest(entry);
	if( m_parserError )
		recover( pTermRest );

    return entry;
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

	SymbolTableEntry* entry;

	if( currIs(tc_ID) )
	{
		entry = m_symTab->lookup( m_currentToken->getDataValue().lexeme );

		match( tc_ID );
		entry = parseFactorRest(entry);
		if( m_parserError )
			recover( pFactorRest );

		return entry;
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		entry = parseExpression();
		if( m_parserError )
			recover( pExpression );
		match( tc_RPAREN );

		return entry;
	}
	else if( currIs(tc_NOT) )
	{
		match( tc_NOT );
		entry = parseFactor();
		if( m_parserError )
			recover( pFactor );

		return entry;
	}
	else
	{
		entry =  m_symTab->lookup( m_currentToken->getDataValue().lexeme );
		match( tc_NUMBER );

		return entry;
	}
}
SymbolTableEntry* Parser::parseFactorRest(SymbolTableEntry* prevEntry)
{
	dout << "parseFactorRest\n";

	if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		parseExpressionList(prevEntry);
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

	return prevEntry;
}
SymbolTableEntry* Parser::parseTermRest(SymbolTableEntry* prevEntry)
{
	dout << "parseTermRest\n";

	if( currIs(tc_MULOP) )
	{
		CodeOp oper = m_code->getCodeOpFromOpCode( m_currentToken->getDataValue().op );

		match( tc_MULOP );
		SymbolTableEntry* entry = parseFactor();

		if( m_parserError )
			recover( pFactor );


		SymbolTableEntry* temp = newTemp();
		m_code->generate(oper, prevEntry, entry, temp);



		entry = parseTermRest(entry);
		if( m_parserError )
			recover( pTermRest );

		return temp;
	}
	return prevEntry;
}
