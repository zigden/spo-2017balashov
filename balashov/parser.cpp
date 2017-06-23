#include "Parser.h"

bool Parser::checkSyntax(std::vector<token_pair> tokens)
{
	std::string tokensInStr;
	for (int i = 0; i < tokens.size(); ++i)
	{
		tokensInStr = tokensInStr + tokens[i].name + " ";
	}
	tokensInStr.erase(tokensInStr.size() - 1, 1);

	std::vector<token> syntaxRx = getSyntaxRegEx();

	std::tr1::regex rx(syntaxRx[0].rx);
	bool errors = std::tr1::regex_search(tokensInStr.c_str(), rx);
	return errors;
}

std::vector<token> Parser::getRegexFromFile()
{
	std::vector<token> result;
	std::ifstream rxfile("syntax.txt");

	if (!rxfile.is_open())
	{
		std::cout << "\n‘айл с регул€рными выражени€ми syntax.txt не может быть открыт!\n\n";
		system("pause");
		exit(0);
	}
	else
	{
		token syntax;
		while (!rxfile.eof())
		{
			std::string regex;
			std::getline(rxfile, regex);
			if (regex.empty()) break;
			std::string RXname;
			std::string RXrx;
			std::tr1::regex rxN("^[a-z_]+");
			std::tr1::match_results<const char *> name;
			std::tr1::regex_search(regex.c_str(), name, rxN);
			RXname = name.str();
			regex.erase(0, name.str().size() + 2);
			RXrx = regex;
			
			result.push_back(token{ RXname , RXrx });
		}
		rxfile.close();
	}
	return result;
}

std::vector<token> Parser::getSyntaxRegEx()
{
	std::vector<token> syntaxRx = getRegexFromFile();

	for (int kostil = 0; kostil < 5; ++kostil)
	{
		for (int i = 0; i < syntaxRx.size(); ++i)
		{
			for (int j = 0; j < syntaxRx.size(); ++j)
			{
				{
					std::tr1::regex rxN(syntaxRx[j].name);
					syntaxRx[i].rx = std::tr1::regex_replace(syntaxRx[i].rx.c_str(), rxN, syntaxRx[j].rx);
				}
			}
		}
	}
	syntaxRx[0].rx = "^" + syntaxRx[0].rx + "$";

	return syntaxRx;
}
