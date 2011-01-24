%option noyywrap
%option c++
%{
#include "stdio.h"
%}

%%
old ;
%%

int main(int argc, char* argv[]) {
yyFlexLexer Lexer;
Lexer.yylex();
return 0;
}
