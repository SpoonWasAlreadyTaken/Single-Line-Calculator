#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <math.h>


using namespace std;

// variables

int highestMax;
int lowestMin;

int timesRolled;
vector<string> diceRolled;
vector<int> numbersRolled;

bool fullNumbers = false;

string currentInput;


char breakOn = 'd';


// functions

int NumberGenerator(int min, int max);
vector<string> Parse(string input, char breakOn);
void ShowStats();
string MathOperations(string input);
double ToDouble(string convert);
int ToInt(string convert);
int EvaluateOperator(string operation);
string RemoveTrail(string number);
string Rainbow(string toColor);
int bracketPairs = 0;
template <typename T, typename Y>
int Index(vector<T> toIndex, Y find);




// main

int main()
{
start:
	cout << "\u001b[2m" << "Write an equation to solve\n" << "Currently supports the following operations: +, -, *, /, ^, !\n" << "Can use d as an operator to roll dice\n" << "Can use f as an operator to produce the fibonacci sequence, number before operator is starting point, number after is sequence length\n" << "Can use l as an operator for Logarithm, number before operator is base, number after is value\n" << "stats for statistics of all operations performed this session\n" << "clear to clear the console\n" << "help to show this help menu\n" << "round for rounded numbers mode, round again to disable \n" << "\u001b[0m";

runAgain:

	getline(cin, currentInput);


	if (currentInput == "stats")
	{
		ShowStats();
	}
	else if (currentInput == "clear")
	{
		system("CLS");
	}
	else if (currentInput == "help")
	{
		goto start;
	}
	else if (currentInput == "")
	{
		cout << "\n";
	}
	else if (currentInput == "round")
	{
		fullNumbers = !fullNumbers;
		if (fullNumbers)
		{
			cout << "Rounding = True" << "\n";
		}
		else
		{
			cout << "Rounding = False" << "\n";
		}

	}
	else
	{
		cout << MathOperations(currentInput);
	}



	goto runAgain;


	system("PAUSE");


}


string MathOperations(string input)
{
	string store = "";
	vector<string> toProcess = {};
	vector<int*> operatorOrder = {};
	vector<int> operatorPositions = {};
	int positionIdentifier = 0;
	string output = "";
	double result = 0;
	int repeats = 0;
	bool hasNumbers = false;
	bool monoOperator = false;
	bool openingBracket = false;
	bracketPairs = 0;

	double number1;
	double number2;

	input += " ";

	// creates toProcess
	for (int i = 0; i < input.length(); i++)
	{
		if (isdigit(input.at(i)) || input.at(i) == '.' || input.at(i) == '-')
		{
			if (input.at(i) != '-')
			{
				store += input.at(i);
				if (isdigit(input.at(i)))
				{
					hasNumbers = true;
				}
			}
			else
			{
				if (i == 0)
				{
					store += input.at(i);
				}
				else if (!isdigit(input.at(i - 1)) && isdigit(input.at(i + 1)))
				{
					store += input.at(i);
				}
				else
				{
					toProcess.push_back(store);
					store = "";
					positionIdentifier++;

					toProcess.push_back("-");
					operatorPositions.push_back(positionIdentifier);
					positionIdentifier++;
				}
			}
		}
		else if (input.at(i) == ' ' || input.at(i) == 'd' || input.at(i) == '+' || input.at(i) == '/' || input.at(i) == '*' || input.at(i) == '^' || input.at(i) == '!' || input.at(i) == 'f' || input.at(i) == 'l' || input.at(i) == '(' || input.at(i) == ')')
		{
			if (store != "")
			{
				toProcess.push_back(store);
				store = "";
				positionIdentifier++;
			}

			if (input.at(i) != ' ')
			{
				if (input.at(i) == 'd')
				{
					toProcess.push_back("d");
				}
				else if (input.at(i) == '+')
				{
					toProcess.push_back("+");
				}
				else if (input.at(i) == '/')
				{
					toProcess.push_back("/");
				}
				else if (input.at(i) == '*')
				{
					toProcess.push_back("*");
				}
				else if (input.at(i) == '^')
				{
					toProcess.push_back("^");
				}
				else if (input.at(i) == '!')
				{
					toProcess.push_back("!");
				}
				else if (input.at(i) == 'f')
				{
					toProcess.push_back("f");
				}
				else if (input.at(i) == 'l')
				{
					toProcess.push_back("l");
				}
				else if (input.at(i) == '(')
				{
					toProcess.push_back("(");

					openingBracket = true;
				}
				else if (input.at(i) == ')')
				{
					toProcess.push_back(")");

					if (openingBracket)
					{
						bracketPairs++;
						openingBracket = false;
					}
				}
				operatorPositions.push_back(positionIdentifier);
				positionIdentifier++;
			}
		}
	}

	if (operatorPositions.size() == 0)
	{
		output = "\x1b[1A";
		output += "\u001b[31m";
		output += currentInput + " no operators found\n" + "\u001b[37m";
		return output;
	}

	if (!hasNumbers)
	{
		output = "\x1b[1A";
		output += "\u001b[31m";
		output += currentInput + " no numbers found\n" + "\u001b[37m";
		return output;
	}

	// assigns operationPositions to operationOrder
	for (int i = 0; i < operatorPositions.size(); i++)
	{
		operatorOrder.push_back(&operatorPositions.at(i));
	}

	// sorts operatorOrder in order of operations
	for (int i = 0; i < operatorOrder.size() - 1; i++)
	{
		for (int j = i + 1; j < operatorOrder.size(); j++)
		if (EvaluateOperator(toProcess.at(*operatorOrder.at(i))) < EvaluateOperator(toProcess.at(*operatorOrder.at(j))))
		{
			swap(operatorOrder.at(i), operatorOrder.at(j));
		}
	}
/*
	cout << "operatorPositions: ";
	for (int i = 0; i < operatorOrder.size(); i++)
	{
		cout << *operatorOrder.at(i) << " ";
	}
	cout << "\n";
	cout << "toProcess(" << toProcess.size() << "): ";
	for (int i = 0; i < toProcess.size(); i++)
	{
		cout << toProcess.at(i) << " ";
	}
	cout << "\n\n";


	// removes brackets ):
	for (int i = 0; i < toProcess.size(); i++)
	{
		if (toProcess.at(i) == "(" || toProcess.at(i) == ")")
		{
			int index = Index(operatorPositions, i);
			toProcess.erase(toProcess.begin() + i);
			for (int j = index + 1; j < operatorOrder.size(); j++)
			{
				operatorPositions.at(j)--;
			}

			operatorPositions.erase(operatorPositions.begin() + index);
			operatorOrder.erase(operatorOrder.begin() + index);	
		}
	}


	cout << "I made it (:\n\n";
	cout << "operatorPositions: ";
	for (int i = 0; i < operatorOrder.size(); i++)
	{
		cout << *operatorOrder.at(i) << " ";
	}
	cout << "\n";
	cout << "toProcess(" << toProcess.size() << "): ";
	for (int i = 0; i < toProcess.size(); i++)
	{
		cout << toProcess.at(i) << " ";
	}
	cout << "\n\n";
	*/


	// performs operations
	if (operatorPositions.size() > 0)
	{
		for (int i = 0; i < operatorOrder.size(); i++)
		{
			string toOutput = "";
			monoOperator = false;

			if (*operatorOrder.at(i) != 0)
			{
				number1 = ToDouble(toProcess.at(*operatorOrder.at(i) - 1));
			}
			else
			{
				number1 = 0;
			}

			if (*operatorOrder.at(i) != toProcess.size() - 1)
			{
				number2 = ToDouble(toProcess.at(*operatorOrder.at(i) + 1));
			}
			else
			{
				number2 = 0;
			}


			if (*operatorOrder.at(i) < toProcess.size())
			{
				if (toProcess.at(*operatorOrder.at(i)) == "d")
				{
					int final = 0;
					
					if (number1 < 1)
					{
						number1 = 1;
					}

					if (number2 < 1)
					{
						number2 = 1;
					}

					for (int j = 0; j < number1; j++)
					{
						result = NumberGenerator(1, number2);

						if (result == 1)
						{
							toOutput += "\u001b[31m" + RemoveTrail(to_string(result)) + "\u001b[37m";
						}
						else if (result == ToInt(toProcess.at(*operatorOrder.at(i) + 1)))
						{
							toOutput += "\u001b[32m" + RemoveTrail(to_string(result)) + "\u001b[37m";
						}
						else
						{
							toOutput += RemoveTrail(to_string(result));
						}
						if (j != number1 - 1)
						{
							toOutput += " ";
						}

						final += result;
					}

					toOutput = RemoveTrail(to_string(final)) + " (" + toOutput + ")";
					output += toOutput;
					result = final;

				}
				else if (toProcess.at(*operatorOrder.at(i)) == "+")
				{
					result = number1 + number2;
					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + " + " + RemoveTrail(to_string(number2)) + ")";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "-")
				{
					result = number1 - number2;
					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + " - " + RemoveTrail(to_string(number2)) + ")";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "/")
				{
					result = number1 / number2;
					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + " / " + RemoveTrail(to_string(number2)) + ")";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "*")
				{
					result = number1 * number2;
					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + " * " + RemoveTrail(to_string(number2)) + ")";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "^")
				{
					result = number1;

					if (number2 != 0)
					{
						for (int j = 1; j < number2; j++)
						{
							result *= number1;
						}
					}
					else
					{
						result = 1;
					}

					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + " ^ " + RemoveTrail(to_string(number2)) + ")";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "!")
				{
					monoOperator = true;

					result = 1;

					if (number1 != 0)
					{
						for (int j = number1; j > 0; j--)
						{
							result *= j;
						}
					}

					toOutput = RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(number1)) + "!)";
					output += toOutput;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "f")
				{
					toOutput = "";
					if (number1 == 0)
					{
						number1 = 1;
					}

					double final = 0;
					result = 0;

					for (int j = 0; j < number2; j++)
					{
						final += result;

						toOutput += "\u001b[35m" + RemoveTrail(to_string(result)) + "\u001b[37m";

						j++;

						if (j == number2)
						{
							break;
						}

						toOutput += " ";

						final += number1;

						toOutput += "\u001b[36m" + RemoveTrail(to_string(number1)) + "\u001b[37m";
						if (j != number2 - 1)
						{
							toOutput += " ";
						}

						result += number1;
						number1 += result;
					}

					toOutput = RemoveTrail(to_string(final)) + " (" + toOutput + ")";
					output += toOutput;
					result = final;
				}
				else if (toProcess.at(*operatorOrder.at(i)) == "l")
				{
					if (number1 != 0)
					{
						if (number1 < 0)
						{
							number1 *= -1;
						}
					}
					else
					{
						number1 = 2;
					}

					if (number2 != 0)
					{
						if (number2 < 0)
						{
							number2 *= -1;
						}
					}
					else
					{
						number1 *= 2;
					}

					result = log(number2) / log(number1);

					output += RemoveTrail(to_string(result)) + " (" + RemoveTrail(to_string(result)) + ")";
				}

			}	
			output += " ";

			// removes operated parts
			if (toProcess.size() > *operatorOrder.at(i))
			{
				toProcess.at(*operatorOrder.at(i)) = RemoveTrail(to_string(result));

				if (toProcess.size() - 1 > *operatorOrder.at(i) && !monoOperator)
				{
					toProcess.erase(toProcess.begin() + *operatorOrder.at(i) + 1);
				}

				if (*operatorOrder.at(i) > 0)
				{
					toProcess.erase(toProcess.begin() + *operatorOrder.at(i) - 1);
				}
			}

			// reduce operatorPositions when necessary

			for (int j = Index(operatorPositions, *operatorOrder.at(i)) + 1; j < operatorPositions.size(); j++)
			{
				if (*operatorOrder.at(i) == 0 || monoOperator)
				{
					operatorPositions.at(j) += 1;
				}

					operatorPositions.at(j) -= 2;
			}
		}
	}
	else 
	{
		output = "\x1b[1A";
		output += "\u001b[31m";
		output += currentInput + " isn't an equation\n" + "\u001b[37m";
		return output;
	}
	

	output = "\x1b[1A" + currentInput + " = " + output + " = " + RemoveTrail(to_string(result)) + "\n";




	/* debug code :
	cout << "\u001b[2m";
	cout << "repeats: " << repeats << "\n";
	cout << "operatorPositions: ";
	for (int i = 0; i < operatorOrder.size(); i++)
	{
		cout << *operatorOrder.at(i) << " ";
	}
	cout << "\n";
	cout << "toProcess(" << toProcess.size() << "): ";
	for (int i = 0; i < toProcess.size(); i++)
	{
		cout << toProcess.at(i) << " ";
	}
	cout << "\n\n";
	cout << "\u001b[0m";
	cout << "\n";
	
	// debug code end */ 


	return output;
}

void ShowStats()
{
	cout << "Unfinished ):\n";
}

int NumberGenerator(int min, int max) // generates a random number based on input parameters
{
	random_device rd;
	uniform_int_distribution<> distr(min, max);
	return distr(rd);
}

vector<string> Parse(string input, char breakOn) // seperates a string based on given split
{
	vector<string> output;
	string stored;

	input += breakOn;

	for (int i = 0; i < input.length(); i++)
	{
		if (input.at(i) == breakOn || input.at(i) == ' ')
		{
			output.push_back(stored);
			stored = "";
		}
		else 
		{
			stored += input.at(i);
		}
	}

	return output;
}

double ToDouble(string convert)
{
	double output = 0;

	for (int i = 0; i < convert.length(); i++)
	{
		if (isdigit(convert.at(i)))
		{
			output = stod(convert);
			break;
		}
	}

	return output;
}

int ToInt(string convert)
{
	int output = 0;

	for (int i = 0; i < convert.length(); i++)
	{
		if (isdigit(convert.at(i)))
		{
			output = stoi(convert);
			break;
		}
	}

	return output;
}

int EvaluateOperator(string operation)
{
	int value;

	if (operation == "+" || operation == "-")
	{
		value = 1;
	}
	else if (operation == "/" || operation == "*" || operation == "^" || operation == "!")
	{
		value = 2;
	}
	else
	{
		value = 3;
	}
	
	return value;
}

string RemoveTrail(string number)
{
	bool hasDecimal = false;

	for (int i = 0; i < number.size(); i++)
	{
		if (number.at(i) == '.')
		{
			hasDecimal = true;
			break;
		}
	}
	
	if (hasDecimal)
	{
		for (int i = number.size() - 1; i > 0; i--)
		{
			if (number.at(i) == '0' || fullNumbers)
			{
				number.erase(number.begin() + i);

				if (number.at(i - 1) == '.')
				{
					number.erase(number.begin() + i - 1);
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	return number;
}

string Rainbow(string toColor)
{
	string output = "";
	string colors[] = { "\u001b[31m", "\u001b[32m", "\u001b[33m", "\u001b[34m", "\u001b[35m", "\u001b[36m"};
	for (int i = 0; i < toColor.size(); i++)
	{
		if (toColor.at(i) != ' ')
		{
			output += colors[(NumberGenerator(0, 5))] + " \b" + toColor.at(i) + "\u001b[37m";
		}
		else
		{
			output += " ";
		}
	}
	return output;
}

template <typename T, typename Y>
int Index(vector<T> toIndex, Y find)
{
	int index = 0;

	for (int i = 0; i < toIndex.size(); i++)
	{
		if (toIndex.at(i) == find)
		{
			index = i;
			break;
		}
	}

	return index;
}
