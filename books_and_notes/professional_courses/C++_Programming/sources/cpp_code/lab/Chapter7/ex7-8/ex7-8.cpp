#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class Document {
public:
	Document() {
	}
	Document(char *nm);
	char *name; // Document name.
	void PrintNameOf(); // Print name.
};
Document::Document(char *nm) {
	name = new char[strlen(nm) + 1];
	strcpy(name, nm);
};
void Document::PrintNameOf() {
	cout << name << endl;
}

class Book: public Document {
public:
	Book(char *nm, long pagecount);
	void PrintNameOf();
private:
	long pageCount;
};
Book::Book(char *nm, long pagecount) :
	Document(nm) {
	pageCount = pagecount;
}
void Book::PrintNameOf() {
	cout << "Name of book: ";
	Document::PrintNameOf();
}

int main() {
	Document a("Document1");
	Book b("Book1", 100);
	b.PrintNameOf();
}
