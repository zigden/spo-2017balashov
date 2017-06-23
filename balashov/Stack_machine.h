#pragma once
#include "Interpreter.h"

class Stack_machine
{
public:
	std::vector<token_pair> execute_program(std::vector<token_pair> rpn);

private:
	float sum(std::string a, std::string b);
	float sub(std::string a, std::string b);
	float mul(std::string a, std::string b);
	float div(std::string a, std::string b);
	float inc(std::string a);
	float dec(std::string a);
	bool  com(std::string a, std::string b);
	float ass(std::string a, std::string b);
	
	std::vector<token_pair> stack;
	std::vector<token_pair> RAM;

	void update_variable(token_pair var);
	std::string find_value_by_name(std::string name);
};
