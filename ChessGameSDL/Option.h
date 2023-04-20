#pragma once


#include "ChessPiece.h"

template <typename  T>
class Option {

public:
	Option();
	Option(T data);

	void setData(T data);
	T getData();
	bool isEmpty();
	bool isNotEmpty();
	void Empty();

private:
	T data;
	bool empty;
};


template <class T>
Option<T>::Option() {
	this->empty = true;
}

template <class T>
Option<T>::Option(T data) {
	this->data = data;
	this->empty = false;
}

template <class T>
void Option<T>::setData(T data) {
	this->data = data;
	this->empty = false;
}

template <typename T>
T Option<T>::getData() {
	if (this->empty) {
		throw;
	}
	return this->data;
}

template <class T>
bool Option<T>::isEmpty() {
	return this->empty;
}

template <class T>
bool Option<T>::isNotEmpty() {
	return !this->empty;
}

template <class T>
void Option<T>::Empty() {
	this->empty = true;
}