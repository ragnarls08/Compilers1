#include "scanner.h"
#include "token.h"
#include "symtab.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();
	
	SymbolTable st = SymbolTable();
	
	Token *currTok;
	currTok = scanner.nextToken();
	
	while( 1 )
	{					
		std::cout << currTok->tokenCodeToString();
		
		if( currTok->getDataType() == dt_OP )
			std::cout << "(" << currTok->opCodeToString() << ")";
		else if( currTok->getDataType() != dt_KEYWORD && currTok->getDataType() != dt_NONE )
		{
			std::cout << "(" << currTok->getDataValue().lexeme << ")";
		}	
		std::cout << " ";
		
		
		//symtab
		if( currTok->getTokenCode() == tc_ID || currTok->getTokenCode() == tc_NUMBER )
		{
			SymbolTableEntry *entry = st.lookup( currTok->getDataValue().lexeme );
			
			if(!entry)
			{
				entry = st.insert( currTok->getDataValue().lexeme );
				currTok->setSymTabEntry( entry );
			}
			currTok->setSymTabEntry( entry );	
		}
		
		
		if(currTok->getTokenCode() == tc_EOF)
			break;
		currTok = scanner.nextToken();
	}
	
	std::cout << "\n\n";
	st.print();
	return 0;
}




