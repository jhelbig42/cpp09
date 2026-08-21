#include "PmergeMe.hpp"

// ./PmergeMe $(shuf -i 0-10000 -n 20)

int main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Error. Usage: ./PmergeMe [positive integers...]" << std::endl;
		return (1);
	}

	PmergeMe::printBefore(argc, argv);
	std::cout << "worst case comparisons: " << PmergeMe::worstCaseComparisons(argc - 1) << std::endl;

	try{
		PmergeMe sorter;
		clock_t t = clock();
		int comparisons = 0;

		sorter.parseInputVector(argc, argv, comparisons);
		sorter.runVector(comparisons);
		t = clock() - t;
		
		std::cout << "comparisons using vector: " << comparisons << std::endl;
		std::cout << "time it took with vectors: " << ((float)t)/CLOCKS_PER_SEC
			<< " seconds" <<std::endl;
		
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	// LIST implementation
	try{
		PmergeMe sorter;
		clock_t t = clock();
		int comparisons = 0;

		sorter.parseInputList(argc, argv, comparisons);
		sorter.runList(comparisons);
		t = clock() - t;

		std::cout << "comparisons using list: " << comparisons << std::endl;
		std::cout << "time it took with lists: " << ((float)t)/CLOCKS_PER_SEC
			<< " seconds" <<std::endl;

	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

