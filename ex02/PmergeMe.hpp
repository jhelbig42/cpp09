#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <exception>

class PmergeMe
{
	public:
		// Orthodox Canonical Form
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		class BadInput : public std::exception
		{
			public:
				virtual const char *what() const throw()
				{
					return ("Error: Bad Input");
				}
		};

		// Static helpers
		static int worstCaseComparisons(int nb);

		struct Element
		{
			int value;
			size_t id;
		};

		struct Pair
		{
			PmergeMe::Element main;
			PmergeMe::Element pend;
		};

		//vector
		std::vector<int> parseInputVector(int argc, char **argv);
		std::vector<int> sortVector(std::vector<int> Incoming, int &Comparisons);

	private:
		std::vector<PmergeMe::Element> sortRecursive(const std::vector<PmergeMe::Element>& Incoming, int &Comparisons);
		std::vector<PmergeMe::Element> insertPendIntoResult(int &Comparisons, const std::vector<PmergeMe::Element>& Main, const std::vector<PmergeMe::Element>& Pend);

		// Helper
		std::vector<int> _jacobsthalSequence;
		std::vector<int> _insertionOrder;
};

#endif