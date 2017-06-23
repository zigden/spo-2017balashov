#include "Stack_machine.h"

std::vector<token_pair> Stack_machine::execute_program(std::vector<token_pair> rpn)
{
	for (int i = 0; i < rpn.size(); ++i)
	{
		if (rpn[i].name == "VAR")
		{
			bool already_exist = false;
			for (int j = 0; j < RAM.size(); ++j)
			{
				already_exist = RAM[j].name == rpn[i].value;
				if (already_exist)
				{
					break;
				}
			}
			if (!already_exist)
			{
				RAM.push_back({ rpn[i].value, "0" });
			}
		}
	}

	int i = 0;
	while (i < rpn.size())
	{
		stack.push_back(rpn[i]);
		//rpn.erase(rpn.begin());

		int last   = stack.size() - 1;
		int penult = stack.size() - 2;

		if (stack.back().name == "BIN_ARITHM_OPER")
		{
			last--;
			penult--;
			std::string a;
			std::string b;
			if (stack[penult].name == "VAR")
			{
				a = find_value_by_name(stack[penult].value);
			}
			else
			{
				a = stack[penult].value;
			}
			if (stack[last].name == "VAR")
			{
				b = find_value_by_name(stack[last].value);
			}
			else
			{
				b = stack[last].value;
			}

			if (stack.back().value == "+")
			{
				float temp = sum(a, b);
				stack._Pop_back_n(3);
				stack.push_back({ "DIGIT", std::to_string(temp) });
			}
			if (stack.back().value == "-")
			{
				float temp = sub(a, b);
				stack._Pop_back_n(3);
				stack.push_back({ "DIGIT", std::to_string(temp) });
			}
			if (stack.back().value == "*")
			{
				float temp = mul(a, b);
				stack._Pop_back_n(3);
				stack.push_back({ "DIGIT", std::to_string(temp) });
			}
			if (stack.back().value == "/")
			{
				float temp = div(a, b);
				stack._Pop_back_n(3);
				stack.push_back({ "DIGIT", std::to_string(temp) });
			}
		}
		if (stack.back().name == "UNA_ARITHM_OPER")
		{
			if (!stack.empty() && stack.back().value == "++")
			{
				float temp = inc(find_value_by_name(stack[penult].value));
				std::string var = stack[penult].value;
				stack._Pop_back_n(2);
				//stack.push_back({ "DIGIT", std::to_string(temp) });
				update_variable({ var, std::to_string(temp) });
			}
			if (!stack.empty() && stack.back().value == "--")
			{
				float temp = dec(stack[penult].value);
				std::string var = stack[penult].name;
				stack._Pop_back_n(2);
				stack.push_back({ "DIGIT", std::to_string(temp) });
				update_variable({ var, std::to_string(temp) });
			}
		}
		if (!stack.empty() && stack.back().name == "LOGIC_OPER")
		{
			std::string op = stack[last].value;
			stack._Pop_back_n(1);
			last--;
			penult--;
			std::string a;
			std::string b;
			if (stack[penult].name == "VAR")
			{
				a = find_value_by_name(stack[penult].value);
			}
			else
			{
				a = stack[penult].value;
			}
			if (stack[last].name == "VAR")
			{
				b = find_value_by_name(stack[last].value);
			}
			else
			{
				b = stack[last].value;
			}
			bool temp;
			if (op == ">" | op == "<")
			{
				temp = com(a, b);
				if (op == "<")
				{
					temp = !temp;
				}
			}
			if (op == "==")
			{
				bool t1 = com(a, b);
				bool t2 = com(b, a);
				temp = !t1 && !t2;
			}
			if (op == "!=")
			{
				bool t1 = com(a, b);
				bool t2 = com(b, a);
				temp = !t1 || !t2;
			}
			stack._Pop_back_n(2);
			stack.push_back({ "bool", std::to_string(temp) });
		}
		if (!stack.empty() && stack.back().name == "ASSIGN_OPER")
		{
			stack._Pop_back_n(1);
			last--;
			penult--;
			float temp = stof(stack[last].value);
			std::string var = stack[penult].value;
			stack._Pop_back_n(2);
			update_variable({ var, std::to_string(temp) });
		}
		if (!stack.empty() && stack.back().name == "if")
		{
			last--;
			penult--;
			std::string a;
			if (stack[penult].name == "VAR")
			{
				a = find_value_by_name(stack[penult].value);
			}
			else
			{
				a = stack[penult].value;
			}
			float temp = stof(a);
			float label = stof(stack[last].value);
			stack._Pop_back_n(3);
			if (temp == 0)
			{
				i = label - 1;
			}
		}
		if (!stack.empty() && stack.back().name == "jump")
		{
			float label = stof(stack[stack.size() - 2].value);
			stack._Pop_back_n(2);
			i = label - 1;
		}
		
		++i;

		if (i < 0)
		{
			int g = 9;
		}
	}

	return RAM;
}

float Stack_machine::sum(std::string a, std::string b)
{
	return stof(a) + stof(b);
}

float Stack_machine::sub(std::string a, std::string b)
{
	return stof(a) - stof(b);
}

float Stack_machine::mul(std::string a, std::string b)
{
	return stof(a) * stof(b);
}

float Stack_machine::div(std::string a, std::string b)
{
	return stof(a) / stof(b);
}

float Stack_machine::inc(std::string a)
{
	return stof(a) + 1;
}

float Stack_machine::dec(std::string a)
{
	return stof(a) - 1;
}

bool Stack_machine::com(std::string a, std::string b)
{
	return stof(a) > stof(b);
}

float Stack_machine::ass(std::string a, std::string b)
{
	return 0.0f;
}

void Stack_machine::update_variable(token_pair var)
{
	for (int i = 0; i < RAM.size(); ++i)
	{
		if (RAM[i].name == var.name)
		{
			RAM[i].value = var.value;
			break;
		}
	}
}

std::string Stack_machine::find_value_by_name(std::string name)
{
	for (int i = 0; i < RAM.size(); ++i)
	{
		if (RAM[i].name == name)
		{
			return RAM[i].value;
		}
	}
}
