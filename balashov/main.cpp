#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Stack_machine.h"

int main()
{
	setlocale(LC_ALL, "Russian");
	std::cout << "Практическое задание №4\n\n";
	std::cout << "Введите имя *.txt файла \n(файл должен располагаться в одной папке с программой)\n";
	std::string fname;
	std::cin >> fname;

	Lexer lexer;
	std::vector<token_pair> tokens = lexer.getTokens(fname);

	if (!tokens.empty())
	{
		std::cout << "\nЛексер не выявил ошибок.";

		Parser parser;
		bool correct = parser.checkSyntax(tokens);
		if (correct)
		{
			std::cout << "Парсер не выявил ошибок.\n\n";

			Interpreter interpreter;
			std::vector<token_pair> rpn = interpreter.get_reverse_polish_notation(tokens);

			Stack_machine stack_machine;
			std::vector<token_pair> exe = stack_machine.execute_program(rpn);

			for (int i = 0; i < exe.size(); ++i)
			{
				std::cout << std::left << std::setw(6) << exe[i].name << exe[i].value << "\n";
			}
		}
		else
		{
			std::cout << "\nПарсер выявил ошибку.\n";
		}
	}
	else
	{
		std::cout << "\nЛексер выявил ошибку.\n";
	}

	std::cout << "\n";
	system("pause");
	return 0;
}
