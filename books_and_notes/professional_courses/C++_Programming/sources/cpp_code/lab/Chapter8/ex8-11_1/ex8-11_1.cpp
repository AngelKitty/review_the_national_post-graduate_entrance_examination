#include <iostream>
using namespace std;

class Shape
{
public:
    Shape(){}
    ~Shape(){}
    virtual float getArea() =0 ;
    virtual float getPerim () =0 ;
    int getVertexCount() const;
};

class Circle : public Shape
{
public:
    Circle(float radius):itsRadius(radius){}
    ~Circle(){}
    float getArea() { return 3.14 * itsRadius * itsRadius; }
	float getPerim () { return 6.28 * itsRadius; }
	
private:
    float itsRadius;
};

class Rectangle : public Shape
{
public:
    Rectangle(float len, float width): itsLength(len), itsWidth(width){};
    ~Rectangle(){};
    virtual float getArea() { return itsLength * itsWidth; }
	float getPerim () { return 2 * itsLength + 2 * itsWidth; }
    virtual float GetLength() { return itsLength; }
    virtual float GetWidth() { return itsWidth; }
protected:
    float itsWidth;
    float itsLength;
};

class Square: public Rectangle
{
public:
    Square (float len): Rectangle(len, len){};
    ~Square(){};
    virtual float getArea() { return itsLength * itsWidth; }
	float getPerim () { return 2 * itsLength + 2 * itsWidth; }
    virtual float GetLength() { return itsLength; }
    virtual float GetWidth() { return itsWidth; }
};

int Shape::getVertexCount() const
{
		if (dynamic_cast<Circle*>(const_cast<Shape*>(this)) != 0)
			return 0;
		else if (dynamic_cast<Rectangle*>(const_cast<Shape*>(this)) != 0 || dynamic_cast<Square*>(const_cast<Shape*>(this)) != 0)
			return 4;
		else
			return -1;
}

int main()
{
 	Shape * sp;
	
	sp = new Circle(5);
	cout << "The area of the Circle is " << sp->getArea () << endl;
	cout << "The perimeter of the Circle is " << sp->getPerim () << endl;
	cout << "The vertex count of the Circle is " << sp->getVertexCount() << endl;
	delete sp;
	sp = new Rectangle(4,6);
	cout << "The area of the Rectangle is " << sp->getArea() << endl;
	cout << "The perimeter of the Rectangle is " << sp->getPerim () << endl;
	cout << "The vertex count of the Rectangle is " << sp->getVertexCount() << endl;
	delete sp;
	sp = new Square(6);
	cout << "The area of the Square is " << sp->getArea() << endl;
	cout << "The perimeter of the Square is " << sp->getPerim () << endl;
	cout << "The vertex count of the Square is " << sp->getVertexCount() << endl;
	delete sp;
	return 0;
}
