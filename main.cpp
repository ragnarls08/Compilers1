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
	int totalErr = p.totalErrors();

    if( totalErr == 0){
        Code* code = p.getCode();
        code->print();
    }
    else{

        std::cout << "Total number of errors: " << totalErr << "\n";
    }
	return 0;
}




