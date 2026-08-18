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

		void parseInputVector(int argc, char **argv);
		void runVector();

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
		std::vector< std::pair<unsigned int, int> > _main;
		std::vector<int> _pend;
		std::vector<int> _result;

		static std::vector<std::pair <unsigned int, int> > mergeSortVector(std::vector<std::pair<unsigned int, int> >Input);
		static std::vector<std::pair <unsigned int, int> > mergeVector(std::vector< std::pair<unsigned int, int> > Left, std::vector< std::pair<unsigned int, int> > Right);
		int pendingOf(std::pair<unsigned int, int> MainElem);
};

#endif
