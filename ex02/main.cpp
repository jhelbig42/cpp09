#include "PmergeMe.hpp"

int main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Error. Usage: ./PmergeMe [positive integers...]" << std::endl;
		return (1);
	}

	PmergeMe sorter;

	try{
		sorter.parseInput(argc, argv);
		sorter.printMain();
		sorter.printPend();
		sorter.sort();
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

void mergeSortMain(){
	
}

void PmergeMe::run(){

}
