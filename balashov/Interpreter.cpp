#include "Interpreter.h"

std::vector<token_pair> Interpreter::get_reverse_polish_notation(std::vector<token_pair> code)
{
	std::vector<token_pair> rpn;

	std::vector<token_pair> buffer;
	std::vector<token_pair> buffer2;
	std::vector<token_pair> buffer3;
	std::vector<int> labels;
	code.push_back({ "end", "end" });

	for (int i = 0; i < code.size(); ++i)
	{
		if (i == 62)
		{
			int deb = 0;
		}
  		if (code[i].name == "KW_IF" || code[i].name == "KW_FOR")
		{
			buffer3.push_back(code[i]);
		}
		else
		{
			if (code[i].name == "LBRACE")
			{
				buffer2.push_back(code[i]);
			}
			else
			{
				if (code[i].name == "DIGIT" || code[i].name == "VAR" || code[i].name == "UNA_ARITHM_OPER")
				{
					rpn.push_back(code[i]);
				}
				else
				{
					if (buffer.empty() && code[i].value != "=" && code[i].value != "}" && code[i].value != ";" && code[i].value != "end")
					{
						buffer.push_back(code[i]);
					}
					else
					{
						/*========================================================================*/
						if (code[i].value == "+" || code[i].value == "-" || code[i].value == ">" || code[i].value == "<" || code[i].value == "==" || code[i].value == "!=")
						{
							while (!buffer.empty())
							{
								if (buffer.back().name == "BIN_ARITHM_OPER")
								{
									rpn.push_back(buffer.back());
									buffer.pop_back();
								}
								else break;
							}
							buffer.push_back(code[i]);
						}
						/*========================================================================*/

						if (code[i].value == "*" || code[i].value == "/")
						{
							if (buffer.back().value == "*" || buffer.back().value == "/")
							{
								rpn.push_back(buffer.back());
								buffer.pop_back();
							}
							else
							{
								buffer.push_back(code[i]);
							}
						}
						/*========================================================================*/

						if (code[i].value == ")")
						{
							while (buffer.back().value != "(")
							{
								rpn.push_back(buffer.back());
								buffer.pop_back();
							}
							if (buffer.back().value == "(")
							{
								buffer.pop_back();
							}
							if (!buffer3.empty() && buffer3.back().value == "if" && buffer3.size() > labels.size())
							{
								labels.push_back(rpn.size());
								rpn.push_back({ "label", "void" });
								rpn.push_back({ "if", "!F" });
							}
							if (!buffer2.empty() && buffer2.back().value != "{" && buffer2.back().value != "=")
							{
								buffer2.pop_back();
							}
						}
						/*========================================================================*/

						if (code[i].value == "(")
						{
							buffer.push_back(code[i]);
						}
						/*========================================================================*/

						if (code[i].value == "}")
						{
							if (!buffer2.empty())
							{
								if (buffer2.back().value == "{" && buffer3.back().name == "KW_IF")
								{
									if (buffer3.size() > 1 && buffer3[buffer3.size() - 2].name == "KW_FOR")
									{
										rpn[labels.back()] = { "@", std::to_string(rpn.size() + 2) };
										labels.pop_back();
										rpn.push_back({ "@", std::to_string(labels.back()) });
										rpn.push_back({ "jump", "!" });
										labels.pop_back();
										buffer2.pop_back();
										buffer3.pop_back();
										buffer3.pop_back();
									}
									else
									{
										rpn[labels.back()] = { "@", std::to_string(rpn.size()) };
										labels.pop_back();
										buffer2.pop_back();
										buffer3.pop_back();
									}
								}
								else
								{
									if (buffer2.back().value == "{" && buffer3.back().name == "KW_FOR")
									{
										rpn.push_back({ "@", std::to_string(labels.back()) });
										rpn.push_back({ "jump", "!" });
										labels.pop_back();
										buffer2.pop_back();
										buffer3.pop_back();

										rpn[labels.back()] = { "@", std::to_string(rpn.size() + 2) };
										labels.pop_back();
										buffer2.pop_back();
										buffer3.pop_back();
									}
								}
							}
						}
						/*========================================================================*/

						if (code[i].value == "=")
						{
							buffer2.push_back(code[i]);
						}
						/*========================================================================*/

						if (code[i].value == ";")
						{
							while (!buffer.empty() && buffer.back().value != "(")
							{
								rpn.push_back(buffer.back());
								buffer.pop_back();
							}

							while (!buffer2.empty() && buffer2.back().value != "{")
							{
								rpn.push_back(buffer2.back());
								buffer2.pop_back();
							}

							if (!buffer3.empty())
							{
								if (buffer3.back().name == "KW_FOR")
								{
									if (buffer3.size() > labels.size())
									{
										labels.push_back(rpn.size());
									}
									else
									{
										if (buffer3.size() == labels.size())
										{
											if (buffer3.back().value == "for")
											{
												buffer3.push_back({ "KW_IF", "if" });
												labels.push_back(rpn.size());
												rpn.push_back({ "label", "void" });
												rpn.push_back({ "if", "!F" });
											}
										}
									}
								}
							}
						}
						/*========================================================================*/

						if (code[i].value == "end")
						{
							while (!buffer.empty())
							{
								rpn.push_back(buffer.back());
								buffer.pop_back();
							}
							while (!buffer3.empty())
							{
								if (buffer3.back().name == "KW_IF")
								{
									//rpn[labels.back()] = { "@", std::to_string(rpn.size() + 1) };
									rpn[labels.back()] = { "@", std::to_string(rpn.size()) };
									labels.pop_back();
									buffer3.pop_back();
								}
								if (buffer3.back().value == "for")
								{
									//rpn.push_back({ "@", std::to_string(labels.back() + 1) });
									rpn.push_back({ "@", std::to_string(labels.back()) });
									rpn.push_back({ "jump", "!" });
									labels.pop_back();
									buffer3.pop_back();
								}
							}
						}
					}
				}
			}
		}
	}

	return rpn;
}

std::vector<std::string> Interpreter::getSimpleRPN(std::vector<token_pair> code)
{
	std::vector<std::string> rpn;
	std::vector<token_pair> buffer;
	code.push_back({ "end", "end" });

	for (int i = 0; i < code.size(); ++i)
	{
		if (code[i].name == "DIGIT" || code[i].name == "VAR")
		{
			rpn.push_back(code[i].value);
		}
		else
		{
			if (buffer.empty())
			{
				buffer.push_back(code[i]);
			}
			else
			{
				/*========================================================================*/
				if (code[i].value == "+" || code[i].value == "-" || code[i].value == ">" || code[i].value == "<" || code[i].value == "==" || code[i].value == "!=")
				{
					while (!buffer.empty())
					{
						if (buffer.back().name == "BIN_ARITHM_OPER")
						{
							rpn.push_back(buffer.back().value);
							buffer.pop_back();
						}
						else break;
					}
					buffer.push_back(code[i]);
				}
				/*========================================================================*/

				if (code[i].value == "*" || code[i].value == "/")
				{
					if (buffer.back().value == "*" || buffer.back().value == "/")
					{
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}
					else
					{
						buffer.push_back(code[i]);
					}
				}
				/*========================================================================*/

				if (code[i].value == ")")
				{
					while (buffer.back().value != "(")
					{
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}
					if (buffer.back().value == "(")
					{
						buffer.pop_back();
					}
				}
				/*========================================================================*/

				if (code[i].value == "(")
				{
					buffer.push_back(code[i]);
				}
				/*========================================================================*/

				if (code[i].value == "end")
				{
					while (!buffer.empty())
					{
						rpn.push_back(buffer.back().value);
						buffer.pop_back();
					}
				}
			}
		}
	}

	return rpn;
}

