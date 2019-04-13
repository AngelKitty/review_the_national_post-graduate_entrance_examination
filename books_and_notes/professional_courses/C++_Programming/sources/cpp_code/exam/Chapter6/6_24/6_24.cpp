#include <iostream>
#include <string>
using namespace std;

int main() {
	for (int i = 0; i < 2; i++)	{
		string city, state;
		getline(cin, city, ',');
		getline(cin, state);
		cout << "City: " << city << "   State: " << state << endl;
	}
	return 0;
}
