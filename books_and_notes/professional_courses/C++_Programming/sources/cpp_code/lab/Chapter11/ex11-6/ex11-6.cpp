#include <iostream>
#include <fstream>
using namespace std;

class Dog {
public:
	Dog(int weight, long days) :
		itsWeight(weight), itsNumberDaysAlive(days) {
	}
	~Dog() {
	}

	int getWeight() const {
		return itsWeight;
	}
	void setWeight(int weight) {
		itsWeight = weight;
	}

	long getDaysAlive() const {
		return itsNumberDaysAlive;
	}
	void setDaysAlive(long days) {
		itsNumberDaysAlive = days;
	}

private:
	int itsWeight;
	long itsNumberDaysAlive;
};

int main() // returns 1 on error
{
	char fileName[80];

	cout << "Please enter the file name: ";
	cin >> fileName;
	ofstream fout(fileName);
	if (!fout) {
		cout << "Unable to open " << fileName << " for writing.\n";
		return (1);
	}

	Dog dog1(5, 10);
	fout.write((char*) &dog1, sizeof dog1);

	fout.close();

	ifstream fin(fileName);
	if (!fin) {
		cout << "Unable to open " << fileName << " for reading.\n";
		return (1);
	}

	Dog dog2(2, 2);

	cout << "dog2 weight: " << dog2.getWeight() << endl;
	cout << "dog2 days: " << dog2.getDaysAlive() << endl;

	fin.read((char*) &dog2, sizeof dog2);

	cout << "dog2 weight: " << dog2.getWeight() << endl;
	cout << "dog2 days: " << dog2.getDaysAlive() << endl;
	fin.close();
	return 0;
}
