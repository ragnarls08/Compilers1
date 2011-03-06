#include "sourceline.h"
#include <iostream>
#include <string.h>

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
    m_maxLen = size;
    m_lineNo = 1;
    m_firstLexemeColumn = 0;
    m_lastLexemeColumn = 0;
}

void SourceLine::clearLine()
{
    m_line[0] = '\0';
    m_lineErrors = 0;
    m_firstLexemeColumn = 0;
    m_lastLexemeColumn = 0;
}

void SourceLine::printErrors()
{
    // ekki klarad, tharf ad indenta med whitespaceum
    for(int i = 0; i<m_lineErrors; ++i)
    {
        for (int a = 0; i<m_errors[i].getColumn(); ++a)
        {
            std::cout << " ";
        }
        std::cout << "^" << m_errors[i].getError() << '\n';
    }
}
void SourceLine::buildLine(const char* str) // builds the line
{
    m_firstLexemeColumn = m_lastLexemeColumn;
    strcat(m_line,str);
    m_lastLexemeColumn += strlen(str);
}
void SourceLine::printLine() // prints the line and error messages in line if any
{
    std::cout << *m_line << '\n';
    printErrors();
}

void SourceLine::setError(char* msg)		// sets an error message
{
    m_errors[m_lineErrors].set(m_firstLexemeColumn, msg);
    ++m_lineErrors;
}

void SourceLine::newLine()
{
    ++m_lineNo;
    clearLine();
}
