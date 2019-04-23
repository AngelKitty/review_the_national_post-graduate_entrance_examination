#include <iostream>
using namespace std;

enum Core { Single, Dual, Quad };
enum Words { Bit32, Bit64 };
enum HyperThread { Support, NotSupport };
class CPU {
public:
	CPU(unsigned frequence, Core type, Words length, HyperThread mode)
		: frequence (frequence), CoreType (type), WordLen (length), mode(mode) { }
	void show();
private:
	unsigned frequence: 32;
	Core CoreType: 3;
	Words WordLen: 2;
	HyperThread mode: 2;
};

void CPU::show() {
	cout << " Frequence:    " << frequence << endl;
	cout << " Core:     ";
	switch ((unsigned)CoreType) {
		case Single:  cout << " Single-Core"; break;
		case Dual:	cout << " Dual-Core"; break;
		case Quad:	cout << " Quad-Core"; break;
	}
	cout << endl;
	cout << " Words:     ";
	switch ((unsigned)WordLen) {
		case Bit32:	cout << "32-bits len"; break;
		case Bit64:	cout << "64-bits len"; break;
	}
	cout << endl;
	cout << " HyperThread:     ";
	switch ((unsigned)mode) {
		case Support:		cout << "Support Hyper-Thread"; break;
		case NotSupport:	cout << "Not Support Hyper-Thread"; break;
	}
	cout << endl;
}

int main() {
	CPU c(3000000000, Quad, Bit64,Support);
	cout << "Size of Class CPU: " << sizeof(CPU) << endl;
	c.show();
	return 0;
}
