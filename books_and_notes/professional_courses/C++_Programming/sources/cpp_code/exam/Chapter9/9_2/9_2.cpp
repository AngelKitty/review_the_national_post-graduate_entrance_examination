//9_2.cpp
#include <iostream>
#include <cstdlib>
using namespace std;

struct Student {	// 结构体Student
	int id;		//学号
	float gpa;	//平均分
};  

template <class T>  //类模板：实现对任意类型数据进行存取
class Store {
private:
	T item;			// item用于存放任意类型的数据
	bool haveValue;	// haveValue标记item是否已被存入内容
public:
	Store();		// 缺省形式（无形参）的构造函数
	T &getElem();	//提取数据函数
	void putElem(const T &x);  //存入数据函数
};

//以下实现各成员函数。
template <class T>	//缺省构造函数的实现 
Store<T>::Store(): haveValue(false) { }

template <class T>                //提取数据函数的实现
T &Store<T>::getElem() {
	if (!haveValue) {	//如果试图提取未初始化的数据，则终止程序
		cout << "No item present!" << endl;
		exit(1);	//使程序完全退出，返回到操作系统。
		//参数可用来表示程序终止的原因，可以被操作系统接收
	}
	return item;    // 返回item中存放的数据 
}

template <class T>	//存入数据函数的实现 
void Store<T>::putElem(const T &x) {
	haveValue = true;	// 将haveValue 置为true，表示item中已存入数值
	item = x;			// 将x值存入item
}

int main() {
	Store<int> s1, s2;	//定义两个Store<int>类对象，其中数据成员item为int类型
	s1.putElem(3);	//向对象S1中存入数据（初始化对象S1）
	s2.putElem(-7);	//向对象S2中存入数据（初始化对象S2）
	cout << s1.getElem() << "  " << s2.getElem() << endl;	//输出对象S1和S2的数据成员

	Student g = { 1000, 23 };	//定义Student类型结构体变量的同时赋以初值
	Store<Student> s3;	//定义Store<Student>类对象s3，其中数据成员item为Student类型
	s3.putElem(g); //向对象D中存入数据（初始化对象D）
	cout << "The student id is " << s3.getElem().id << endl;	//输出对象s3的数据成员

	Store<double> d;	//定义Store<double>类对象s4，其中数据成员item为double类型
	cout << "Retrieving object d... ";
	cout << d.getElem() << endl;  //输出对象D的数据成员
	//由于d未经初始化,在执行函数D.getElement()过程中导致程序终止

	return 0;
}
