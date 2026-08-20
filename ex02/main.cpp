#include "PmergeMe.hpp"

void printBefore(int argc, char **argv){
	std::cout << "before sorting: " ;
	for (int i = 1; i < argc; i++){
		std::cout << argv[i] << " ";
	}
	std::cout << std::endl;	
}

int main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Error. Usage: ./PmergeMe [positive integers...]" << std::endl;
		return (1);
	}

	PmergeMe sorter;

	printBefore(argc, argv);

	try{
		clock_t t = clock();
		int comparisons = 0;

		sorter.parseInputVector(argc, argv, comparisons);
		sorter.runVector(comparisons);
		t = clock() - t;

		std::cout << "comparisons: " << comparisons << std::endl;
		std::cout << "time it took with vectors: " << ((float)t)/CLOCKS_PER_SEC
			<< " seconds" <<std::endl;
		
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

