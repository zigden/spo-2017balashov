#include "Lexer.h"

std::vector<token_pair> Lexer::getTokens(std::string fname)
{
	std::string code = readCodeFromFile(fname);
	std::vector<Lexer::tokens> myRegex = getRegexFromFile();
	std::vector<token_pair> resultTokens;

	while (!code.empty())
	{
		eraseWhitespaces(code);
		std::string buff;
		std::vector<Lexer::tokens> tempTokens;
		bool tokenSolved = false;
		if (code.size() != 0)
		{
			do
			{
				buff.push_back(code[0]);
				tempTokens = findMatch(buff, myRegex);
				if (tempTokens.empty())
				{
					buff.erase(buff.size() - 1, 1);
					tempTokens = findMatch(buff, myRegex);
					if (tempTokens.empty())
					{
						buff.push_back(code[0]);
						std::cout << "\n\nОшибка! Неизвестный символ \"" << buff << "\"\n";
						resultTokens.clear();
						return resultTokens;
					}
					Lexer::tokens tempResult = tempTokens[0];
					for (int i = 1; i < tempTokens.size(); ++i)
					{
						if (tempTokens[i].priority < tempResult.priority)
						{
							tempResult = tempTokens[i];
						}
					}
					resultTokens.push_back({ tempResult.name, buff });
					tokenSolved = true;
				}
				else
				{
					code.erase(0, 1);
				}
			} while (!tokenSolved);
		}
	}

	return resultTokens;
}

std::vector<Lexer::tokens> Lexer::getRegexFromFile()
{
	std::vector<Lexer::tokens> result;
	std::ifstream rxfile("regex.txt");

	if (!rxfile.is_open())
	{
		std::cout << "\nФайл с регулярными выражениями regex.txt не может быть открыт!\n\n";
		system("pause");
		exit(0);
	}
	else
	{
		while (!rxfile.eof())
		{
			std::string regex;
			std::getline(rxfile, regex);
			if (regex.empty()) break;
			std::string RXname;
			std::string RXrx;
			int         RXpr;

			std::tr1::regex rxN("^[A-Z_]+");
			std::tr1::match_results<const char *> name;
			std::tr1::regex_search(regex.c_str(), name, rxN);
			RXname = name.str();
			regex.erase(0, name.str().size() + 1);

			RXrx = regex.substr(0, regex.size() - 2);
			regex.erase(0, RXrx.size() + 1);

			RXpr = atoi(regex.c_str());

			result.push_back(Lexer::tokens{ RXname , RXrx , RXpr });
		}
		rxfile.close();
	}
	return result;
}

std::vector<Lexer::tokens> Lexer::findMatch(std::string str, std::vector<Lexer::tokens> & myRegex)
{
	std::vector<tokens> match;
	for (int j = 0; j < myRegex.size(); ++j)
	{
		std::tr1::regex rx(myRegex[j].rx);
		if (std::tr1::regex_match(str.begin(), str.end(), rx))
		{
			match.push_back(myRegex[j]);
		}
	}
	return match;
}

void Lexer::eraseWhitespaces(std::string & str)
{
	char space = ' ';
	char tab = '	';
	char newLine = '\n';
	while (str[0] == space || str[0] == tab || str[0] == newLine)
	{
		str.erase(0, 1);
	}
}

std::string Lexer::readCodeFromFile(std::string fname)
{
	std::ifstream txtfile(fname);
	std::string code;
	code.clear();

	if (!txtfile.is_open())
	{
		std::cout << "Файл с исходным кодом не может быть открыт!\n\n";
		system("pause");
		exit(0);
	}
	else
	{
		while (!txtfile.eof())
		{
			std::string buff;
			std::getline(txtfile, buff);
			code = code + " " + buff;
		}
		txtfile.close();
	}

	return code;
}
