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

ws				[ \t]*
comment			\{[^\}]*\}  
letter			[a-z]
digit			[0-9]
id			{letter}({letter}|{digit})*
digits			{digit}{digit}*
int_num			{digits}
optional_fraction	("."{int_num})
optional_exponent	(E[+-]?{int_num})
real_num		{int_num}{optional_fraction}?{optional_exponent}?


relop			[=|<>|<|<=|>=|>]
addop			[\+|-|or]
mulop			[\*|/|div|mod|and]
assignop		:=


%%
{int_num}		{ Oper = op_NONE; Type = dt_INTEGER; return (tc_NUMBER);}
{real_num}		{ Oper = op_NONE; Type = dt_REAL; return (tc_NUMBER);}

{comment}		{ Oper = op_NONE; Type = dt_NONE; return(tc_COMMENT);}
{ws}			{ Oper = op_NONE; Type = dt_NONE; return(tc_SPACE);}
"="				{ Oper = op_EQ; Type = dt_OP; return(tc_RELOP);}
"<>"			{ Oper = op_NE; Type = dt_OP; return(tc_RELOP);}
"<"				{ Oper = op_LT; Type = dt_OP; return(tc_RELOP);}
"<="			{ Oper = op_LE; Type = dt_OP; return(tc_RELOP);}
">="			{ Oper = op_GE; Type = dt_OP; return(tc_RELOP);}
">"				{ Oper = op_GT; Type = dt_OP; return(tc_RELOP);}

"+"				{ Oper = op_PLUS; Type = dt_OP; return(tc_ADDOP);}
"-"				{ Oper = op_MINUS; Type = dt_OP; return(tc_ADDOP);}
"or"			{ Oper = op_OR; Type = dt_OP; return(tc_ADDOP);}

"*"				{ Oper = op_MULT; Type = dt_OP; return(tc_MULOP);}
"/"				{ Oper = op_DIVIDE; Type = dt_OP; return(tc_MULOP);}
"div"			{ Oper = op_DIV; Type = dt_OP; return(tc_MULOP);}
"mod"			{ Oper = op_MOD; Type = dt_OP; return(tc_MULOP);}
"and"			{ Oper = op_AND; Type = dt_OP; return(tc_MULOP);}

":="			{ Oper = op_NONE; Type = dt_OP; return(tc_ASSIGNOP);}

";"				{ Oper = op_NONE; Type = dt_NONE; return(tc_SEMICOL);}
":"				{ Oper = op_NONE; Type = dt_NONE; return(tc_COLON);}
","				{ Oper = op_NONE; Type = dt_NONE; return(tc_COMMA);}	
"."				{ Oper = op_NONE; Type = dt_NONE; return(tc_DOT);}
".."			{ Oper = op_NONE; Type = dt_NONE; return(tc_DOTDOT);}
"("				{ Oper = op_NONE; Type = dt_NONE; return(tc_LPAREN);}
"["				{ Oper = op_NONE; Type = dt_NONE; return(tc_LBRACKET);}
")"				{ Oper = op_NONE; Type = dt_NONE; return(tc_RPAREN);}
"]"				{ Oper = op_NONE; Type = dt_NONE; return(tc_RBRACKET);}

{id}   			{ Oper = op_NONE; Type = dt_ID ;return(tc_ID);}
<<EOF>>			{ Oper = op_NONE; Type = dt_NONE; return(tc_EOF);}
.				{ Oper = op_NONE; Type = dt_NONE; return(tc_ERROR);}
"\n"			{ Oper = op_NONE; Type = dt_NONE; return(tc_NEWLINE);}
%%


