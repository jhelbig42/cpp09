#include "PmergeMe.hpp"

int main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Error. Usage: ./PmergeMe [positive integers...]" << std::endl;
		return (1);
	}

	PmergeMe sorter;

	try{
		//so
		sorter.parseInputVector(argc, argv);
		sorter.printChains();
		sorter.runVector();
		sorter.printChains();
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

