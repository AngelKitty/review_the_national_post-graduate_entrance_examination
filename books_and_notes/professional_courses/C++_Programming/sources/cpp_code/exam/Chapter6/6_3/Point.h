//Point.h
#ifndef _POINT_H
#define _POINT_H

class Point {	//类的定义
public:	//外部接口
	Point();
	Point(int x, int y);
	~Point();
	void move(int newX,int newY);
	int getX() const { return x; }
	int getY() const { return y; }
	static void showCount();	//静态函数成员
private:	//私有数据成员
	int x, y;
};

#endif	//_POINT_H
