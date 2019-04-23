#include <fstream>
#include <strstream>
#include <cstdlib>
using namespace std;

int main(int argc, char* argv[])
{
	
	strstream textfile;
	{
		ifstream in(argv[1]);
        textfile << in.rdbuf();
	}
	ofstream out(argv[2]); 
	
	const int bsz = 100;
	char buf[bsz];
	int line = 0;
	while(textfile.getline(buf, bsz)) {
		out.setf(ios::right, ios::adjustfield);
		out.width(1);
		out << ++line << ". " << buf << endl;
	}
	return 0;
}
