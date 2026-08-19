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
		_main = other._main;
		_pend = other._pend;
	}
	return (*this);
}

PmergeMe::~PmergeMe(){

}

// 

// handling task with VECTOR
void PmergeMe::printMain(){
	std::cout << "main chain: " << std::endl;
	for (size_t i = 0; i < _main.size(); i++){
		std::cout << "index: " << _main[i].first << " value: " << _main[i].second << std:: endl;
	}
}

void PmergeMe::printPend(){
	std::cout << "pend chain: " << std::endl;
	for (size_t i = 0; i < _pend.size(); i++){
		std::cout << "value: " << _pend[i] << std::endl;
	}
}

void PmergeMe::printChains(){
	printMain();
	printPend();
}

//input is supposed to be positive int
static void checkInputValue (long value){
	if (value >= INT_MAX || value < 0)
		throw PmergeMe::BadInput();
}

void PmergeMe::parseInputVector(int argc, char **argv){
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
				_main.push_back(std::pair<unsigned int, int>((unsigned int)(i / 2), (int)value1));
				_pend.push_back((int)value2);
			}
			else{
				_main.push_back(std::pair<unsigned int, int>((unsigned int)(i / 2), (int)value2));
				_pend.push_back((int)value1);
			}
			i = i + 2;
		}
		if (argc % 2 == 0){ // odd number of argument
			input.str(argv[argc - 1]);
			input >> value1;
			if (input.fail() || !input.eof())
				throw PmergeMe::BadInput();
			checkInputValue(value1);
			_pend.push_back((int)value1);
		}
	}
	catch (std::exception &e){
		throw PmergeMe::BadInput();
	}
}

int PmergeMe::pendingOf(std::pair<unsigned int, int> MainElem){
	return _pend[MainElem.first];
}

/*
sequence starts with 0 and 1, then each following number is found by 
adding the number before it to twice the number before that
    0, 1, 1, 3, 5, 11, 21, 43, 85, 171, 341, 683, 1365, 2731, 5461, 10923, ...
*/
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

void PmergeMe::reOrderPend(){
	std::vector<int> newPend;
	for (size_t i = 0; i < _main.size(); i++){
		newPend.push_back(pendingOf(_main[i]));
	}
	//if there where odd elements
	if (_pend.size()>_main.size())
		newPend.push_back(_pend[_main.size()]);
	_pend = newPend;
}

void PmergeMe::insertPendIntoResult(){
	//b0 first
	_result.push_back(_pend[0]);
	//insert whole main chain
	for (std::vector < std::pair<unsigned int, int> > ::iterator it = _main.begin() + 1; it != _main.end(); it++){
		_result.push_back(it->second);
	}
	//figure insertion sequence out
	_jacobsthalSequence = createJacobsthal(_pend.size());
	_insertionOrder = createInsertionOrder(_jacobsthalSequence, _pend.size());

	/*
	std::cout << "insertion order: " << std::endl;
	for (size_t i = 0; i < _insertionOrder.size(); i++){
		std::cout  << _insertionOrder[i] << std::endl;
	}
	*/
	//insert pend into result
	int insertMax = _insertionOrder.size();
	for (int i = 0; i < insertMax; i++){
		int toInsert = _pend[_insertionOrder[i]];
		std::vector<int>::iterator it = _result.begin();
		//find correct place within main to insert the element
		while (it != _result.end() && *it < toInsert)
			it++;
		_result.insert(it, toInsert);
	}
	/*
	std::cout << "after insertion result: " << std::endl;
	for (size_t i = 0; i < _result.size(); i++){
		std::cout  << _result[i] << std::endl;
	}
	*/
}

void PmergeMe::runVector(){
	_main = mergeSortVector(_main);
	//reorder _pend
	reOrderPend();
	insertPendIntoResult();

	std::cout << "ordered with Vectors: ";
	for (size_t i = 0; i < _result.size(); i++){
		std::cout  << _result[i] << " ";
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
std::vector<std::pair <unsigned int, int> > PmergeMe::mergeVector(std::vector<std::pair<unsigned int, int> > Left, std::vector<std::pair <unsigned int, int> > Right){
	std::vector< std::pair<unsigned int, int> > result;
	while (!Left.empty() && !Right.empty()){
		if (Left.front().second <= Right.front().second){
			result.push_back(Left.front());
			Left.erase(Left.begin());
		}
		else{
			result.push_back(Right.front());
			Right.erase(Right.begin());
		}
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
std::vector<std::pair <unsigned int, int> > PmergeMe::mergeSortVector(std::vector<std::pair<unsigned int, int> >Input){
	if (Input.size() <= 1)
		return Input;
	
	size_t middle = Input.size() / 2;
	//endpoint in constructor is exclusive!
	std::vector<std::pair <unsigned int, int> > left(Input.begin(), Input.begin() + middle);
	std::vector<std::pair <unsigned int, int> > right(Input.begin() + middle, Input.end()); 
	
	left = mergeSortVector(left);
	right = mergeSortVector(right);

	return mergeVector(left, right);
}

