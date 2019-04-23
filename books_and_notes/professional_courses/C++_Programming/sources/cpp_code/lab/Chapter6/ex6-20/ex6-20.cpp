#include <iostream>
using namespace std;

class SimpleCircle
{
public:
	SimpleCircle();
	SimpleCircle(int);
	SimpleCircle(const SimpleCircle &);
	~SimpleCircle() {}
	
		void setRadius(int);
	int getRadius() const;
	
private:
	int *itsRadius;
};

SimpleCircle::SimpleCircle()
{
	itsRadius = new int(5);
}

SimpleCircle::SimpleCircle(int radius)
{
	itsRadius = new int(radius);
}

SimpleCircle::SimpleCircle(const SimpleCircle & rhs)
{
	int val = rhs.getRadius();
	itsRadius = new int(val);
}

int SimpleCircle::getRadius() const
{
	return *itsRadius;
}

int main()
{
	SimpleCircle CircleOne, CircleTwo(9); 
	cout << "CircleOne: " << CircleOne.getRadius() << endl;
	cout << "CircleTwo: " << CircleTwo.getRadius() << endl;
	return 0;
}