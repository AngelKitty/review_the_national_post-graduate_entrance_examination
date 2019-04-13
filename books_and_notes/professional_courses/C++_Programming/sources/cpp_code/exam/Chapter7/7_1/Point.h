//Point.h
#ifndef _POINT_H
#define _POINT_H

class Point {	//基类Point类的定义
public:	//公有函数成员
	void initPoint(float x = 0, float y = 0) { this->x = x; this->y = y;}
	void move(float offX, float offY) { x += offX; y += offY; }
	float getX() const { return x; }
	float getY() const { return y; }
private:	//私有数据成员
	float x, y;
};

#endif //_POINT_H
