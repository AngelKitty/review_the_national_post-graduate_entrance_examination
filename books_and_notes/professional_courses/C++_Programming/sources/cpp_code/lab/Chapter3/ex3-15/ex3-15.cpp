#include <iostream>
using namespace std;

int getPower(int x, int y);
double getPower(double x, int y);

int main()
{
	int iNumber, power;
	double dNumber;
	int iAnswer;
	double dAnswer;
	cout << "Enter an int base number: ";
	cin >> iNumber;
	cout << "Enter a double base number: ";
	cin >> dNumber;
	cout << "To what power? ";
	cin >> power;
	iAnswer = getPower(iNumber,power);
	dAnswer = getPower(dNumber,power);
	cout << iNumber << " to the " << power << "th power is " <<iAnswer << endl;
	cout << dNumber << " to the " << power << "th power is " <<dAnswer << endl;
	return 0;
}

int getPower(int x, int y)
{
    if(y == 1)
       return x;
    else if(y == 0)
       return 1;
    else if(y < 0)
       return 0;
    else
       return (x * getPower(x,y-1));
}

double getPower(double x, int y)
{
    if(y == 1)
       return x;
    else if(y == 0)
       return 1;
    else if(y < 0)
       return 1/ getPower(x,-y);
    else
       return (x * getPower(x,y-1));
}
