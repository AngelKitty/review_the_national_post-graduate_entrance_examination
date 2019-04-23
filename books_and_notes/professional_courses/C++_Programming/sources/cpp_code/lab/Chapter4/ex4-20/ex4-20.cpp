#include <iostream>
using namespace std;

class Complex {
public:
	Complex(double r, double i) :
		real(r), image(i) {
	}
	Complex(double r) :
		real(r), image(0) {
	}
	void show();
	void add(Complex c2);

private:
	double real;
	double image;
};
void Complex::add(Complex c2) {
	real += c2.real;
	image += c2.image;
}

void Complex::show() {
	cout << real << " + ";
	cout << image << "i";
	cout << endl;
}

int main() {
	Complex c1(1.5, 2.5);
	Complex c2 = 4.5;
	c1.show();
	c1.add(c2);
	c1.show();
	return 0;
}
