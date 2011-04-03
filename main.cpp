#include "parser.h"
#include "code.h"

int main( int /* argc */, char** /* argv */ )
{
	Parser p = Parser(false);

	p.parse();
	int totalErr = p.totalErrors();

    if( totalErr == 0){
        Code* code = p.getCode();
        code->print();
    }
    else{

        std::cout << "\nTotal number of errors: " << totalErr << "\n";
    }
	return 0;
}




