#include "scanner.h"
#include "token.h"

int main( int /* argc */, char** /* argv */ )
{
	Scanner scanner = Scanner();

	while( scanner.nextToken()->getTokenCode() != tc_EOF )
		;
	return 0;
}




