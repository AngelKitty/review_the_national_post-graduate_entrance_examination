//11_10.cpp
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	int values[] = { 3, 7, 0, 5, 4 };
	ofstream os("integers", ios_base::out | ios_base::binary);
	os.write(reinterpret_cast<char *>(values), sizeof(values));
	os.close();

	ifstream is("integers", ios_base::in | ios_base::binary);
	if (is) {
		is.seekg(3 * sizeof(int));
		int v;
		is.read(reinterpret_cast<char *>(&v), sizeof(int));
		cout << "The 4th integer in the file 'integers' is " << v << endl;
	} else {
		cout << "ERROR: Cannot open file 'integers'." << endl;
	}
	is.close();

	return 0;
}
