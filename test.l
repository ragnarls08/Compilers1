%option c++
%option yylineno
%option noyywrap	
%option caseless

%{
#include "token.h"
OpType Oper = op_NONE;
DataType Type = dt_NONE;

using namespace std;
%}

letter			[a-z]
digit			[0-9]
id			{letter}({letter}|{digit})*
digits			{digit}{digit}*
int_num			{digits}
optional_fraction	("."{int_num})
optional_exponent	(E(\+|-)?{int_num})
real_num		{int_num}{optional_fraction}?{optional_exponent}?

%%
{int_num}	{ Oper = op_NONE; Type = dt_INTEGER; return (tc_NUMBER);}
{real_num}	{ Oper = op_NONE; Type = dt_REAL; return (tc_NUMBER);}

"<="		{ Oper = op_LE; Type = dt_OP; return(tc_RELOP);}
{id}   		{ Oper = op_NONE; Type = dt_ID ;return(tc_ID);}
<<EOF>>		{ Oper = op_NONE; Type = dt_NONE; return(tc_EOF);}
.			{ ;}
"\n"		{ ;}
%%


