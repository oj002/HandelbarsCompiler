#include "Compiler.hpp"
#include <chrono>

namespace ch  = std::chrono;


int main()
{
	ch::high_resolution_clock::time_point t1 = ch::high_resolution_clock::now();

	hbs::Parser p("index.hbs");
	hbs::Linker l(p);
	hbs::Compiler c(l);
	

	ch::high_resolution_clock::time_point t2 = ch::high_resolution_clock::now();

	auto duration = ch::duration_cast<ch::microseconds>(t2 - t1).count();

	std::cout << duration << " ms" << std::endl;
	system("pause");
	return 0;
}
