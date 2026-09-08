#include "PmergeMe.hpp"

// ./PmergeMe $(shuf -i 0-10000 -n 20)

int main(int argc, char **argv){
	if (argc < 2){
		std::cerr << "Error. Usage: ./PmergeMe [positive integers...]" << std::endl;
		return (1);
	}

	std::cout << "worst case comparisons: " << PmergeMe::worstCaseComparisons(argc - 1) << std::endl;

	try{
		PmergeMe sorter;
		clock_t t = clock();
		int comparisons = 0;

		std::vector<int> result;
		result = sorter.parseInputVector(argc, argv);
		
		std::cout << "before Ordering: ";
		for (size_t i = 0; i < result.size(); i++){
			std::cout  << result[i] << " ";
		}
		std::cout << std::endl;
		
		result = sorter.sortVector(result, comparisons);

		t = clock() - t;
		
		std::cout << "after Ordering: ";
		for (size_t i = 0; i < result.size(); i++){
			std::cout  << result[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "comparisons using vector: " << comparisons << std::endl;
		std::cout << "time it took with vectors: " << ((float)t)/CLOCKS_PER_SEC
			<< " seconds" <<std::endl;
		
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return (1);
	}
	// LIST implementation

	return (0);
}

