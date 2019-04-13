//Trapzint.h  文件一，类定义

class Function {	//抽象类Function的定义
public:
	virtual double operator () (double x) const = 0;	//纯虚函数重载运算符()
	virtual ~Function() { }
};

class MyFunction: public Function {	//公有派生类MyFunction定义
public:
	virtual double operator()(double x) const;	//覆盖虚函数
};

class Integration {	//抽象类Integration定义
public:
	virtual double operator () (double a, double b, double eps) const = 0;
	virtual ~Integration() { }
};

class Trapz: public Integration	{	//公有派生类Trapz定义
public:
	Trapz(const Function &f) : f(f) {}	//构造函数
	virtual double operator ()(double a, double b,double eps) const;
private:
	const Function &f;	//私有成员，Function类对象的指针
};	
