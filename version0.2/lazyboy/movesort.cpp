/*
 * movesort.cpp
 *
 *  Created on: 2017年3月25日
 *      Author: peteryuanpan
 */

#include "base.h"

void MoveSortStruct::InitEnd ( void ) { // 为EndSearch生成着法
	nMoveIndex = 0;
	nMoveNum = 0;

	// 1.生成所有着法
	nMoveNum += search.pos.GenAllMove ( move + nMoveNum );

	// 2.给每个局面赋值上排序关键字
	for ( int i = 0; i < nMoveNum; i ++ ) {
		sortKey[i] = 0;
	}
	for ( int i = 0; i < nMoveNum; i ++ ) {
		search.pos.MakeMove ( move[i] );
		if ( search.pos.Check() ) { // 将军着法
			sortKey[i] = 3;
		}
		search.pos.UndoMakeMove ( move[i] );
		if ( sortKey[i] != 3 ) {
			if ( search.pos.NonEmpty(DST(move[i])) ) { // 吃子着法
				sortKey[i] = 2;
			}
			else { // 非吃子着法
				sortKey[i] = 1;
			}
		}
	}

	// 3.按 将军着法、吃子着法、非吃子着法 这样的排序关键字排序
	int t = 0;
	for ( int sortkey = 3; sortkey >= 1; sortkey -- ) {
		for ( int i = t; i < nMoveNum; i ++ ) {
			if ( sortKey[i] == sortkey ) {
				SWAP ( move[i], move[t] );
				t ++;
			}
		}
	}
}

void MoveSortStruct::InitQuiesc ( void ) { // 为SearchQuiesc生成着法
 // ....
}
