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
	parseProgram();
}
void Parser::getToken()
{
	this->m_currentToken = m_lexan->nextToken();
}
void Parser::match( TokenCode tc )
{
	if( getTokenCode() != tc )
	{
		Token t = Token();
		t.setTokenCode(tc);

		std::cout << "expected: " << t.tokenCodeToString() <<
		" Found: " << m_currentToken->tokenCodeToString() << "\n";
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
	getToken();
	match( tc_PROGRAM );

	getToken();
	match( tc_ID );

	getToken();
	match( tc_LPAREN );

	getToken();
//	parseIdentifierList();

	getToken();
	match( tc_RPAREN );

	
	return 0; // ?? 
}

void Parser::parseDeclarations(bool subProgramHead){
	
}
void Parser::parseSubprogramDeclaration(){
	
}
void Parser::parseSubprogramDeclarations(){
	
}
void Parser::parseSubprogramHead(){
	
}
void Parser::parseArguments(){
	
}
void Parser::parseParameterList(){
	
}
void Parser::parseParameterListMore(){
	
}
void Parser::parseIdentifierList(EntryList *eList){
	
}
void Parser::parseIdentifierListMore(EntryList *eList){
	
}
void Parser::parseIdentifierListAndType(bool subProgramHead){
	
}
void Parser::parseType(){
	
}
void Parser::parseStandardType(){
	
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
