#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "HeadFile.h"
/*--------队列--------*/
class Queue{
private:
	int t1, t2, q[N];
	void Movet1t2();
public:
	void clear();
	int size();
	bool empty();
	int front();
	void pop();
	void push(int x);
};

/*--------栈--------*/
class Stack{
private:
	int t1, t2, s[N];
public:
	void clear();
	int size();
	bool empty();
	int top();
	void pop();
	void push(int x);
};
#endif
