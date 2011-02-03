// **************************************************************
// *															*
// *	S C A N N E R			(Header)						*
// *															*
// *	CLASSES:	Scanner										*
// *															*
// **************************************************************

#ifndef	scanner_h
#define	scanner_h

#include "token.h"
#include "FlexLexer.h"

extern OpType Oper;
extern DataType Type;

struct keyWord {
	char	*lexeme;
	TokenCode	tCode;
};

static keyWord keyWords[] = {             /* Reserved words */
	{"program", tc_PROGRAM },
	{"var", tc_VAR },
	{"array", tc_ARRAY}, 
	{"of", tc_OF},
	{"integer", tc_INTEGER},
	{"real", tc_REAL},
	{"function", tc_FUNCTION}, 
	{"procedure", tc_PROCEDURE},
	{"begin", tc_BEGIN},
	{"end", tc_END},
	{"if", tc_IF},
	{"then", tc_THEN},
	{"else", tc_ELSE},
	{"while", tc_WHILE},
	{"do", tc_DO},
	{"not", tc_NOT},
	{"not_used", tc_NONE}
};

class Scanner {

private:
	FlexLexer* lexer;				// A Flex scanner
	Token	m_currentToken;				// current token
	void	setCurrentToken(TokenCode tc, DataType ty, OpType oper); // sets the corresponding fields in m_currentToken
	void	setCurrentToken(TokenCode tc, DataType ty, const char* lexeme); // sets the corresponding fields in m_currentToken
	TokenCode keywordCheck(const char *str);	// returns the appropriate tokencode if str is a keyword, else tc_ID
											
public:
	Scanner();
	~Scanner();
	Token* nextToken();				// returns the next token from FlexLexer	
	
};



#endif

