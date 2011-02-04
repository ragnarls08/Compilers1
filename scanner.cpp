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
//If the current token is an identifier then you need to check if it is a keyword. 
//If that is the case, then you should change tCode appropriately, as well as setting Type= dt_KEYWORD.  
//Each keyword has a separate token name.

	TokenCode tCode;
	tCode = (TokenCode)lexer->yylex();

	if( Type == dt_ID ) // identifier
	{
		TokenCode tc = keywordCheck( lexer->YYText() );
		if( tc != -1 )// its a keyword
		{
			tCode = tc;
			Type = dt_KEYWORD;
		}
		setCurrentToken(tCode, Type, lexer->YYText() );
	}
	else if ( Type == dt_REAL || Type == dt_INTEGER )
		setCurrentToken(tCode, Type, lexer->YYText());
	else
		setCurrentToken(tCode, Type, Oper );

	return &m_currentToken;
}

TokenCode Scanner::keywordCheck(const char *str)
{
//	std::cout << "YYText is now: " << str << "\n";
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
	return (TokenCode)-1;
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



