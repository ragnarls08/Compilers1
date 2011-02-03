#include "scanner.h"
#include "token.h"
#include "FlexLexer.h"


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
//	std::cout << lexer->YYText() << "\n";

	std::cout << "\nOper: " << Oper << "\n";
	std::cout << "\nType: " << Type << "\n";

	Token *token = new Token();
	token->setTokenCode(tCode);

	return token;
	//std::cout << token.tokenCodeToString() << "\n";
//	std::cout << lexer->yylex() << "\n";		
}

