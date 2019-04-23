#include <iostream>
using namespace std;
enum myColor {
	BLACK, WHITE
};
class Mammal {
public:
	// constructors
	Mammal();
	~Mammal();

	//accessors
	int getAge() const {
		return itsAge;
	}
	void setAge(int age) {
		itsAge = age;
	}
	int getWeight() const {
		return itsWeight;
	}
	void setWeight(int weight) {
		itsWeight = weight;
	}

	//Other methods
	void speak() const {
		cout << "Mammal sound!\n";
	}

protected:
	int itsAge;
	int itsWeight;
};

class Dog: public Mammal {
public:
	Dog();
	~Dog();

	myColor getColor() const {
		return itsColor;
	}
	void setColor(myColor color) {
		itsColor = color;
	}

	void wagTail() {
		cout << "Tail wagging...\n";
	}

private:
	myColor itsColor;
};

Mammal::Mammal() :
	itsAge(1), itsWeight(5) {
	cout << "Mammal constructor...\n";
}

Mammal::~Mammal() {
	cout << "Mammal destructor...\n";
}

Dog::Dog() :
	itsColor(WHITE) {
	cout << "Dog constructor...\n";
}

Dog::~Dog() {
	cout << "Dog destructor...\n";
}
int main() {
	Dog jack;
	jack.speak();
	jack.wagTail();
	cout << " jack is " << jack.getAge() << " years old\n";

	return 0;
}
