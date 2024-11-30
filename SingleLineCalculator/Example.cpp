#include <iostream>
#include <vector>
#include <string>



using namespace std;

int main()
{
    vector<string> toProcess = { "5","*","(","5","+","5",")"};
    vector<int> operatorPositions = { 1,2,4,6};
    vector<int*> operatorOrder;
    for (int i = 0; i < operatorPositions.size(); i++)
    {
        operatorOrder.push_back(&operatorPositions.at(i));
    }

    for (int i = 0; i < operatorPositions.size(); i++)
    {
        if (toProcess.at(operatorPositions.at(i)) == "(" || toProcess.at(operatorPositions.at(i)) == ")")
        {
            int storedPosition = operatorPositions.at(i);

            for (int j = i + 1; j < operatorPositions.size(); j++)
            {
                operatorPositions.at(j)--;
            }

            operatorOrder.erase(operatorOrder.begin() + i);
            operatorPositions.erase(operatorPositions.begin() + i);

            toProcess.erase(toProcess.begin() + storedPosition);
        }
    }

    cout << "Operator Positions: ";
    for (int i = 0; i < operatorPositions.size(); i++)
    {
        cout << operatorPositions.at(i) << " ";
    }
    cout << "\n";
    cout << "Operator Order: ";
    for (int i = 0; i < operatorOrder.size(); i++)
    {
        cout << *operatorOrder.at(i) << " ";
    }
    cout << "\n";
    cout << "toProcess: ";
    for (int i = 0; i < toProcess.size(); i++)
    {
        cout << toProcess.at(i) << " ";
    }
}