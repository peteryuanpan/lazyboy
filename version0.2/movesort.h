#ifndef MOVESORT_TYPE_H_
#define MOVESORT_TYPE_H_

#include "base.h"
#include "position.h"

// 着法分类
const int SORT_TYPE_BEST_MOVE = 2;
const int SORT_TYPE_GOOD_CAP = 1;
const int SORT_TYPE_OTHER = 0;
const int MAX_SORT_TYPE = SORT_TYPE_BEST_MOVE;

// 清空历史表
void ClearHistoryTable ( void );

// 更新历史表
void InsertHistoryTable ( const int mv, const int depth );

// 着法排序结构体
struct MoveSortStruct {
	int move[128];	// 着法表
	int nMoveIndex;	// 索引下标
	int nMoveNum;	// 着法表长度

	int InitMove ( void ); // 生成着法
	int InitGoodCapMove ( void ); // 生成好的吃子着法

	int NextMove ( void ) { // 提供下一个着法
		if ( nMoveIndex < nMoveNum ) {
			nMoveIndex ++;
			return move [ nMoveIndex-1 ];
		}
		else {
			return 0;
		}
	}
};


#endif /* MOVESORT_TYPE_H_ */
