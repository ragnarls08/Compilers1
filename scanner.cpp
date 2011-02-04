#include "scanner.h"
#include "token.h"
#include "FlexLexer.h"
#include <stdio.h>
#include <string.h>

//for case insensitive comparison
#ifndef WIN32 
  #define stricmp strcasecmp 
  #define strnicmp strncasecmp 
#endif

Scanner::Scanner()
{
	lexer = new yyFlexLexer;
}
Scanner::~Scanner()
{}

Token* Scanner::nextToken()
{
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
	/*	
		char *temp = new *char[32];
		strcpy(*temp, lexer->YYText() );
		int i = 0;
		while( temp[i] != '\0' )
		{
//			*temp[i] = tolower(*temp[i]);
			i++;;
		}
*/
		setCurrentToken(tCode, Type, lexer->YYText() );
		//delete temp;
		
	}
	else if ( Type == dt_REAL || Type == dt_INTEGER )
		setCurrentToken(tCode, Type, lexer->YYText());
	else
		setCurrentToken(tCode, Type, Oper );

	return &m_currentToken;
}

TokenCode Scanner::keywordCheck(const char *str)
{
	keyWord *iter = &keyWords[0];
	//iterate through the keyword list return -1 if string is not in list
	while( 1 )
	{	
		if( stricmp(iter->lexeme, str ) == 0 )//return correct tcode if a match is found
			return iter->tCode;
		
		
		if( iter->tCode == tc_NONE ) // break if at the end
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

