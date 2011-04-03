//#include "scanner.h"
//#include "token.h"
//#include "symtab.h"
#include "parser.h"
#include "code.h"

int main( int /* argc */, char** /* argv */ )
{
	//std::cout << "testing123...\n";

	Parser p = Parser(false);

	p.parse();

	Code* code = p.getCode();
	code->print();
	return 0;
}




