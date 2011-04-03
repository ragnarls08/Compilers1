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

//is currentToken same as tc
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
	//while currentToken is not in the recoverySet fetch the next token
	while( !tokenCodeIn(getTokenCode(), plist) )
	{
		//if EOF then quit
		if( getTokenCode() == tc_EOF )
		{
			return;
		}

		getToken();
	}

	//if the token is a semicol then we want to continue from the next token
//	if( getTokenCode() == tc_SEMICOL )
//				getToken();

	m_parserError = false;
}

void Parser::parse()
{
	//initialize current token
	getToken();
	parseProgram();
	match( tc_EOF );

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
void Parser::setErrorMessage(char *msg)
{
	m_lexan->setError( msg );
}
void Parser::match( TokenCode tc )
{
	//dont try to match if an error has not been recovered
	if( !m_parserError )
	{
		//if tokencode doesnt match then this is an error
		if( getTokenCode() != tc )
		{
		    m_totalErrors++;
			expectedTokenCode(tc);
			m_parserError = true;
		}
		else
		{
			getToken();
		}
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
	
	//head of program label
	m_code->generate(cd_LABEL, NULL, NULL, progName);

	parseCompoundStatement();
	if( m_parserError )
		recover( pCompoundStatement );

	//end of program match the dot and generate a return
	match( tc_DOT );
	m_code->generate(cd_RETURN, NULL, NULL, NULL);
}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	dout << "parseProgramDefinition\n";

	EntryList *eList = new EntryList();

	match( tc_PROGRAM );

	//get the name of the program from the lexeme
	SymbolTableEntry* progName = m_symTab->insert( m_currentToken->getDataValue().lexeme ); 
	match( tc_ID );
	match( tc_LPAREN );
	//get the identifiers for the program def
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

		parseIdentifierListAndType(false); 
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
	
	//return after the subprogram
	m_code->generate(cd_RETURN, NULL, NULL, NULL);
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
		
		//get the function name from the lexeme	and generate the label respectively
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
		
		//get the procedure name from the lexeme and generate the label 
		SymbolTableEntry *procEntry = m_symTab->lookup( m_currentToken->getDataValue().lexeme );
		m_code->generate(cd_LABEL, NULL, NULL, procEntry);
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
		//get the identifier and push it to the entry list
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

	//create a new entrylist to gather up the identifiers
	EntryList *eList = new EntryList();

	//send the entrylist down to get the identifiers
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
		//get the identifier from the lexeme
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
	else if( currIs(tc_BEGIN) )
	{
		parseCompoundStatement();
		if( m_parserError )
			recover( pCompoundStatement );
	}
	else
	{
		m_totalErrors++;
		m_parserError = true;
		setErrorMessage( "Excpected ID,IF,WHILE or BEGIN" );
	}
	
}
void Parser::parseIfStatement()
{
	dout << "parseIfStatement\n";

	match(tc_IF);

    SymbolTableEntry *boolValue = parseExpression();
	SymbolTableEntry *falseLabel = newLabel();
	SymbolTableEntry *endLabel = newLabel();

	//if the condition is false goto falsecode else execute the true code
	m_code->generate(cd_EQ, boolValue, m_symTab->lookup("0"), falseLabel);
	if( m_parserError )
		recover( pExpression );
    match(tc_THEN);

	//parseExpression can return correctly if the first paren was not present
	if( m_parserError && currIs(tc_RPAREN))
	{
		m_parserError = false;
		getToken();
		match(tc_THEN);
	}
		

    parseStatement();
	if( m_parserError )
		recover( pStatement );

	//true code over, goto end	
	m_code->generate(cd_GOTO, NULL,NULL, endLabel);

	//after this label the false code resides
	m_code->generate(cd_LABEL, NULL,NULL, falseLabel);

    match(tc_ELSE);
	//false code
    parseStatement();
	if( m_parserError )
		recover( pStatement );
	
	//end of if	
	m_code->generate(cd_LABEL, NULL,NULL, endLabel);	
}
void Parser::parseWhileStatement()
{
	dout << "parseWhileStatement\n";

	SymbolTableEntry *labelEnd = newLabel();
	SymbolTableEntry *labelTop = newLabel();
	
	//generate top label to evaluate expression again.
	m_code->generate(cd_LABEL, NULL,NULL, labelTop);

	match(tc_WHILE);
	//get the condition 
    SymbolTableEntry *whileEntry = parseExpression();

	//if the condition is false goto end else execute the code below
	m_code->generate(cd_EQ, whileEntry, m_symTab->lookup("0"), labelEnd);

	if( m_parserError )
		recover( pExpression );
    match(tc_DO);
    parseStatement();

	//after code executes gotoTop and evaluate the condition again
	m_code->generate(cd_GOTO, NULL, NULL, labelTop);

	//end label if while condition is not met
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
		
		//assign the entry to prevEntry	
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
	//skip arrays
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
	
	//generate call for the entries parsed
	m_code->generateCall( prevEntry, eList );
}

void Parser::parseExpressionListMore(EntryList* eList)
{
	dout << "parseExpressionListMore\n";

	if( currIs(tc_COMMA) )
	{
		match( tc_COMMA );
		//add the entry to the entrylist
		SymbolTableEntry* entry = parseExpression();
		eList->push_back( entry );
		if( m_parserError )
			recover( pExpression );

		//check if there are more entries
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

	//this addop can only be a unary minus
	if( currIs(tc_ADDOP) )//&& m_currentToken->getLexeme() == "-") //ath betur
	{
		match( tc_ADDOP );
		isUnary = true;
	}

	SymbolTableEntry* entry = parseTerm();

	if( m_parserError )	
		recover( pTerm );

	// if unary put the minus value to a temp and pass it on
	if( isUnary )
	{
		temp = newTemp();
		m_code->generate(cd_UMINUS, entry, NULL, temp);
		entry = parseSimpleExpressionAddop(temp);
	}
	else
	{ 
		entry = parseSimpleExpressionAddop(entry);
	}

	if( m_parserError )
		recover( pSimpleExpressionAddop );

	return entry;
}

SymbolTableEntry* Parser::parseSimpleExpressionRelop(SymbolTableEntry* prevEntry)
{
	dout << "parseSimpleExrpessionRelop\n";

    if( currIs(tc_RELOP) )
    {

		SymbolTableEntry* temp = newTemp();
		SymbolTableEntry* labelTrue = newLabel();
		SymbolTableEntry* labelEnd = newLabel();

		//get the op code from the lexeme
		CodeOp oper = m_code->getCodeOpFromOpCode( m_currentToken->getDataValue().op );		
        match(tc_RELOP);

		//get the right hand side of the relation
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
		//get the operator from the lexeme
		CodeOp oper = m_code->getCodeOpFromOpCode( m_currentToken->getDataValue().op );				
		match( tc_ADDOP );
	
		//get the right hand side of the calculation
		SymbolTableEntry* rhs = parseTerm();
		if( m_parserError )
			recover( pTerm );

		SymbolTableEntry* temp = newTemp();
		temp = parseSimpleExpressionAddop(temp);

		//store the calculations to the temp and return it
		m_code->generate(oper, prevEntry, rhs, temp); 

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
	//skip array reference
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
		//get the identifier from the current lexeme
		entry = m_symTab->lookup( m_currentToken->getDataValue().lexeme ); 
		
		match( tc_ID );
		entry = parseFactorRest(entry);
		if( m_parserError )
			recover( pFactorRest );
	}
	else if( currIs(tc_LPAREN) )
	{
		match( tc_LPAREN );
		entry = parseExpression();
		if( m_parserError )
			recover( pExpression );
		match( tc_RPAREN );
	}
	else if( currIs(tc_NOT) )
	{
		match( tc_NOT );
		entry = parseFactor();
		if( m_parserError )
			recover( pFactor );
	}
	else
	{
		//get the number from the current tokens lexeme
		entry =  m_symTab->lookup( m_currentToken->getDataValue().lexeme ); 
		match( tc_NUMBER );
	}
	
	return entry;
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
		match( tc_MULOP );

		SymbolTableEntry* entry = parseFactor();

		if( m_parserError )
			recover( pFactor );


		SymbolTableEntry* temp = newTemp();
		//generate the code for mult, op can only be mult
		m_code->generate(cd_MULT, prevEntry, entry, temp);
	

		entry = parseTermRest(entry);
		if( m_parserError )
			recover( pTermRest );

		return temp;
	}
	return prevEntry;
}
