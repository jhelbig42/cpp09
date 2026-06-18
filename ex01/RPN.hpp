#include <cstdlib>
#include <stack>
#include <iostream>
#include <sstream>

class RPN{
	private:
		std::stack<double> _operands;

		RPN(RPN &other);
		RPN &operator&(RPN &other);

	public:
		RPN();
		~RPN();

		int calculate(std::string Input);

	
	class BadInput : public std::exception{
		public:
			virtual const char * what() const throw(){
				return ("Error");
			}
	};
};
