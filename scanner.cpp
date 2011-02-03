#include "scanner.h"
#include "token.h"

Scanner::Scanner()
{
	lexer = new yyFlexLexer;
}
Scanner::~Scanner()
{
}

Token* Scanner::nextToken()
{
	std::cout << lexer->yylex() << "\n";	
}

