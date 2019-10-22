#ifndef A3_STACK_H
#define A3_STACK_H

#include "Stack.h"
#include "CardTypes.h"
#include <stdexcept>

template <class T>
Stack<T>::Stack(){

}

template <class T>
Stack<T>::Stack(std::vector<T> s):S(s){

}

template <class T>
Stack<T> Stack<T>::push(T e){
	S.push_back(e);
	return S;
}

template <class T>
Stack<T> Stack<T>::pop(){
	if(size() > 0){
		S.pop_back();
		return S;
	}
	throw std::out_of_range("Stack is empty");
}

template <class T>
T Stack<T>::top(){
	if(size() > 0){
		return S.back();
	}
	throw std::out_of_range("Stack is empty");
}

template <class T>
unsigned int Stack<T>::size(){
	return S.size();
}

template <class T>
std::vector<T> Stack<T>::toSeq(){
	return S;
}
#endif
// Keep this at bottom
template class Stack<CardT>;