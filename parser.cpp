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
}

void Parser::parse()
{
	//start the parsing
	getToken();//fetch the first token
	parseProgram();
}
void Parser::getToken()
{
	this->m_currentToken = m_lexan->nextToken();
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
}
//vantar

void Parser::parseProgram()
{
	parseProgramDefinition();	
	parseDeclarations(false); //false?
	parseSubprogramDeclarations();
	parseCompoundStatement();

}

SymbolTableEntry* Parser::parseProgramDefinition()
{
	match( tc_PROGRAM );
	getToken();
	
	match( tc_ID );
	getToken();

	match( tc_LPAREN );
	getToken();
	
	parseIdentifierList(0);//breyta í rétt siðar

	match( tc_RPAREN );
	getToken();	

	match( tc_SEMICOL );
	getToken();
	
	return 0; // ?? 
}

void Parser::parseDeclarations(bool subProgramHead)
{
	while( getTokenCode() == tc_VAR )
	{
		match( tc_VAR );
		getToken();

		parseIdentifierListAndType(true);//true?
		getToken();
	
		match( tc_SEMICOL );
		getToken();
	}
}
void Parser::parseSubprogramDeclaration()
{
	parseSubprogramHead();
	parseDeclarations(false);///////
	parseCompoundStatement();	
}
void Parser::parseSubprogramDeclarations()
{
	while( getTokenCode() == tc_FUNCTION )
	{
		parseSubprogramDeclaration();
		
		match( tc_SEMICOL );
		getToken();
	}	
}
void Parser::parseSubprogramHead()
{
	if( getTokenCode() == tc_FUNCTION )
	{
		match( tc_FUNCTION );
		getToken();

		match( tc_ID );
		getToken();

		parseArguments();

		match( tc_COLON );
		getToken();

		parseStandardType();
	}
	else
	{
		match( tc_PROCEDURE );
		getToken();
		
		match( tc_ID );
		getToken();
	
		parseArguments();		
	}

	match( tc_SEMICOL );
	getToken();
}
void Parser::parseArguments()
{
		
}
void Parser::parseParameterList(){
	
}
void Parser::parseParameterListMore(){
	
}
void Parser::parseIdentifierList(EntryList *eList)
{
	match( tc_ID );
	getToken();

	parseIdentifierListMore(0);// breyta 
}
void Parser::parseIdentifierListMore(EntryList *eList)
{
	while( getTokenCode() == tc_COMMA )
	{
		match( tc_COMMA );
		getToken();

		match( tc_ID );
		getToken();

	}	
}
void Parser::parseIdentifierListAndType(bool subProgramHead)
{
	parseIdentifierList(0);

	match( tc_COLON );
	getToken();

	parseType();
}
void Parser::parseType()
{
	if( getTokenCode() == tc_ARRAY )
	{
		match( tc_ARRAY );
		getToken();

		match( tc_LBRACKET );
		getToken();

		match( tc_NUMBER);
		getToken();

		match( tc_DOTDOT );
		getToken();

		match( tc_NUMBER );
		getToken();

		match( tc_RBRACKET );
		getToken();
		
		match( tc_OF );
		getToken();

		parseStandardType();
	}
	else
		parseStandardType();
}
void Parser::parseStandardType()
{
	if( getTokenCode() == tc_INTEGER )
		match( tc_INTEGER );
	else
		match( tc_REAL );
	

}
void Parser::parseCompoundStatement(){
	
}
void Parser::parseOptionalStatement(){
	
}
void Parser::parseStatementList(){
	
}
void Parser::parseStatementListMore(){
	
}
void Parser::parseStatement(){
	
}
void Parser::parseIfStatement(){
	
}
void Parser::parseWhileStatement(){
	
}
void Parser::parseIdOrProcedureStatement(SymbolTableEntry* prevEntry){
	
}

//vantar

void Parser::parseExpressionList(SymbolTableEntry* prevEntry){
	
}
void Parser::parseExpressionListMore(EntryList* eList){
	
}

//vantar

void Parser::parseArrayReference(){
	
}
