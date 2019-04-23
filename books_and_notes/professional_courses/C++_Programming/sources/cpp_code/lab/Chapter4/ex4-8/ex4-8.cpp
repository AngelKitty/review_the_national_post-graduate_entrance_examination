#include <iostream>
using namespace std;

class Dog {
public:
	Dog(int initialAge = 0, int initialWeight = 5);
	~Dog();
	int getAge() {
		return age;
	}
	void setAge(int age) {
		this->age = age;
	}
	int getWeight() {
		return weight;
	}
	void setWeight(int weight) {
		this->weight = weight;
	}
private:
	int age, weight;
};

Dog::Dog(int initialAge, int initialWeight) {
	age = initialAge;
	weight = initialWeight;
}

Dog::~Dog() //析构函数，不做任何工作
{
}

int main() {
	Dog Jack(2, 10);
	cout << "Jack is a Dog who is ";
	cout << Jack.getAge() << " years old and " << Jack.getWeight() << " pounds weight" << endl;
	Jack.setAge(7);
	Jack.setWeight(20);
	cout << "Now Jack is ";
	cout << Jack.getAge() << " years old and " << Jack.getWeight() << " pounds weight." << endl;

	return 0;
}
