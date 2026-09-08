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

//_main saves the vector index of the corresponding _pend element for later access
std::vector<int> PmergeMe::parseInputVector(int argc, char **argv){
	int i;
	long value1;
	std::stringstream input;
	std::vector<int> start;
	i = 1;
	try{
		while (i < argc){
			input.str(argv[i]);
			input >> value1;
			if (input.fail() || !input.eof())
				throw PmergeMe::BadInput();
			checkInputValue(value1);
			input.clear();
			start.push_back(value1);
			i++;
		}
	}
	catch (std::exception &e){
		throw PmergeMe::BadInput();
	}
	return (start);
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


std::vector<int> PmergeMe::insertPendIntoResult(int &Comparisons, const std::vector<int>& Main, const std::vector<int>& Pend) {
    
    if (Pend.empty()) 
		return Main;

	std::vector<int> result;

    // create base result, Main is ordered, Pend[0] is smaller than Main[0] by pairing
    result.push_back(Pend[0]);
    for (size_t i = 0; i < Main.size(); i++)
        result.push_back(Main[i]);

    // Track the CURRENT index position of each original main element in 'result'
    std::vector<unsigned int> mainPos(Main.size());
    for (size_t i = 0; i < Main.size(); i++)
        mainPos[i] = i + 1; // Shifted by 1 because Pend[0] was prepended

    // Generate Jacobsthal insertion order for remaining pend elements
    std::vector<int> jacobsthalSeq = createJacobsthal(Pend.size());
    std::vector<int> insertionOrder = createInsertionOrder(jacobsthalSeq, Pend.size());

    // Binary insert each element from Pend into 'result'
    for (size_t i = 0; i < insertionOrder.size(); i++) {
        size_t index = insertionOrder[i];
        if (index == 0) 
			continue; // Pend[0] is already placed

        int toInsert = Pend[index];

        // Upper bound: If index exists in mainPos, we only search up to its paired main element
        unsigned int bound;
        if (index < mainPos.size())
            bound = mainPos[index];
        else
            bound = result.size();

        // Binary search within [0, bound)
        unsigned int lo = 0;
        unsigned int hi = bound;
        while (lo < hi) {
            unsigned int mid = lo + (hi - lo) / 2;
            Comparisons++;
            if (result[mid] < toInsert)
                lo = mid + 1;
            else
                hi = mid;
        }

        result.insert(result.begin() + lo, toInsert);

        // Update tracking positions for Main elements shifted by the insertion
        for (size_t j = 0; j < mainPos.size(); j++) {
            if (mainPos[j] >= lo)
                mainPos[j]++;
        }
    }
    return result;
}

std::vector<int> PmergeMe::sortVector(std::vector<int> Incoming, int &Comparisons) {
    uint inLen = Incoming.size();
    if (inLen <= 1)
        return Incoming;

    std::vector<std::pair<int, int> > pairs;
    std::vector<int> main_unsorted;

    // group into pairs, find the larger element, and prepare the unsorted main chain
    for (size_t i = 1; i < inLen; i += 2) {
        if (Incoming[i - 1] > Incoming[i]) {
            pairs.push_back(std::make_pair(Incoming[i - 1], Incoming[i]));
            main_unsorted.push_back(Incoming[i - 1]);
        } else {
            pairs.push_back(std::make_pair(Incoming[i], Incoming[i - 1]));
            main_unsorted.push_back(Incoming[i]);
        }
		Comparisons++;
    }

    // Recursively sort the main
    std::vector<int> main = sortVector(main_unsorted, Comparisons);

    // Reorder the pend chain to match the newly sorted main chain
    std::vector<int> pend;
    std::vector<bool> used_pairs(pairs.size(), false);

    for (size_t i = 0; i < main.size(); i++) {
        for (size_t j = 0; j < pairs.size(); j++) {
            if (!used_pairs[j] && pairs[j].first == main[i]) {
                pend.push_back(pairs[j].second);
                used_pairs[j] = true;
                break;
            }
        }
    }

    // Handle an odd leftover element by appending it to the pend chain
    if (inLen % 2 != 0) {
        pend.push_back(Incoming[inLen - 1]);
    }

    // Insert Pend into Main following Jacobsthal order
    return insertPendIntoResult(Comparisons, main, pend);
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
