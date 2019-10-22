#ifndef A3_STACK_H_
#define A3_STACK_H_

#include <vector>

template <class T>

class Stack{
private:
	std::vector<T> S;	

public:
	Stack ();
	Stack (std::vector<T> s);
	Stack<T> push(T e);
	Stack<T> pop();
	T top();
	unsigned int size();
	std::vector<T> toSeq();
};
#endif
