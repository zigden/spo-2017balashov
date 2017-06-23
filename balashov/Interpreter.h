#pragma once
#include "Parser.h"


class Interpreter
{


public:
	std::vector<token_pair> get_reverse_polish_notation(std::vector<token_pair> code);

private:
	std::vector<std::string> getSimpleRPN(std::vector<token_pair> code);
	

};