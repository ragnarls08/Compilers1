#include "scanner.h"
#include "token.h"
#include "FlexLexer.h"
#include <stdio.h>
#include <string.h>

Scanner::Scanner()
{
	lexer = new yyFlexLexer;
	
}
Scanner::~Scanner()
{
}

Token* Scanner::nextToken()
{

	TokenCode tCode;
	tCode = (TokenCode)lexer->yylex();

	if( tCode == tc_ID ) //check if keyword, if so change tCode, set type to keyword
	{
		std::cout << "identifier found\n";
		
		TokenCode tc = keywordCheck( lexer->YYText() );			
		if(tc == 0)//id
		{
			std::cout << "its an id ...\n";
			char *test = "tester";
			setCurrentToken(tCode, Type, test);
		}
		else// not id use new tcode
		{
			std::cout << "its a keyword ...\n";
			setCurrentToken(tCode, Type, Oper);
		}
	}
	else 
	{
		
		setCurrentToken(tCode, Type, Oper);
	}

	return &m_currentToken;
}

TokenCode Scanner::keywordCheck(const char *str)
{
	std::cout << "checkin: " << str << "\n";
	keyWord *iter = &keyWords[0];

	while( 1 )
	{
		std::cout << iter->lexeme << "\n";
		
		std::cout << "str : " << str << "\n";
		if( strcmp(iter->lexeme, str ) == 0 )
			std::cout << "ITS THE SAME \n\n\n";
		
		if( iter->tCode == tc_NONE )
			break;
			
		*iter++;
	}	
	return (TokenCode)0;
}
void Scanner::setCurrentToken(TokenCode tc, DataType ty, OpType oper)
{
	m_currentToken.setTokenCode(tc);
	m_currentToken.setDataType(ty);	
	m_currentToken.setDataValue(oper);
}
void Scanner::setCurrentToken(TokenCode tc, DataType ty, const char* lexeme)
{
	m_currentToken.setTokenCode(tc);
	m_currentToken.setDataType(ty);
	m_currentToken.setDataValue(lexeme);
}




