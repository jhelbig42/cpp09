#include "BitcoinExchange.hpp"

int main (int argc, char** argv){
	
	if (argc != 2){
		std::cerr << "usage: ./btc [inputfile]" << std::endl;
		return 1;
	}

	//reads database
	try{
		BitcoinExchange exchange;
		exchange.handleInputFile(argv[1]);
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
