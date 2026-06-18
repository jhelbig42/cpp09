#include "RPN.hpp"

int main(int argc, char** argv){
	if (argc != 2){
		std::cerr << "Error. Usage: ./RPN [calculation string]";
		return 1;
	}

	RPN calculation;

	try{
		std::cout << calculation.calculate(argv[1]) << std::endl;
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
