// **************************************************************
// *															*
// *	S O U R C E L I N E			(Header)					*
// *															*
// *	CLASSES:	SourceLine									*
// *															*
// **************************************************************

#ifndef	sourceline_h
#define	sourceline_h

#define MAX_ERRORS_IN_LINE 10


class LineError
{
private:						/* A class consisting of			*/
  int  m_column;				/* the column number of the error   */
  char* m_errorMsg;             /* and the associated error message */
public:
	int getColumn();
	char* getError();
	void set(int col, char* err);	/* Set column number and error		*/
};


class SourceLine {

private:
	char*	m_line;				// Characters in current line 
	int		m_firstLexemeColumn;	// First column number of current lexeme
	int		m_lastLexemeColumn;	// Last column number of current lexeme
	int		m_lineNo;			// Line number in original source
	int		m_lineErrors;		// Number of errors in current line
	int		m_maxLen;			// Maximum length of line
	LineError m_errors[MAX_ERRORS_IN_LINE];

	void clearLine();			// clears the line
	void printErrors();			// prints error messages in current line
	
public:
	SourceLine(int size);

	void buildLine(const char* str);// builds the line
	void printLine();				// prints the line and error messages in line if any
	void setError(char* msg);		// sets an error message
	void newLine();					// announces a new line in the input
};

#endif
