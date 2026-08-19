#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <ctime>
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
		
		std::vector<int> _jacobsthalSequence;
		std::vector<int> _insertionOrder;
	
		static std::vector<std::pair <unsigned int, int> > mergeSortVector(std::vector<std::pair<unsigned int, int> >Input);
		static std::vector<std::pair <unsigned int, int> > mergeVector(std::vector< std::pair<unsigned int, int> > Left, std::vector< std::pair<unsigned int, int> > Right);
		int pendingOf(std::pair<unsigned int, int> MainElem);
		void reOrderPend();
		void insertPendIntoResult();
};

/*
std::ostream& operator<<(std::ostream& os, const T& obj)
{
    // write obj to stream
    return os;
}*/
/*
std::ostream& operator<<(std::ostream& Os, const std::vector<int> &Vect){
	size_t len = Vect.size();
	for (size_t i = 0; i < len; i++)
		Os << Vect[i] << " ";
	Os << std::endl;
	return Os;
}
	*/

#endif
