#include "token.h"

Token::Token()
{}


//public
TokenCode Token::getTokenCode(void) const
{
	return TokenCode();
}
DataType Token::getDataType(void) const
{
	return DataType();
}
DataValue Token::getDataValue(void) const
{
	return DataValue();
}
SymbolTableEntry* Token::getSymTabEntry(void) const
{
	return m_symtabEntry;
}
// Set operations
void Token::setTokenCode(const TokenCode tc)
{}
void Token::setDataType(const DataType type)
{}
void Token::setDataValue(const char *val)
{}
void Token::setDataValue(const OpType oper)
{}
void Token::setSymTabEntry(SymbolTableEntry* entry)
{}

char* Token::tokenCodeToString(void)
{}
char* Token::opCodeToString(void)
{}

