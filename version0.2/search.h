#ifndef SEARCH_H_
#define SEARCH_H_

#include "base.h"
#include "position.h"
#include "rollback.h"
#include "evaluate.h"

extern int DEP_LIMIT;
extern int BVL_LIMIT;

extern PositionStruct pos; // 当前搜索局面
extern RollBackListStruct roll; // 回滚着法表

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

// わたしの小さい搜索树
const int NNODE = 1000010;
struct MyTreeStruct {
	int to;
	int mv;
	int vl;
	int next;
};
extern MyTreeStruct MyTree[NNODE];
extern int head[NNODE];
extern int nNode;
extern int nEdge;

inline void InitMyTreeStruct ( void ) {
	for ( int i = 0; i < NNODE; i ++ ) {
		head[i] = -1;
		MyTree[i].to = 0;
		MyTree[i].mv = 0;
		MyTree[i].next = -1;
	}
	nNode = 1;
	nEdge = 0;
}

inline void AddEdge ( const int a, const int b, const int mv, const int vl ) {
	if ( nEdge + 1 < NNODE ) {
		for ( int i = head[a]; i != -1; i = MyTree[i].next ) {
			if ( MyTree[i].mv == mv ) {
				return;
			}
		}
		int t = ++ nEdge;
		MyTree[t].to = b;
		MyTree[t].mv = mv;
		MyTree[t].vl = vl;
		MyTree[t].next = head[a];
		head[a] = t;
	}
}

inline void AddEdge ( const int a, const int depth, const int *bmv, const int *bvl ) {
	for ( int i = nBest - 1; i >= 0; i -- ) {
		if ( bvl[0] - bvl[i] <= BVL_LIMIT && bmv[i] != 0 && bvl[i] > - BAN_VALUE ) {
			++ nNode;
			AddEdge ( a, nNode, bmv[i], bvl[i] );
		}
	}
}

inline void DelEdge ( const int a, const int mv ) {
	int last = -1;
	for ( int i = head[a]; i != -1; i = MyTree[i].next ) {
		if ( MyTree[i].mv == mv ) {
			if ( last == -1 ) {
				head[a] = MyTree[i].next;
			}
			else {
				MyTree[last].next = MyTree[i].next;
			}
			break;
		}
		last = i;
	}
}

inline void DelEdge ( const int a, const int depth, const int *bmv, const int *bvl ) {
	const int d = MAX ( 2, BVL_LIMIT - ( depth - DEP_LIMIT ) );
	for ( int i = 0; i < nBest; i ++ ) {
		if ( bvl[0] - bvl[i] > d && bmv[i] != 0 && bvl[i] > - BAN_VALUE ) {
			DelEdge ( a, bmv[i] );
		}
	}
}

// Search Function
int SearchCut ( int depth, int beta );
int SearchAlphaBeta ( int depth, int alpha, int beta );
int SearchMain ( void );

#endif /* SEARCH_H_ */