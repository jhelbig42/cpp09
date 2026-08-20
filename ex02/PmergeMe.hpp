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

		struct Elements{
			int idx;
			int main;
			int pend;
		};

		void parseInputVector(int argc, char **argv, int &Comparisons);
		void runVector(int &Comparisons);

		class BadInput : public std::exception{
			public:
				virtual const char * what() const throw(){
					return ("Error: Bad Input");
				}
		};

	private:
		std::vector< Elements > _chain;
		
		std::vector<int> _jacobsthalSequence;
		std::vector<int> _insertionOrder;
	
		static std::vector<Elements> mergeSortVector(std::vector< Elements >Input, int &Comparisons);
		static std::vector<Elements> mergeVector(std::vector< Elements > Left, std::vector< Elements > Right, int &Comparisons);
		void insertPendVector(int &Comparisons);
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
