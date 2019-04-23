#ifndef NODE_H_
#define NODE_H_

template <class T>
class Node {
private:
	T data;
public:
	Node* next;
	Node();
	Node(const T& data, Node<T>* nt = 0);
//	Node(T data, Node<T>* n = NULL);
	T& getData();
};

template <class T>
Node<T>::Node() {
	data = 0;
	next = NULL;
}

template <class T>
Node<T>::Node(const T& d, Node<T>* nt) {
	data = d;
	next = nt;
}

template <class T>
T& Node<T>::getData() {
	return data;
}

#endif //NODE_H_