//Point.h
#ifndef _POINT_H
#define _POINT_H

class Point {	//Point类的定义
public:	//外部接口
	Point(float x = 0, float y = 0) : x(x), y(y) { }
	float getX() const { return x; }
	float getY() const { return y; }
private:	//私有数据成员
	float x, y;
};
#endif	//_POINT_H
