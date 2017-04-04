/*
 * base.h
 *
 *  Created on: 2017年3月25日
 *      Author: peteryuanpan
 */

#ifndef BASE_H_
#define BASE_H_

typedef long long LL;
typedef unsigned long long ULL;

#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <ctime>

template <typename T> inline T MIN ( T Arg1, T Arg2 ) {
	return Arg1 < Arg2 ? Arg1 : Arg2;
}

template <typename T> inline T MAX ( T Arg1, T Arg2 ) {
	return Arg1 > Arg2 ? Arg1 : Arg2;
}

template <typename T> inline void SWAP ( T & Arg1, T & Arg2 ) {
	T temp = Arg1;
	Arg1 = Arg2;
	Arg2 = temp;
}

#include "position.h"
#include "hash.h"
#include "movesort.h"
#include "evaluate.h"
#include "search.h"
#include "ucci.h"
#include "lazyboy.h"

#endif /* BASE_H_ */
