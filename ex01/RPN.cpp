#include "RPN.hpp"

RPN::RPN(){

}

RPN::~RPN(){

}

//according to subject, we just accept single digit numerical input
static bool isOperand(char cur){
	if (cur >= '0' && cur <= '9')
		return true;
	return false;
}

int RPN::calculate(std::string Input){
	std::stringstream sinput(Input);
	int operand1;
	int operand2;

	char cur;

	while(sinput >> cur){
		if (isOperand(cur))
			_operands.push(cur - '0');
		else {
			if (_operands.size() < 2)
				throw RPN::BadInput();
			//at least 2 ints on the stack
			//we just pushed ints to the stack, so atoi will not fail
			operand2 = _operands.top();
			_operands.pop();
			operand1 = _operands.top();
			_operands.pop();
			switch(cur){
				case ('+'):
					_operands.push(operand1 + operand2);
					break;
				case ('-'):
					_operands.push(operand1 - operand2);
					break;
				case ('*'):
					_operands.push(operand1 * operand2);
					break;
				case ('/'):
					if (operand2 == 0)
						throw RPN::BadInput();
					_operands.push(operand1 / operand2);
					break;
				// not a valid operand
				default:
					throw RPN::BadInput();
			}

		}
	}
	//non char input detected
	if (!sinput.eof())
		throw RPN::BadInput();
	if (_operands.size() != 1)
		throw RPN::BadInput();
	return _operands.top();
}