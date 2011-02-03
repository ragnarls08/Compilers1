#include "scanner.h"
#include "token.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();
	
	Token *currTok;
	currTok = scanner.nextToken();
	
	while(currTok->getTokenCode() != tc_EOF )
	{
		std::cout << "Main: code = " << currTok->getTokenCode() << "\n";
		currTok = scanner.nextToken();
	}
	return 0;
}




