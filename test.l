%option c++
%option yylineno
%option noyywrap	
%option caseless

%{
#include "token.h"
OpType Oper=op_NONE;
DataType Type=dt_NONE;

using namespace std;
%}


letter	[a-z]
digit	[0-9]

id		{letter}({letter}|{digit})*
%%
"<="		{ Oper = op_LE; Type = dt_OP; return(tc_RELOP);}
{id}   		{ Oper = op_NONE; Type = dt_ID ;return(tc_ID);}
.			{ ;}
%%


