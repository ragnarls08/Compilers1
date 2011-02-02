%option c++
%option yylineno
%option noyywrap
%option caseless

%{
#include <iostream>
#include "token.h"
OpType Oper=op_NONE;
DataType Type=dt_NONE;

using namespace std;
%}

digit		[0-9]
alpha		[a-fA-F]
hextail		({digit}|{alpha}){1,8}
hex		0[xX]{hextail}

%%
{hex}	cout << "hex number " << YYText() << "\n";
.		printf("");
%%

main()
{
	printf("Give me your input:\n");
	yyFlexLexer Lexer;
	Lexer.yylex();
	return 0;
}

