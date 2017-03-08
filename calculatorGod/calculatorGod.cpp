// calculatorGod.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

enum SYMBOLTYPE
{
	number,
	operation,
	bracket
};

class symbol
{
public:
	int value;
	SYMBOLTYPE type;

	symbol(int _value, SYMBOLTYPE _type)
	{
		value = _value;
		type = _type;
	}
};

template <typename T>
std::vector<T> SubVector(std::vector<T> vector, int from, int to)
{
	if (from < 0 || to < from || to > vector.size() - 1)
	{
		std::cout << "BITCH!" << std::endl;
		return vector;
	}

	std::vector<T> newVector;
	for (int i = from; i <= to; i++)
	{
		newVector.push_back(vector.at(i));
	}
	return newVector;
}

template <typename T>
std::vector<T> Replace(std::vector<T> vector, int from, int to, T thingy)
{
	if (from < 0 || to < from || to > vector.size() - 1)
	{
		std::cout << "BITCHES dont replace!" << std::endl;
		return vector;
	}

	vector.at(from) = thingy;

	auto begin = vector.cbegin() + from + 1;
	auto end = vector.cbegin() + to + 1;

	vector.erase(begin, end);
	return vector;
}

void print(std::vector<symbol> symbols)
{
	for (int i = 0; i < symbols.size(); i++)
	{
		if (symbols.at(i).type == SYMBOLTYPE::number)
		{
			std::cout << symbols.at(i).value;
		}
		else if (symbols.at(i).type == SYMBOLTYPE::operation)
		{
			switch (symbols.at(i).value)
			{
			case 0:
				std::cout << '+';
				break;
			case 1:
				std::cout << '-';
				break;
			case 2:
				std::cout << '*';
				break;
			case 3:
				std::cout << '/';
				break;
			case 4:
				std::cout << '^';
				break;
			case 5:
				std::cout << '%';
				break;
			}
			
		}
		else if (symbols.at(i).type == SYMBOLTYPE::bracket)
		{
			if (symbols.at(i).value == 0)
			{
				std::cout << '(';
			}
			else
			{
				std::cout << ')';
			}
		}
		
	}
	std::cout << std::endl;
}

class equation
{
public:
	std::vector<symbol> symbols;

	equation(std::string input)
	{
		for (int i = 0; i < input.size(); i++)
		{
			char curChar = input.at(i);
			if (curChar >= '0' && curChar <= '9')
			{
				int num = curChar - '0';
				if (i < input.size() - 1)
				{
					for (int x = i+1; x < input.size(); x++, i++)
					{
						if (input.at(x) >= '0' && input.at(x) <= '9')
						{
							num *= 10;
							num += input.at(x)-'0';
						}
						else
						{
							break;
						}
					}
				}
				symbols.push_back(symbol(num, SYMBOLTYPE::number));
			}
			else if (curChar == '+')
			{
				symbols.push_back(symbol(0, SYMBOLTYPE::operation));
			}
			else if (curChar == '-')
			{
				symbols.push_back(symbol(1, SYMBOLTYPE::operation));
			}
			else if (curChar == '*' || curChar == 'x')
			{
				symbols.push_back(symbol(2, SYMBOLTYPE::operation));
			}
			else if (curChar == '/' || curChar == ':')
			{
				symbols.push_back(symbol(3, SYMBOLTYPE::operation));
			}
			else if (curChar == '^')
			{
				symbols.push_back(symbol(4, SYMBOLTYPE::operation));
			}
			else if (curChar == '%')
			{
				symbols.push_back(symbol(5, SYMBOLTYPE::operation));
			}
			else if (curChar == '(' || curChar == '[')
			{
				symbols.push_back(symbol(0, SYMBOLTYPE::bracket));
			}
			else if (curChar == ')' || curChar == ']')
			{
				symbols.push_back(symbol(1, SYMBOLTYPE::bracket));
			}
		}
	}
};

symbol simplify(std::vector<symbol> symbols)
{
	//std::cout << symbols.size() << std::endl;
	//print(symbols);
	int lastOpenBracket = -1;

	//int lastOperation = -1;
	while (symbols.size() != 1)
	{
		bool doSimpleMath = true;
		bool doNonSimpleMath = true;
		for (int i = 0; i < symbols.size(); i++)
		{
			symbol* curSymbol = &symbols.at(i);
			if (curSymbol->type == SYMBOLTYPE::bracket)
			{
				doSimpleMath = false;
				doNonSimpleMath = false;
				if (curSymbol->value == 0)
				{
					lastOpenBracket = i;
				}
				else if (lastOpenBracket != -1)
				{
					if (lastOpenBracket + 1 == i - 1)
					{
						symbols = Replace(symbols, lastOpenBracket, i, symbols.at(lastOpenBracket + 1));
					}
					else
					{
						std::vector<symbol> v = SubVector(symbols, lastOpenBracket + 1, i - 1);
						print(v);
						std::cout << " = ";
						symbols = Replace(symbols, lastOpenBracket, i, simplify(v));
						print(symbols);
					}
					break;
				}
			}
		}

		if (doNonSimpleMath)
		{
			for (int i = 0; i < symbols.size(); i++)
			{
				symbol* curSymbol = &symbols.at(i);
				if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 2)
				{
					doSimpleMath = false;
					symbols = Replace(symbols, i - 1, i + 1, symbol(symbols.at(i - 1).value*symbols.at(i + 1).value, SYMBOLTYPE::number));
					break;
				}
				else if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 3)
				{
					doSimpleMath = false;
					symbols = Replace(symbols, i - 1, i + 1, symbol(symbols.at(i - 1).value / symbols.at(i + 1).value, SYMBOLTYPE::number));
					break;
				}
				else if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 4)
				{
					doSimpleMath = false;
					symbols = Replace(symbols, i - 1, i + 1, symbol(pow(symbols.at(i - 1).value, symbols.at(i + 1).value), SYMBOLTYPE::number));
					break;
				}
				else if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 5)
				{
					doSimpleMath = false;
					symbols = Replace(symbols, i - 1, i + 1, symbol(symbols.at(i - 1).value%symbols.at(i + 1).value, SYMBOLTYPE::number));
					break;
				}
			}
		}
		

		if (doSimpleMath)
		{
			for (int i = 0; i < symbols.size()-1; i++)
			{
				symbol* curSymbol = &symbols.at(i);
				if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 0)
				{
					symbols = Replace(symbols, i - 1, i + 1, symbol(symbols.at(i - 1).value + symbols.at(i + 1).value, SYMBOLTYPE::number));
					break;
				}
				else if (curSymbol->type == SYMBOLTYPE::operation && curSymbol->value == 1)
				{
					if (symbols.at(i + 1).type == SYMBOLTYPE::number)
					{
						if (i - 1 < 0 || symbols.at(i - 1).type != SYMBOLTYPE::number)
						{
							symbols = Replace(symbols, i, i + 1, symbol(-1 * symbols.at(i + 1).value, SYMBOLTYPE::number));
						}
						else
						{
							symbols = Replace(symbols, i - 1, i + 1, symbol(symbols.at(i - 1).value - symbols.at(i + 1).value, SYMBOLTYPE::number));
						}
						break;
					}
				}
			}
		}
	}
	return symbols.at(0);
}

void askEquation()
{
	std::cout << "Input equation: " << std::endl;
	std::string input;
	std::cin >> input;
	equation test(input);
	std::cout << "answer: " << simplify(test.symbols).value << std::endl;
}

int main()
{
	while (true)
	{
		askEquation();
		if (GetAsyncKeyState(VK_LMENU))
		{
			break;
		}
		Sleep(100);
	}
    return 0;
}

