#include "scanner.h"
#include "token.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();
	
	Token *currTok;
	currTok = scanner.nextToken();
	
	while(currTok->getTokenCode() != tc_EOF )
	{
		std::cout << currTok->tokenCodeToString() << "\n";
		if( currTok->getDataType() != dt_KEYWORD || currTok->getDataType() != dt_NONE )
		{
	//		std::cout << currTok->getDataValue().lexeme << "\n";
		}
		else
		{
	//		std::cout << currTok->opCodeToString() << "\n";
		}

				std::cout << currTok->getDataValue().lexeme << "\n";
		currTok = scanner.nextToken();
	}
	return 0;
}




