//11_8.cpp
#include <iostream>
#include <string>
using namespace std;

int main() {
    string line;
    cout << "Type a line terminated by 't' " << endl; 
	getline(cin, line, 't');
    cout << line << endl;
	return 0;
}
