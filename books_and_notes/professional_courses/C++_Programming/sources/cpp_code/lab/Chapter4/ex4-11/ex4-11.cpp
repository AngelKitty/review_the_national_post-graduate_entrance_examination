#include <iostream>
using namespace std;

class Rectangle {
public:
	Rectangle(float l, float w) {
		length = l;
		width = w;
	}
	~Rectangle() {
	}
	float getArea() {
		return length * width;
	}
	float getlength() {
		return length;
	}
	float getwidth() {
		return width;
	}
private:
	float length;
	float width;
};

int main() {
	float length, width;
	cout << "请输入矩形的长度：";
	cin >> length;
	cout << "请输入矩形的宽度：";
	cin >> width;
	Rectangle r(length, width);
	cout << "长为" << length << "宽为" << width << "的矩形的面积为：" << r.getArea()
			<< endl;
	return 0;
}
