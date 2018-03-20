#include "Parser.hpp"
#include "Linker.hpp"

int main()
{
	hbs::Parser p("index.hbs");
	hbs::Linker l(p);


	system("pause");
	return 0;
}
