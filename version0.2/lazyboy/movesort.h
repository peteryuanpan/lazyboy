/*
 * movesort.h
 *
 *  Created on: 2017年3月25日
 *      Author: peteryuanpan
 */

#ifndef MOVESORT_H_
#define MOVESORT_H_

#include "base.h"

struct MoveSortStruct {
	int move[128];	// 着法表
	int nMoveIndex;	// 索引下标
	int nMoveNum;	// 着法表长度
	int sortKey[128];	// 排序关键字
	bool duplicated;	// 着法表中存在重复局面

	void InitEnd ( void ); // 为EndSearch生成着法
	int NextEnd ( void ) { // 为NextSearch提供下一个着法
		if ( nMoveIndex < nMoveNum ) {
			nMoveIndex ++;
			return move [ nMoveIndex-1 ];
		}
		else {
			return 0;
		}
	}

	void InitQuiesc ( void ); // 为SearchQuiesc生成着法
	int NextQuiesc ( void ) { // 为SearchQuiesc提供下一个着法
		if ( nMoveIndex < nMoveNum ) {
			nMoveIndex ++;
			return move [ nMoveIndex-1 ];
		}
		else {
			return 0;
		}
	}
};


#endif /* MOVESORT_H_ */
