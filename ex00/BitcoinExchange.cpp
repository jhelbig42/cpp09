#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(){
	//read from Database to generate map
	std::fstream database;
	database.open("data.csv");

	if (!database.is_open())
		throw BitcoinExchange::BadFile();

	int year, month, day;
	float rate;
	char dash1, dash2, comma;
	std::string line;
	while (std::getline(database, line)){
		if (line.find("date,exchange_rate") != std::string::npos)
			continue ;
		std::stringstream sline(line);
		if (sline >> year >> dash1 >> month >> dash2 >> day >> comma >> rate
				&& dash1 == '-' && dash2 == '-' && comma == ',')
					_exchangeRate[year * 10000 + month * 100 + day] = rate;
		else
			throw BitcoinExchange::BadInput();
	}	
}

BitcoinExchange::~BitcoinExchange(){
}

void BitcoinExchange::checkAmount(float amount) const{	
	if (amount > 1000)
		throw BitcoinExchange::NumberTooLarge();
	if (amount < 0)
		throw BitcoinExchange::NumberNotPositive();
}

void BitcoinExchange::checkDate(int year, int month, int day) const{
    if (month < 1 || month > 12) 
        throw BitcoinExchange::BadInput();
    if (day < 1)
        throw BitcoinExchange::BadInput();

    int maxDays;
    switch (month) {
        // April, June, September, November
        case 4: case 6: case 9: case 11:
            maxDays = 30;
            break;

        // February 
        case 2:
            // Leap year rule in the Gregorian calendar:
            // A year is a leap year if it's divisible by 4, 
            // EXCEPT for century years, which must also be divisible by 400.
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                maxDays = 29;
            else
                maxDays = 28;
            break;

        default:
            maxDays = 31;
    }

    if (day > maxDays)
		throw BitcoinExchange::BadInput();
}


float BitcoinExchange::findRate(int date) const{
	if (date < _exchangeRate.begin()->first)
		throw BitcoinExchange::BadInput();
	while(1)
	{
		try
		{
			return(_exchangeRate.at(date));
		}
		catch(...)
		{
			date--;
		}
	}
}

void BitcoinExchange::handleInputFile(const char *Filename) const{
	//open file
	std::fstream input;
	input.open(Filename);

	if (!input.is_open())
		throw BitcoinExchange::BadFile();

	int year, month, day;
	float amount;
	int date;
	char dash1, dash2, pipe;
	std::string line;
	while (std::getline(input, line)){
		if (line.find("date | value") != std::string::npos)
			continue ;
		std::stringstream sline(line);
		{	
			try{
				if (!(sline >> year >> dash1 >> month >> dash2 >> day >> pipe >> amount
				&& dash1 == '-' && dash2 == '-' && pipe == '|'))
					throw BitcoinExchange::BadInput();
				checkAmount(amount);
				checkDate(year, month, day);
				
				date = year * 10000 + month * 100 + day;
				
				const float rate = findRate(date);
				
				std::cout << year << "-" << month << "-" << day << " => " 
					<< amount << " = " << amount * rate << std::endl; 
			}
			catch(BitcoinExchange::BadInput &e){
				std::cout << e.what() << line << std::endl;
			}
			catch(std::exception &e){
				std::cout << e.what() << std::endl;
			}
		}		
	}
}