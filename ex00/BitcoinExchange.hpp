#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

class BitcoinExchange{
	private:
		std::map<int, float> _exchangeRate;
		BitcoinExchange( const BitcoinExchange &other);
		BitcoinExchange &operator=( const BitcoinExchange &other);
		
	public:

		BitcoinExchange();
		~BitcoinExchange(); 

		void checkDate(int year, int month, int day) const ;
		void checkAmount(float amount) const;
		float findRate(int date) const;
		void handleInputLine(std::string &line);
		void handleInputFile(const char *Filename) const;
		
		class BadFile :public std::exception{
			public:
				virtual const char * what() const throw(){
					return ("Error: could not open file");
				}
		};

		class BadInput : public std::exception{
			public:
			virtual const char * what() const throw(){
				return ("Error: Bad Input => ");
			 }
		};

		class NumberNotPositive : public std::exception{
			public:
			virtual const char * what() const throw(){
				return ("Error: not a positive number");
			}
		};
		
		class NumberTooLarge : public std::exception{
			public:
			 virtual const char * what() const throw(){
				return ("Error: too large a number");
			}
		};
};
