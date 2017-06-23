#pragma once
#include "Lexer.h"

struct token
{
	std::string name;
	std::string rx;
};

class Parser
{
public:
	bool checkSyntax(std::vector<token_pair> tokens);
	std::vector<token> getSyntaxRegEx();
private:

	std::vector<token> getRegexFromFile();
	
};