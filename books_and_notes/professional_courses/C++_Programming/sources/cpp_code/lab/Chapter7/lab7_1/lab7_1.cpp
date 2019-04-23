#include <iostream>
using namespace std;

class Animal {
	//int age;
public:
	int age;
};

class Dog : Animal {
public:
	void SetAge(int n) {age = n;}
};

int main()
{
	Dog d;
	d.SetAge(2);
	return 0;
}
