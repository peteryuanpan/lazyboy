#ifndef MOVESORT_H_
#define MOVESORT_H_

#include "base.h"
#include "position.h"
#include "search.h"

struct MoveSortStruct {
	int move[128];	// 着法表
	int nMoveIndex;	// 索引下标
	int nMoveNum;	// 着法表长度

	bool HasMove ( const int mv ) {
		for ( int i = 0; i < nMoveNum; i ++ ) {
			if ( move[i] == mv ) {
				return true;
			}
		}
		return false;
	}

	void InitPV ( void ); // 为SearchPV生成着法
	int NextPV ( void ) { // 为SearchPV提供下一个着法
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
