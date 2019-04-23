#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
	ifstream in;
	in.open(argv[1], ios::binary);
	if (!in) {
		cout << "Cannot open file.";
		return 1;
	}
	const int bsz = 1024;
	char buf[bsz];
	int line = 0;
	while(in.getline(buf, bsz)) {
		cout << ++line << ": " << buf << endl;
	}
	return 0;
}