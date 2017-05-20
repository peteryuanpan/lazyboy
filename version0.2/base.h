#ifndef BASE_H_
#define BASE_H_

typedef long long LL;
typedef unsigned long long ULL;

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include <cctype>
#include <cassert>

template < typename T > inline T MIN ( T Arg1, T Arg2 ) {
	return Arg1 < Arg2 ? Arg1 : Arg2;
}

template < typename T > inline T MAX ( T Arg1, T Arg2 ) {
	return Arg1 > Arg2 ? Arg1 : Arg2;
}

template < typename T > inline void SWAP ( T & Arg1, T & Arg2 ) {
	T temp = Arg1;
	Arg1 = Arg2;
	Arg2 = temp;
}

#endif /* BASE_H_ */
