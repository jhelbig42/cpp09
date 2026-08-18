#include "PmergeMe.hpp"
#include <algorithm>
#include <string>
#include <sstream>
#include <cstdlib>
#include <climits>
#include <cctype>
#include <cerrno>

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

static void checkInputValue (long value){
	if (value >= INT_MAX || value <= INT_MIN)
		throw PmergeMe::BadInput();
}

void PmergeMe::parseInput(int argc, char **argv){
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

