#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

struct token_pair
{
	std::string name;
	std::string value;
};

class Lexer
{
public:
	std::vector<token_pair> getTokens(std::string fname);
	struct tokens
	{
		std::string name;
		std::string rx;
		int priority;
	};
	std::vector<Lexer::tokens> getRegexFromFile();
	std::vector<Lexer::tokens> findMatch(std::string str, std::vector<Lexer::tokens> & myRegex);
private:
	void eraseWhitespaces(std::string & str);
	std::string readCodeFromFile(std::string fname);
};