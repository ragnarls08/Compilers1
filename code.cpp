#include "code.h"
#include <sstream>
#include <iostream>
Quadruple::Quadruple()
{
}

Quadruple::Quadruple(CodeOp op, SymbolTableEntry* arg1, SymbolTableEntry* arg2, SymbolTableEntry* result) : m_op(op), m_arg1(arg1), m_arg2(arg2), m_result(result)
{
} 

void Quadruple::set(CodeOp op, SymbolTableEntry* arg1, SymbolTableEntry* arg2, SymbolTableEntry* result)
{
	m_op = op;
	m_arg1 = arg1;
	m_arg2 = arg2;
	m_result = result;
}

void Quadruple::print()
{
	char* CodeStrings[] = {"LABEL", "UMINUS", "ASSIGN", "ADD", "SUB", "MULT", "DIVIDE", "DIV", "MOD", "OR", "AND", "NOT", "LT", "LE", "GT", "GE", "EQ", "NE", "GOTO", "CALL", "APARAM", "FPARAM", "VAR", "RETURN", "NOOP"};

	static bool lastisLabel = false;
	char *opStr;

	opStr = CodeStrings[m_op];
	
	if (m_op == cd_LABEL) {
		if (lastisLabel)
			printf("\n");
		printf("%7s",m_result->getLexeme());
		printf("%c",':');
		lastisLabel = true;
	}
	else {
		if (!lastisLabel)
		   printf("%8s"," ");
		printf("%10s",opStr);
		
		if (m_arg1 != NULL)
			printf("%15s", m_arg1->getLexeme());
		else
			printf("%15s"," ");
		
		if (m_arg2 != NULL) 
			printf("%15s", m_arg2->getLexeme());
		else
			printf("%15s"," ");
		
		if (m_result != NULL)
			printf("%15s\n", m_result->getLexeme());
		else
			printf("%15s\n"," ");

		lastisLabel = false;
	}	
}

//---------------------------------------------------------
//Code
Code::Code()
{
	//todo construct
	m_tempCount = 1;
	m_labelCount = 1;
}

void Code::generate(CodeOp op, SymbolTableEntry* arg1, SymbolTableEntry* arg2, SymbolTableEntry* result)
{
	m_QdrList.push_back( Quadruple(op, arg1, arg2, result ) );
}

void Code::generateCall(SymbolTableEntry* entry, EntryList* eList)
{
	for(int i=0; i<eList->m_elist.size(); ++i)
	{
		m_QdrList.push_back( Quadruple(cd_APARAM, NULL, NULL, eList->m_elist.at(i) ) );
	}
	m_QdrList.push_back( Quadruple(cd_CALL, entry, NULL, NULL) );
}

void Code::generateFormals(EntryList* eList)
{
	for(int i=0; i<eList->m_elist.size(); ++i)
	{
		m_QdrList.push_back( Quadruple(cd_FPARAM, NULL, NULL, eList->m_elist.at(i) ));
	}
}

void Code::generateVariables(EntryList* eList)
{
	for(int i=0; i<eList->m_elist.size(); ++i)
	{
		m_QdrList.push_back( Quadruple(cd_VAR, NULL, NULL, eList->m_elist.at(i) ));
	}
}

std::string Code::newLabel()
{
	std::stringstream os;
	os << m_labelCount;

	std::string ret = "lab" + os.str();
	
	m_labelCount++;
	return ret;
}
std::string Code::newTemp()
{
	std::stringstream os;
	os << m_tempCount;

	std::string ret = "t" + os.str();
	
	m_tempCount++;
	return ret;	
}
void Code::print()
{
	for(int i=0; i<m_QdrList.size(); ++i)
	{
		m_QdrList.at(i).print();
	}
}

CodeOp Code::getCodeOpFromOpCode( OpType code )
{
	switch( code )
	{
		case op_PLUS:
			return cd_ADD;
		case op_MINUS:
			return cd_SUB;
		case op_OR:
			return cd_OR;
		case op_MULT:
			return cd_MULT;
		case op_DIVIDE:
			return cd_DIVIDE;
		case op_AND:
			return cd_AND;
		case op_DIV:
			return cd_DIV;
		case op_MOD:
			return cd_MOD;
		case op_LT:
			return cd_LT;
		case op_GT:
			return cd_GT;
		case op_LE:
			return cd_LE;
		case op_GE:
			return cd_GE;
		case op_EQ:
			return cd_EQ;
		case op_NE:
			return cd_NE;
		case op_NONE:
			return cd_NOOP;
	}
}
