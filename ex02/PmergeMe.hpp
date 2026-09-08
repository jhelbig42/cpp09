#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <ctime>
#include <math.h>
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <exception>

class PmergeMe{
	public:
		// orthodox canonical
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		class BadInput : public std::exception{
			public:
				virtual const char * what() const throw(){
					return ("Error: Bad Input");
				}
		};

		//static helpers
		static int worstCaseComparisons(int nb);

		std::vector<int> parseInputVector(int argc, char **argv);
		std::vector<int> sortVector(std::vector<int> Incoming, int &Comparisons);

	private:
		//vector solution
		std::vector<int> insertPendIntoResult(int &Comparisons, const std::vector<int>& Main, const std::vector<int>& Pend);

		//list solution
		

		//helper
		std::vector<int> _jacobsthalSequence;
		std::vector<int> _insertionOrder;
};

#endif
