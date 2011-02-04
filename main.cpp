#include "scanner.h"
#include "token.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();
	
	Token *currTok;
	currTok = scanner.nextToken();
	
	while(currTok->getTokenCode() != tc_EOF )
	{
					
		std::cout << currTok->tokenCodeToString() << "";
		if( currTok->getDataType() != dt_KEYWORD && currTok->getDataType() != dt_NONE )
		{
			std::cout << "(" << currTok->getDataValue().lexeme << ")";
		}

		currTok = scanner.nextToken();
		std::cout << " ";
	}
	
	std::cout << "\n\n";
	return 0;
}




