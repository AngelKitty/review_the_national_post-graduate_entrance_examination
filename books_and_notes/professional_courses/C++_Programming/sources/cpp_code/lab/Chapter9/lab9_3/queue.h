#ifndef QUEUE_H_
#define QUEUE_H_

#include "link.h"

template <class T>
class Queue : public LinkedList<T>
{
public:
	Queue<T>() {LinkedList<T>();}
	void push(const T& item) {LinkedList<T>::insertRear(item);}
	T pop() {return LinkedList<T>::deleteFront();}
};

#endif  //QUEUE_H_
