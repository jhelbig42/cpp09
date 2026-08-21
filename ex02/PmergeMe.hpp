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

		void parseInputVector(int argc, char **argv, int &Comparisons);
		void runVector(int &Comparisons);

		void parseInputList(int argc, char **argv, int &Comparisons);
		void runList(int &Comparisons);

		class BadInput : public std::exception{
			public:
				virtual const char * what() const throw(){
					return ("Error: Bad Input");
				}
		};

		//static helpers
		static int worstCaseComparisons(int nb);
		static void printBefore(int argc, char **argv);

	private:
		//vector solution
		std::vector< std::pair<unsigned int, int> > _main;
		std::vector<int> _pendV;
		std::vector<int> _resultV;

		static std::vector<std::pair <unsigned int, int> > mergeSortVector(std::vector<std::pair<unsigned int, int> >Input, int &Comparisons);
		static std::vector<std::pair <unsigned int, int> > mergeVector(std::vector< std::pair<unsigned int, int> > Left, std::vector< std::pair<unsigned int, int> > Right, int &Comparisons);
		int pendingOf(std::pair<unsigned int, int> MainElem);
		void reOrderPend();
		void insertPendIntoResult(int &Comparisons);

		//list solution
		//pair.first is an iterator into _pendL rather than an index: a list
		//has no random access, so an index would need an O(n) advance on every
		//lookup anyway. An iterator into _pendL gives O(1) lookup instead, and
		//stays valid because _pendL is never mutated in place before it is used
		//(reOrderPendL builds a whole new list and only swaps it in at the end).
		std::list< std::pair<std::list<int>::iterator, int> > _mainL;
		std::list<int> _pendL;
		std::list<int> _resultL;

		static std::list<std::pair <std::list<int>::iterator, int> > mergeSortList(std::list<std::pair<std::list<int>::iterator, int> > &Input, int &Comparisons);
		static std::list<std::pair <std::list<int>::iterator, int> > mergeList(std::list<std::pair<std::list<int>::iterator, int> > &Left, std::list<std::pair<std::list<int>::iterator, int> > &Right, int &Comparisons);
		int pendingOfL(std::pair<std::list<int>::iterator, int> MainElem);
		void reOrderPendL();
		void insertPendIntoResultL(int &Comparisons);

		//helper
		std::vector<int> _jacobsthalSequence;
		std::vector<int> _insertionOrder;
};

#endif
