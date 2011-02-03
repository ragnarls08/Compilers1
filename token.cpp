#include "token.h"
#include <iostream>
using namespace std;

Token::Token()
{}

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
	cout << m_tokenCode << '\n';///////////////////////////
}
char* Token::opCodeToString(){
	cout << "lol\n";//////////////////////////////////////
}


