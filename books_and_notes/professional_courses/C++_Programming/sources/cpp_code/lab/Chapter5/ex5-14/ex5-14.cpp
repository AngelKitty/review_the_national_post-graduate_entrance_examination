#include <iostream>
using namespace std;

class Boat;
class Car {
private:
	int weight;
public:
	Car(int j) {
		weight = j;
	}
	friend int getTotalWeight(Car &aCar, Boat &aBoat);
};

class Boat {
private:
	int weight;
public:
	Boat(int j) {
		weight = j;
	}
	friend int getTotalWeight(Car &aCar, Boat &aBoat);
};

int getTotalWeight(Car &aCar, Boat &aBoat) {
	return aCar.weight + aBoat.weight;
}

int main() {
	Car c1(4);
	Boat b1(5);

	cout << getTotalWeight(c1, b1) << endl;
 return 0;
}
