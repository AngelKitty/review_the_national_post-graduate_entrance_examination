#include <iostream>
using namespace std;

class Tree {
	int ages;
public:
	Tree(int n=0);
	~Tree();
	void grow(int years);
	void age();
};

Tree::Tree(int n) {
	ages = n;
}

Tree::~Tree() {
	age();
}

void Tree::grow(int years) {
	ages += years;
}

void Tree::age() {
	cout << "这棵树的年龄为" << ages << endl;
}

int main()
{
    Tree t(12);

    t.age();
    t.grow(4);
	return 0;
}
