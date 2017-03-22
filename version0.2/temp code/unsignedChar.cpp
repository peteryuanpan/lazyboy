#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

string get16 ( unsigned char c ) {
	string s = "";
	for ( int i = 8; i >= 0; i -- ) {
		s += c & ( 1 << i ) ? "1" : "0";
	}
	return s;
}

int main ( ) {
	unsigned char a, b, c;
	for ( int i = 0; i < 100; i ++ ) {
		// c = i;
		// cout << c << " " << i << "\n";
		cout << i << " " << get16 ( i ) << endl;
	}
	//a = 16;
	// b = 17;
	// cout << (a == b) << endl;
	//cout << get16 ( a ) << endl;
	return 0;
}