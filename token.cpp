#include "token.h"
#include <iostream>
using namespace std;

Token::Token(){
}

Token::~Token(){
}

//***********************GETTERS

TokenCode Token::getTokenode()const{
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
	this.m_tokenCode = tc;
}
void Token::setDataType(const DataType type){
	this.m_dataType = type;
}
void Token::setDataValue(const char *val){
	//this.m_dataValue = *val
}
void Token::setDataValue(){
	//this.m_dataValue = *val
}
char* Token::tokenCodeToString(){
	cout << m_tokenCode << '\n';
}
char* Token::opCodeToString(){
	cout << m_
}
