#include <iostream>
using namespace std;

class Object {
private:
	int weight;
public:
	Object() {
		cout << "构造Object对象" << endl;
		weight = 0;
	}
	int getWeight() {
		return weight;
	}
	void setWeight(int n) {
		weight = n;
	}
	~Object() {
		cout << "析构Object对象" << endl;
	}
};

class Box: public Object {
private:
	int height, width;
public:
	Box() {
		cout << "构造Box对象" << endl;
		height = width = 0;
	}
	int getHeight() {
		return height;
	}
	void setHeight(int n) {
		height = n;
	}
	int getWidth() {
		return width;
	}
	void setWidth(int n) {
		width = n;
	}
	~Box() {
		cout << "析构Box对象" << endl;
	}
};

int main() {
	Box a;
	return 0;
}
