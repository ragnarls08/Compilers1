#include "sourceline.h"
#include <iostream>
#include <math.h>

int LineError::getColumn()
{
    return m_column;
}

char* LineError::getError()
{
    return m_errorMsg;
}

void LineError::set(int col, char* err)
{
    m_column = col;
    m_errorMsg = err;
}

SourceLine::SourceLine(int size)
{
  
  m_line = new std::string();
    m_firstLexemeColumn = 0;
    m_lastLexemeColumn = 0;
    m_lineNo = 1;
    m_lineErrors = 0;
    m_maxLen = size;


}

void SourceLine::clearLine()
{
    m_line->clear();
    m_lineErrors = 0;
    m_firstLexemeColumn = 0;
    m_lastLexemeColumn = 0;

}

void SourceLine::printErrors()
{
    for(int i = 0; i<m_lineErrors; ++i)
    {
		//length of the line number + 2 for ": " 
		int numLen = ceil( log10( m_lineNo ) );
		numLen += 2;
		std::string str(m_errors[i].getColumn() + numLen, ' ');
		

		std::cout << str;
        std::cout << "^" << m_errors[i].getError() << '\n';
    }

}
void SourceLine::buildLine(const char* str) // builds the line
{
	m_firstLexemeColumn = m_line->size();
	m_line->append(str);	
	m_lastLexemeColumn = m_line->size();

}
void SourceLine::printLine() // prints the line and error messages in line if any
{
	std::cout << m_lineNo << ": ";
    std::cout << *m_line << "\n";
    printErrors();
	std::cout.flush();
}

void SourceLine::setError(char* msg)		// sets an error message
{
	if( m_lineErrors < MAX_ERRORS_IN_LINE )
	{
		m_errors[m_lineErrors].set(m_firstLexemeColumn, msg);	
    	++m_lineErrors;
	}
}

void SourceLine::newLine()
{
	printLine();
    ++m_lineNo;
    clearLine();
}
