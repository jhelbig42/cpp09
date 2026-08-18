#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <exception>

class PmergeMe{
	public:
		PmergeMe();
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();

		void parseInput(int argc, char **argv);
		void run();

		class BadInput : public std::exception{
			public:
				virtual const char * what() const throw(){
					return ("Error: Bad Input");
				}
		};

		// dev helper
		void printMain();
		void printPend();
		void printChains();

	private:
		std::vector<std::pair <unsigned int, int> > _main;
		std::vector<int> _pend;


		static std::vector<std::pair<int, int> > mergeInsertionSort(std::vector<std::pair<int, int> > elems, size_t n, size_t &comparisons);

		static std::vector<int> sortVector(const std::vector<int> &input, size_t &comparisons);
		static std::list<int> sortList(const std::list<int> &input, size_t &comparisons);

		static void printSequence(const char *label, const std::vector<int> &seq);
};

#endif
