#include "PmergeMe.hpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>
#include <iterator>

// orthodox canonical
PmergeMe::PmergeMe(){

}

PmergeMe::PmergeMe(const PmergeMe &other){
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other){
	if (this != &other){
		_main = other._main;
		_pendV = other._pendV;
		_resultV = other._resultV;
		//NB: _mainL.first iterators still point into other._pendL after this
		//copy, not into the freshly copied _pendL below. Fine as long as
		//PmergeMe is only copied before parseInputList has populated it.
		_mainL = other._mainL;
		_pendL = other._pendL;
		_resultL = other._resultL;
		_jacobsthalSequence = other._jacobsthalSequence;
		_insertionOrder = other._insertionOrder;
	}
	return (*this);
}

PmergeMe::~PmergeMe(){

}

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
				_main.push_back(std::pair<unsigned int, int>((unsigned int)(i / 2), (int)value1));
				_pendV.push_back((int)value2);
			}
			else{
				_main.push_back(std::pair<unsigned int, int>((unsigned int)(i / 2), (int)value2));
				_pendV.push_back((int)value1);
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
			_pendV.push_back((int)value1);
		}
	}
	catch (std::exception &e){
		throw PmergeMe::BadInput();
	}
}

//same parsing logic as parseInputVector, but the losing value goes into
//_pendL first so its iterator can be captured immediately and stored
//alongside the winning value in _mainL
void PmergeMe::parseInputList(int argc, char **argv, int &Comparisons){
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
				_pendL.push_back((int)value2);
				std::list<int>::iterator pendIt = _pendL.end();
				--pendIt;
				_mainL.push_back(std::pair<std::list<int>::iterator, int>(pendIt, (int)value1));
			}
			else{
				_pendL.push_back((int)value1);
				std::list<int>::iterator pendIt = _pendL.end();
				--pendIt;
				_mainL.push_back(std::pair<std::list<int>::iterator, int>(pendIt, (int)value2));
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
			_pendL.push_back((int)value1);
		}
	}
	catch (std::exception &e){
		throw PmergeMe::BadInput();
	}
}

int PmergeMe::pendingOf(std::pair<unsigned int, int> MainElem){
	return _pendV[MainElem.first];
}

//_pendL has no random access, but MainElem.first already IS an iterator into
//_pendL (captured in parseInputList), so the lookup is a plain O(1) dereference
int PmergeMe::pendingOfL(std::pair<std::list<int>::iterator, int> MainElem){
	return *(MainElem.first);
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
	if (_pendV.size()>_main.size())
		newPend.push_back(_pendV[_main.size()]);
	_pendV = newPend;
}

void PmergeMe::reOrderPendL(){
	std::list<int> newPend;
	for (std::list< std::pair<std::list<int>::iterator, int> >::iterator it = _mainL.begin(); it != _mainL.end(); it++){
		newPend.push_back(pendingOfL(*it));
	}
	//if there where odd elements: it was pushed last during parsing, so it's
	//still the last element of the original _pendL
	if (_pendL.size() > _mainL.size())
		newPend.push_back(_pendL.back());
	_pendL = newPend;
}

void PmergeMe::insertPendIntoResult(int &Comparisons){
	//b0 first to be _resultV[0]
	_resultV.push_back(_pendV[0]);
	//insert whole main chain
	for (std::vector < std::pair<unsigned int, int> > ::iterator it = _main.begin(); it != _main.end(); it++){
		_resultV.push_back(it->second);
	}

	//track the CURRENT position of each original _main element
	std::vector<unsigned int> mainPos(_main.size());
	for (size_t i = 0; i < _main.size(); i++)
		mainPos[i] = i + 1;

	//figure insertion sequence out
	_jacobsthalSequence = createJacobsthal(_pendV.size());
	_insertionOrder = createInsertionOrder(_jacobsthalSequence, _pendV.size());


	//insert pend into result using BINARY insertion Sort
	int insertMax = _insertionOrder.size();
	int toInsert;
	unsigned int index;
	for (int i = 0; i < insertMax; i++){
		index = _insertionOrder[i];
		toInsert = _pendV[index];

		// if there is an originally unpaired element, it can fit anywhere
		// if it was paired before, it has an upper bound given by index
		unsigned int bound;
		if (index < mainPos.size())
			bound = mainPos[index];
		else
			bound = _resultV.size();
		
		unsigned int lo = 0;
		unsigned int hi = bound;
		while (lo < hi){
			unsigned int mid = lo + (hi - lo) / 2;
			if (_resultV[mid] < toInsert)
				lo = mid + 1;
			else
				hi = mid;
			Comparisons++;
		}

		_resultV.insert(_resultV.begin() + lo, toInsert);
		
		//keeping track of the current positions of original _main elements
		//everything at or after lo just shifted right by one
		for (size_t j = 0; j < mainPos.size(); j++)
			if (mainPos[j] >= lo)
				mainPos[j]++;
	}
	/*
	std::cout << "after insertion result: " << std::endl;
	for (size_t i = 0; i < _resultV.size(); i++){
		std::cout  << _resultV[i] << std::endl;
	}
	*/
}

//the real fork from insertPendIntoResult: mainPos tracks iterators into
//_resultL instead of indices. List insertion never invalidates other
//iterators, so those tracked positions stay correct for free after every
//insert - no "shift everything >= lo" loop needed.
//The binary search itself degrades to O(n) per step (advancing to the
//midpoint each time, since a list has no random access), but Comparisons
//only counts the value comparison, not the traversal, so the count stays
//identical to the vector run.
void PmergeMe::insertPendIntoResultL(int &Comparisons){
	//b0 first to be _resultL's first element
	_resultL.push_back(_pendL.front());
	//insert whole main chain
	for (std::list< std::pair<std::list<int>::iterator, int> >::iterator it = _mainL.begin(); it != _mainL.end(); it++){
		_resultL.push_back(it->second);
	}

	//track the CURRENT position of each original _mainL element
	std::vector<std::list<int>::iterator> mainPos(_mainL.size());
	std::list<int>::iterator posIt = _resultL.begin();
	++posIt; // skip b0
	for (size_t i = 0; i < _mainL.size(); i++){
		mainPos[i] = posIt;
		++posIt;
	}

	//figure insertion sequence out
	_jacobsthalSequence = createJacobsthal(_pendL.size());
	_insertionOrder = createInsertionOrder(_jacobsthalSequence, _pendL.size());

	//insert pend into result using BINARY insertion Sort
	int insertMax = _insertionOrder.size();
	int toInsert;
	unsigned int index;
	for (int i = 0; i < insertMax; i++){
		index = _insertionOrder[i];

		std::list<int>::iterator toInsertIt = _pendL.begin();
		std::advance(toInsertIt, index);
		toInsert = *toInsertIt;

		// if there is an originally unpaired element, it can fit anywhere
		// if it was paired before, it has an upper bound given by index
		std::list<int>::iterator bound;
		if (index < mainPos.size())
			bound = mainPos[index];
		else
			bound = _resultL.end();

		unsigned int lo = 0;
		unsigned int hi = std::distance(_resultL.begin(), bound);
		while (lo < hi){
			unsigned int mid = lo + (hi - lo) / 2;
			std::list<int>::iterator midIt = _resultL.begin();
			std::advance(midIt, mid);
			if (*midIt < toInsert)
				lo = mid + 1;
			else
				hi = mid;
			Comparisons++;
		}

		std::list<int>::iterator insertIt = _resultL.begin();
		std::advance(insertIt, lo);
		_resultL.insert(insertIt, toInsert);

		//no positions to fix up: every iterator in mainPos still points at
		//its original node regardless of where toInsert just landed
	}
}

void PmergeMe::runVector(int &Comparisons){
	_main = mergeSortVector(_main, Comparisons);
	reOrderPend();
	insertPendIntoResult(Comparisons);

	std::cout << "ordered with Vectors: ";
	for (size_t i = 0; i < _resultV.size(); i++){
		std::cout  << _resultV[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::runList(int &Comparisons){
	_mainL = mergeSortList(_mainL, Comparisons);
	reOrderPendL();
	insertPendIntoResultL(Comparisons);

	std::cout << "ordered with Lists: ";
	for (std::list<int>::iterator it = _resultL.begin(); it != _resultL.end(); it++){
		std::cout << *it << " ";
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
std::vector<std::pair <unsigned int, int> > PmergeMe::mergeVector(std::vector<std::pair<unsigned int, int> > Left, std::vector<std::pair <unsigned int, int> > Right, int &Comparisons){
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

//same comparison logic as mergeVector, but splice moves a node in O(1)
//instead of the O(n) shift that vector's erase(begin()) does - the actual
//win of using a list here
std::list<std::pair <std::list<int>::iterator, int> > PmergeMe::mergeList(std::list<std::pair<std::list<int>::iterator, int> > &Left, std::list<std::pair <std::list<int>::iterator, int> > &Right, int &Comparisons){
	std::list< std::pair<std::list<int>::iterator, int> > result;
	while (!Left.empty() && !Right.empty()){
		if (Left.front().second <= Right.front().second)
			result.splice(result.end(), Left, Left.begin());
		else
			result.splice(result.end(), Right, Right.begin());
		Comparisons++;
	}
	//one of the lists is empty now
	result.splice(result.end(), Left);
	result.splice(result.end(), Right);
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
std::vector<std::pair <unsigned int, int> > PmergeMe::mergeSortVector(std::vector<std::pair<unsigned int, int> >Input, int &Comparisons){
	if (Input.size() <= 1)
		return Input;
	
	size_t middle = Input.size() / 2;
	//endpoint in constructor is exclusive!
	std::vector<std::pair <unsigned int, int> > left(Input.begin(), Input.begin() + middle);
	std::vector<std::pair <unsigned int, int> > right(Input.begin() + middle, Input.end()); 
	
	left = mergeSortVector(left, Comparisons);
	right = mergeSortVector(right, Comparisons);

	return mergeVector(left, right, Comparisons);
}

/*
Input is taken by reference and consumed: the back half is spliced out into
right (O(1) per node, no copies), leaving Input holding just the left half,
which recurses in place. This is what makes the splice-based split in TODO
item 4 actually pay off - passing by value like mergeSortVector does would
force a full node-by-node copy on every recursive call.
*/
std::list<std::pair <std::list<int>::iterator, int> > PmergeMe::mergeSortList(std::list<std::pair<std::list<int>::iterator, int> > &Input, int &Comparisons){
	if (Input.size() <= 1)
		return Input;

	//tortoise/hare: find the middle in a single O(n) pass
	std::list< std::pair<std::list<int>::iterator, int> >::iterator slow = Input.begin();
	std::list< std::pair<std::list<int>::iterator, int> >::iterator fast = Input.begin();
	while (fast != Input.end()){
		++fast;
		if (fast == Input.end())
			break;
		++fast;
		++slow;
	}

	std::list< std::pair<std::list<int>::iterator, int> > right;
	right.splice(right.begin(), Input, slow, Input.end());
	//Input now holds only the left half

	std::list< std::pair<std::list<int>::iterator, int> > left = mergeSortList(Input, Comparisons);
	right = mergeSortList(right, Comparisons);

	return mergeList(left, right, Comparisons);
}

//STATIC HELPERS

//sum(ceil_log2(3 * k) - 2 for k in range(1, n + 1))
int PmergeMe::worstCaseComparisons(int nb){
	int sum = 0;
	for (int k = 1; k <= nb; k++){
		sum += ceil((log(3*k) / log(2)) - 2);
	}
	return sum;
}

void PmergeMe::printBefore(int argc, char **argv){
	std::cout << "before sorting: " ;
	for (int i = 1; i < argc; i++){
		std::cout << argv[i] << " ";
	}
	std::cout << std::endl;	
}