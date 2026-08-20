#include "PmergeMe.hpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>

// orthodox canonical
PmergeMe::PmergeMe(){

}

PmergeMe::PmergeMe(const PmergeMe &other){
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	if (this != &other){
		_chain = other._chain;
	}
	return (*this);
}

PmergeMe::~PmergeMe(){

}

// 

// handling task with VECTOR


//input is supposed to be positive int
static void checkInputValue (long value){
	if (value >= INT_MAX || value < 0)
		throw PmergeMe::BadInput();
}

void PmergeMe::parseInputVector(int argc, char **argv, int &Comparisons){
	int i;
	long value1;
	long value2;
	std::stringstream input;
	i = 1;
	try{
		while (i < argc - 1){
			input.str(argv[i]);
			input >> value1;
			if (input.fail() || !input.eof())
				throw PmergeMe::BadInput();
			checkInputValue(value1);
			input.clear();

			input.str(argv[i + 1]);
			input >> value2;
			if (input.fail() || !input.eof())
				throw PmergeMe::BadInput();
			checkInputValue(value2);
			input.clear();

			if (value1 > value2){
				Elements newElem = {0, (int)value1, (int)value2};
				_chain.push_back(newElem);
			}
			else{
				Elements newElem = {0, (int)value2, (int)value1};
				_chain.push_back(newElem);
			}
			Comparisons++;
			i = i + 2;
		}
		if (argc % 2 == 0){ // odd number of argument
			input.str(argv[argc - 1]);
			input >> value1;
			if (input.fail() || !input.eof())
				throw PmergeMe::BadInput();
			checkInputValue(value1);
			Elements newElem = {0, -1, (int)value2};
			_chain.push_back(newElem);
		}
	}
	catch (std::exception &e){
		throw PmergeMe::BadInput();
	}
}

/*
sequence starts with 0 and 1, then each following number is found by 
adding the number before it to twice the number before that
    0, 1, 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, ...
*/
/*
static std::vector<int> createJacobsthal(size_t size){
	std::vector<int> jacobsthal;

	jacobsthal.push_back(0);
	jacobsthal.push_back(1);
	while (jacobsthal.back() < (int)size){
		size_t last = jacobsthal.size();
		jacobsthal.push_back(jacobsthal[last - 1] + 2 * jacobsthal[last - 2]);
	}
	return jacobsthal;
}
*/

/*
turns the Jacobsthal numbers into the order in which the pending elements
should be inserted into the main chain
*/


static std::vector<int> createInsertionOrder(std::vector<int> const &jacobsthal, size_t pendSize){
	std::vector<int> order;

	for (size_t k = 2; k < jacobsthal.size(); k++){
		int lower = jacobsthal[k - 1];
		int upper = jacobsthal[k];
		if ((size_t)lower >= pendSize)
			break;
		if ((size_t)upper > pendSize)
			upper = (int)pendSize;
		for (int rank = upper; rank > lower; rank--)
			order.push_back(rank - 1);
	}
	return order;
}


void PmergeMe::insertPendVector(int &Comparisons){
	//index chain
	size_t vlen = _chain.size();
	for (size_t i = 0; i < vlen; i++){
		_chain[i].idx = i;
	}

	//b0 first
	Elements newElem(0, _chain[0].pend, -1);
	_chain.push_front(_pend[0]);

	//figure insertion sequence out
	_jacobsthalSequence = createJacobsthal(vlen);
	_insertionOrder = createInsertionOrder(_jacobsthalSequence, vlen);

	//insert pend into result
	int insertMax = _insertionOrder.size();
	for (int i = 0; i < insertMax; i++){
		int toInsert = _pend[_insertionOrder[i]];
		std::vector<int>::iterator it = _result.begin();
		//find correct place within main to insert the element
		while (it != _result.end() && *it < toInsert){
			Comparisons++;
			it++;
		}
		Comparisons++;	
		_result.insert(it, toInsert);
	}
}


void PmergeMe::runVector(int &Comparisons){
	_chain = mergeSortVector(_chain, Comparisons);

	std::cout << "(main chain) ";
	for (size_t i = 0; i < _chain.size(); i++){
		std::cout  << _chain[i].main << " ";
	}
	std::cout << std::endl;

	std::cout << "(pend chain) ";
	for (size_t i = 0; i < _chain.size(); i++){
		std::cout  << _chain[i].pend << " ";
	}
	std::cout << std::endl;

	insertPendVector(Comparisons);

	std::cout << "ordered with Vectors: (main chain) ";
	for (size_t i = 0; i < _chain.size(); i++){
		std::cout  << _chain[i].main << " ";
	}
	std::cout << std::endl;
}

/*
funktion merge(linkeListe, rechteListe);
  neueListe
  solange (linkeListe und rechteListe nicht leer)
       falls (erstes Element der linkeListe <= erstes Element der rechteListe)
       dann füge erstes Element linkeListe in die neueListe hinten ein und entferne es aus linkeListe
       sonst füge erstes Element rechteListe in die neueListe hinten ein und entferne es aus rechteListe
  solange_ende
  solange (linkeListe nicht leer)
       füge erstes Element linkeListe in die neueListe hinten ein und entferne es aus linkeListe
  solange_ende
  solange (rechteListe nicht leer)
       füge erstes Element rechteListe in die neueListe hinten ein und entferne es aus rechteListe
  solange_ende
  antworte neueListe
*/
std::vector<PmergeMe::Elements> PmergeMe::mergeVector(std::vector<Elements> Left, std::vector<Elements> Right, int &Comparisons){
	std::vector< Elements > result;
	while (!Left.empty() && !Right.empty()){
		if (Left.front().main <= Right.front().main){
			result.push_back(Left.front());
			Left.erase(Left.begin());
		}
		else{
			result.push_back(Right.front());
			Right.erase(Right.begin());
		}
		Comparisons++;
	}
	//one of the lists is empty now
	while (!Left.empty()){
		result.push_back(Left.front());
		Left.erase(Left.begin());
	}
	while (!Right.empty()){
		result.push_back(Right.front());
		Right.erase(Right.begin());
	}
	return result;
}

/*
funktion mergesort(liste);
  falls (Größe von liste <= 1) dann antworte liste
  sonst
     halbiere die liste in linkeListe, rechteListe
     linkeListe = mergesort(linkeListe)
     rechteListe = mergesort(rechteListe)
     antworte merge(linkeListe, rechteListe)
*/
std::vector<PmergeMe::Elements> PmergeMe::mergeSortVector(std::vector<PmergeMe::Elements>Input, int &Comparisons){
	if (Input.size() <= 1)
		return Input;
	
	size_t middle = Input.size() / 2;
	//endpoint in constructor is exclusive!
	std::vector< Elements > left(Input.begin(), Input.begin() + middle);
	std::vector< Elements > right(Input.begin() + middle, Input.end()); 
	
	left = mergeSortVector(left, Comparisons);
	right = mergeSortVector(right, Comparisons);

	return mergeVector(left, right, Comparisons);
}

