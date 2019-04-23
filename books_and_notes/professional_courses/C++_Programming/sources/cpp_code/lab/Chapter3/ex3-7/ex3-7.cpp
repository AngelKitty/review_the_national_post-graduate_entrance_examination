#include <iostream>
using namespace std;

typedef unsigned short int USHORT;
short int divide(USHORT a, USHORT b)
{
    if (b == 0)
          return -1;
    else
          return a/b;
}


int main()
{
    USHORT one, two;
    short int answer;
    cout << "Enter two numbers.\n Number one: ";
    cin >> one;
    cout << "Number two: ";
    cin >> two;
    answer = divide(one, two);
    if (answer > -1)
       cout << "Answer: " << answer;
    else
       cout << "Error, can't divide by zero!";
	return 0;
}
