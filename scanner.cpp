#include "scanner.h"
#include "token.h"
#include "FlexLexer.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <algorithm>

//for case insensitive comparison
#ifndef WIN32
  #define stricmp strcasecmp
  #define strnicmp strncasecmp
#endif

using namespace std;

Scanner::Scanner()
{
	lexer = new yyFlexLexer;
	m_sourceLine = new SourceLine(10); //
}
Scanner::~Scanner()
{}
void Scanner::setError(char* msg)
{
	m_sourceLine->setError(msg);
}
bool Scanner::notToParser( TokenCode tC)
{
	return (tC == tc_ERROR || tC == tc_ERROR2 || tC == tc_NEWLINE || tC == tc_COMMENT || tC == tc_SPACE);
}
Token* Scanner::nextToken()
{
	TokenCode tCode = (TokenCode)lexer->yylex();
	std::string* str = new std::string( lexer->YYText() );

	const char* lexeme = str->c_str();

	while( notToParser( tCode ) )
	{
		if(tCode == tc_ERROR)
		{
			m_sourceLine->setError( (char*)"Illegal character");
		}
		if(tCode == tc_ERROR2)
		{
			m_sourceLine->setError( (char*)"Identifier to long");
		}
		if(tCode == tc_NEWLINE)
    	{
	        m_sourceLine->newLine();
	    }
		else
	    	m_sourceLine->buildLine(lexeme);


		tCode = (TokenCode)lexer->yylex();
		str->clear();
		str->append( lexer->YYText() );

		lexeme = str->c_str();
	}

	if( tCode == tc_ID ) // identifier
	{
		TokenCode tc = keywordCheck(lexeme);

		if(tc)
		{
			tCode = tc;
			Type = dt_KEYWORD;
		}
				//convert string to lowercase
		char *temp = lowerCase(lexeme, lexer->YYLeng());
				setCurrentToken(tCode, Type, temp );
				delete temp;
	}
	else if ( Type == dt_REAL || Type == dt_INTEGER )
		setCurrentToken(tCode, Type, lexeme);
	else
		setCurrentToken(tCode, Type, Oper );

	m_sourceLine->buildLine(lexeme);
	delete str;

	return &m_currentToken;
}

char* Scanner::lowerCase(const char* str, int size)
{
    char *temp = new char[size];
    strcpy(temp, str);

	int i = 0;
	while( temp[i] )
	{
		temp[i] = tolower( temp[i] );
		i++;;
	}
	return temp;
}

TokenCode Scanner::keywordCheck(const char *str)
{
	keyWord *iter = &keyWords[0];
	//iterate through the keyword list return -1 if string is not in list
	while(iter->tCode != tc_NONE)
	{
		if( stricmp(iter->lexeme, str ) == 0 )//return correct tcode if a match is found
			return iter->tCode;

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

