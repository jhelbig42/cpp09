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

std::vector<PmergeMe::Element> PmergeMe::insertPendIntoResult(int &Comparisons, const std::vector<PmergeMe::Element>& Main, const std::vector<PmergeMe::Element>& Pend)
{
    if (Pend.empty())
        return Main;

    std::vector<PmergeMe::Element> result;

    // Pend[0] is the smaller element of the first pair
    result.push_back(Pend[0]);

	//creating the base main chain
    for (size_t i = 0; i < Main.size(); i++)
        result.push_back(Main[i]);

    // Track the CURRENT position of each Main element in result.
    std::vector<unsigned int> mainPos(Main.size());
    for (size_t i = 0; i < Main.size(); i++)
        mainPos[i] = i + 1;

    // Generate Jacobsthal insertion order.
    std::vector<int> jacobsthalSeq = createJacobsthal(Pend.size());

    std::vector<int> insertionOrder = createInsertionOrder(jacobsthalSeq, Pend.size());

    // Insert each Pend element according to Jacobsthal order.
    for (size_t i = 0; i < insertionOrder.size(); i++)
    {
        size_t index = insertionOrder[i];

        // Pend[0] was already inserted.
        if (index == 0)
            continue;

        PmergeMe::Element toInsert = Pend[index];

        // only search to insertion Position up to the current position of the corresponding main chain element
		// the odd element could be anywhere
        unsigned int bound;

        if (index < mainPos.size())
            bound = mainPos[index];
        else
            bound = result.size();

        unsigned int lo = 0;
        unsigned int hi = bound;

        // Binary search.
        while (lo < hi)
        {
            unsigned int mid = lo + (hi - lo) / 2;
            Comparisons++;
            if (result[mid].value < toInsert.value)
                lo = mid + 1;
            else
                hi = mid;
        }

        result.insert(result.begin() + lo, toInsert);

        // Update positions of Main elements 
        for (size_t j = 0; j < mainPos.size(); j++)
        {
            if (mainPos[j] >= lo)
                mainPos[j]++;
        }
    }

    return result;
}


std::vector<PmergeMe::Element> PmergeMe::sortRecursive(const std::vector<PmergeMe::Element>& Incoming, int &Comparisons){
    size_t inLen = Incoming.size();

    if (inLen <= 1)
        return Incoming;

    std::vector<Pair> pairs;
    std::vector<PmergeMe::Element> main_unsorted;

	//create sorted pairs
    for (size_t i = 1; i < inLen; i += 2){
        Pair p;

		Comparisons++;
        if (Incoming[i - 1].value > Incoming[i].value){
            p.main = Incoming[i - 1];
            p.pend = Incoming[i];
        }
        else{
            p.main = Incoming[i];
            p.pend = Incoming[i - 1];
        }
        pairs.push_back(p);

        // larger elements need to be recursively sorted
        main_unsorted.push_back(p.main);
    }

    std::vector<PmergeMe::Element> main = sortRecursive(main_unsorted, Comparisons);

	//build pend chain, order depends on sorted main chain elements
    std::vector<PmergeMe::Element> pend;

    for (size_t i = 0; i < main.size(); i++){
        for (size_t j = 0; j < pairs.size(); j++){
            if (main[i].id == pairs[j].main.id){ // does just compare ids, not values
                pend.push_back(pairs[j].pend);
                break;
            }
        }
    }

    // add odd element to pend
    if (inLen % 2 != 0)
        pend.push_back(Incoming[inLen - 1]);


    return insertPendIntoResult(Comparisons, main, pend);
}


std::vector<int> PmergeMe::sortVector(std::vector<int> Incoming, int &Comparisons)
{
	//give ids to values - just happening once at the start
    std::vector<PmergeMe::Element> elements;

    for (size_t i = 0; i < Incoming.size(); i++){
        PmergeMe::Element e;
        e.value = Incoming[i];
        e.id = i;
        elements.push_back(e);
    }

    std::vector<PmergeMe::Element> sorted = sortRecursive(elements, Comparisons);

    std::vector<int> result;

	// just return a vector of values
    for (size_t i = 0; i < sorted.size(); i++)
        result.push_back(sorted[i].value);

    return result;
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
