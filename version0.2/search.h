#ifndef SEARCH_H_
#define SEARCH_H_

#include "base.h"
#include "position.h"
#include "rollback.h"
#include "evaluate.h"

extern PositionStruct pos; // 当前搜索局面
extern RollBackListStruct roll; // 回滚着法表

const int NULL_DEPTH = 2; // 空着裁剪减少的深度

// 搜索结构体
const int nBest = 128;
struct SearchStruct {
	int bmv[nBest]; // 最佳着法
	int bvl[nBest]; // 最优得分
	int nNode; // 总节点数
	int nBeta; // beta点个数
};
extern SearchStruct Search;

inline void ClearBmvBvl ( int *bmv, int *bvl ) {
	for ( int i = 0; i < nBest; i ++ ) {
		bmv[i] = 0;
		bvl[i] = - MATE_VALUE;
	}
}

inline void InitSearchStruct ( void ) {
	ClearBmvBvl ( Search.bmv, Search.bvl );
	Search.nNode = 0;
	Search.nBeta = 0;
}

inline void CopyBmvBvl ( int *bmv1, int *bvl1, const int *bmv2, const int *bvl2 ) {
	for ( int i = 0; i < nBest; i ++ ) {
		bmv1[i] = bmv2[i];
		bvl1[i] = bvl2[i];
	}
}

inline void UpdateBmvBvl ( int *bmv, int *bvl, const int mv, const int vl ) {
	for ( int i = 0; i < nBest ; i ++ ) {
		if ( vl > bvl[i] ) {
			for ( int j = nBest - 1; j > i; j -- ) {
				bvl[j] = bvl[j-1];
				bmv[j] = bmv[j-1];
			}
			bmv[i] = mv;
			bvl[i] = vl;
			break;
		}
	}
}

int SearchMain ( void );

#endif /* SEARCH_H_ */
