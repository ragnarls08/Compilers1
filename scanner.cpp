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
//		std::cout << "identifier found\n";
		
		TokenCode tc = keywordCheck( lexer->YYText() );			/////tolower
		if(tc == 0)//id
		{
//			std::cout << "its an id ...\n";
			char *test = "tester";
			setCurrentToken(tCode, dt_KEYWORD, lexer->YYText() );
		}
		else// not id use new tcode
		{
//			std::cout << "its a keyword ...\n";
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
/*
	char *ch = &str[0];
	while( ch != '\0')
	{
		tolower(*ch);
		*ch++;
	}
	*/
//	std::cout << "checkin: " << str << "\n";
	keyWord *iter = &keyWords[0];

	while( 1 )
	{	
		if( strcmp(iter->lexeme, str ) == 0 )
		{
//			std::cout << "MATCHED " << iter->lexeme << " with " << str << "\n";
			return iter->tCode;
		}
		
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



