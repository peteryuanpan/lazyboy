#include "DataStructure.h"
/*--------队列--------*/
void Queue::clear(){
	t1 = 0, t2 = -1;
}
int Queue::size(){
	return t2 - t1 + 1;
}
bool Queue::empty(){
	return size() == 0;
}
int Queue::front(){
	return q[t1];
}
void Queue::pop(){
	t1++;
}
void Queue::Movet1t2(){
	int t = 0;
	for(int i = t1; i <= t2; i++) q[t++] = q[i];
	t1 = 0, t2 = t - 1;
}
void Queue::push(int x){
	if(t2 + 1 == N) Movet1t2();
	q[++t2] = x;
}


/*--------栈--------*/
void Stack::clear(){
	t1 = 1, t2 = 0;
}
int Stack::size(){
	return t2 - t1 + 1;
}
bool Stack::empty(){
	return size() == 0;
}
int Stack::top(){
	return s[t2];
}
void Stack::pop(){
	t2--;
}
void Stack::push(int x){
	s[++t2] = x;
}
