#include <iostream>
using namespace std;

class Cat {
public:
	Cat(int age) :
		itsAge(age) {
		numOfCats++;
	}
	virtual ~Cat() {
		numOfCats--;
	}
	virtual int getAge() {
		return itsAge;
	}
	virtual void setAge(int age) {
		itsAge = age;
	}
	static int getNumOfCats() {
		return numOfCats;
	}
private:
	int itsAge;
	static int numOfCats;
};

int Cat::numOfCats = 0;

void telepathicFunction();

int main() {
	const int maxCats = 5;
	Cat *catHouse[maxCats];
	int i;
	for (i = 0; i < maxCats; i++) {
		catHouse[i] = new Cat(i);
		telepathicFunction();
	}

	for (i = 0; i < maxCats; i++) {
		delete catHouse[i];
		telepathicFunction();
	}
	return 0;
}

void telepathicFunction() {
	cout << "There are " << Cat::getNumOfCats() << " cats alive!\n";
}
