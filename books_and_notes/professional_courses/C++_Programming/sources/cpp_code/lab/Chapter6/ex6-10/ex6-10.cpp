#include <iostream>
using namespace std;

int main()
{
	int myArray[5];
	int i;
	for ( i=0; i<5; i++)  
	{
		cout << "Value for myArray[" << i << "]: ";
		cin >> myArray[i];
	}
	for (i = 0; i<5; i++)
   		cout << i << ": " << myArray[i] << endl;
	return 0;
}
