#include "scanner.h"
#include "token.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();
	
	Token *currTok;
	currTok = scanner.nextToken();
	
	while(currTok->getTokenCode() != tc_EOF )
	{
		std::cout << currTok->getTokenCode();
		currTok = scanner.nextToken();
	}
	return 0;
}




