#include "token.h"
#include <iostream>
using namespace std;

Token::Token()
{
	TokenString.push_back("ID");
	TokenString.push_back("NUMBER");
	TokenString.push_back("ASSIGNOP");
	TokenString.push_back("RELOP");
	TokenString.push_back("ADDOP");
	TokenString.push_back("MULOP");
	TokenString.push_back("SEMICOL");
	TokenString.push_back("COLON");
	TokenString.push_back("COMMA");
	TokenString.push_back("DOT");
	TokenString.push_back("DOTDOT");
	TokenString.push_back("LPAREN");
	TokenString.push_back("LBRACKET");
	TokenString.push_back("RPAREN");
	TokenString.push_back("RBRACKET");
	TokenString.push_back("EOF");
	TokenString.push_back("ERROR");

	TokenString.push_back("PROGRAM");
	TokenString.push_back("VAR");
	TokenString.push_back("ARRAY");
	TokenString.push_back("OF");
	TokenString.push_back("INTEGER");
	TokenString.push_back("REAL");
	TokenString.push_back("FUNCTION");
	TokenString.push_back("PROCEDURE");
	TokenString.push_back("BEGIN");
	TokenString.push_back("END");
	TokenString.push_back("IF");
	TokenString.push_back("THEN");
	TokenString.push_back("ELSE");
	TokenString.push_back("WHILE");
	TokenString.push_back("DO");
	TokenString.push_back("NOT");
	TokenString.push_back("NONE");
	
	
	OpCodes.push_back("op_PLUS");
	OpCodes.push_back("op_MINUS");
	OpCodes.push_back("op_OR");
	OpCodes.push_back("op_MULT");
	OpCodes.push_back("op_DIVIDE");
	OpCodes.push_back("op_AND");
	OpCodes.push_back("op_DIV");
	OpCodes.push_back("op_MOD");
	OpCodes.push_back("op_LT");
	OpCodes.push_back("op_GT");
	OpCodes.push_back("op_LE");
	OpCodes.push_back("op_GE");
	OpCodes.push_back("op_EQ");
	OpCodes.push_back("op_NE");
	OpCodes.push_back("op_NONE");

}

//***********************GETTERS

TokenCode Token::getTokenCode()const{
	return m_tokenCode;
}
DataType Token::getDataType()const{
	return m_dataType;
}
DataValue Token::getDataValue()const{
	return m_dataValue;
}
SymbolTableEntry* Token::getSymTabEntry()const{
	return m_symtabEntry;
}

//***********************SETTERS

void Token::setTokenCode(const TokenCode tc){
	m_tokenCode = tc;
}
void Token::setDataType(const DataType type){
	m_dataType = type;
}
void Token::setDataValue(const char *val){
	*m_dataValue.lexeme = *val;
}
void Token::setDataValue(const OpType oper){
	m_dataValue.op = oper;
}
void Token::setSymTabEntry(SymbolTableEntry* entry){
	m_symtabEntry = 0;
}
char* Token::tokenCodeToString(){
	return TokenString[m_tokenCode-1];
}
char* Token::opCodeToString(){
	
	return OpCodes[m_dataValue.op];
}


