#include "search.h"
#include "base.h"
#include "hash.h"
#include "position.h"
#include "rollback.h"
#include "movesort.h"
#include "evaluate.h"
#include "debug.h"
#include "time.h"

PositionStruct pos;
RollBackListStruct roll;
SearchStruct Search;

// 超时
bool TimeOut ( void ) {
	return TimeOut ( THIS_SEARCH_TIME ) || TimeOut ( SEARCH_TOTAL_TIME );
}

// 无害裁剪
int HarmlessPruning ( void ) {
	// 1. 和局局面
	if ( pos.IsDraw() ) {
		return 0; // eleeye上表示，为了安全起见，不用pos.DrawValue()
	}

	// 2. 路径重复
	const int vRep = roll.RepStatus ();
	if ( vRep != REP_NONE ) {
		return roll.RepValue ( vRep );
	}

	return - MATE_VALUE;
}

// 静态搜索
int SearchQuiesc ( int alpha, int beta ) {
	int mv, vl;
	int bvl = - MATE_VALUE;
	MoveSortStruct mvsort;

	if ( TimeOut() ) { // 超时
		return bvl;
	}

	// 无害裁剪
	vl = HarmlessPruning ();
	if ( vl > - MATE_VALUE ) {
		return vl;
	}

	// 置换裁剪
	vl = QueryValueInHashTableQC ( alpha, beta );
	if ( vl > - MATE_VALUE ) {
		return vl;
	}

	// 生成着法
	if ( pos.checked ) {
		mvsort.InitMove ();
	}
	else {
		bvl = pos.Evaluate ();
		if ( bvl >= beta ) {
			InsertInfoToHashTableQC ( bvl, MATE_VALUE );
			return bvl;
		}
		if ( bvl > alpha ) {
			alpha = bvl;
		}
		mvsort.InitGoodCapMove ();
	}

	// 大搜索
	while ( (mv = mvsort.NextMove()) != 0 ) {
		pos.MakeMove (mv);
		vl = - SearchQuiesc ( -beta, -alpha );
		pos.UndoMakeMove ();

		if ( TimeOut() ) { // 超时
			return bvl;
		}

		if ( vl > bvl ) {
			bvl = vl;
			if ( bvl >= beta ) {
				if ( bvl > - BAN_VALUE && bvl < BAN_VALUE ) {
					InsertInfoToHashTableQC ( bvl, MATE_VALUE );
				}
				return vl;
			}
			if ( bvl > alpha ) {
				alpha = bvl;
			}
		}
	}

	// 最后
	if ( bvl > - BAN_VALUE && bvl < BAN_VALUE ) {
		InsertInfoToHashTableQC ( bvl, bvl );
	}
	return bvl;
}

// 零窗口搜索
int SearchCut ( int depth, int beta, bool bNoNull = false ) {
	int mv, vl;
	int bmv = 0;
	int bvl = - MATE_VALUE;
	MoveSortStruct mvsort;

	if ( TimeOut() ) { // 超时
		return bvl;
	}

	// 无害裁剪
	vl = HarmlessPruning ();
	if ( vl > - MATE_VALUE ) {
		return vl;
	}

	// 置换裁剪
	vl = QueryValueInHashTable ( depth, beta - 1, beta );
	if ( vl > - MATE_VALUE ) {
		return vl;
	}

	// 达到极限深度
	if ( depth <= 0 ) {
		return SearchQuiesc ( beta - 1, beta );
	}

	// 空着裁剪 : 大切に
	if ( !bNoNull && pos.checked == false && pos.NullOkay() ) {
		pos.NullMove ();
		vl = - SearchCut ( depth - NULL_DEPTH - 1, beta, true );
		pos.UndoNullMove ();
		if ( TimeOut() ) { // 超时
			return bvl;
		}

		if ( vl >= beta ) {
			if ( pos.NullSafe() ) {
				InsertInfoToHashTable ( MAX(depth, NULL_DEPTH + 1), 0, vl, HASH_TYPE_BETA );
				return vl;
			}
			else if ( SearchCut(depth - NULL_DEPTH, beta, true) >= beta ) {
				InsertInfoToHashTable ( MAX(depth, NULL_DEPTH), 0, vl, HASH_TYPE_BETA );
				return vl;
			}
		}
	}

	// 生成着法
	int nMoveNum = mvsort.InitMove ();
	Search.nNode ++;

	// 大搜索
	while ( (mv = mvsort.NextMove()) != 0 ) {
		pos.MakeMove ( mv );
		int newDepth = ( pos.checked || nMoveNum == 1 ) ? depth : depth - 1;
		vl = - SearchCut ( newDepth, 1 - beta );
		pos.UndoMakeMove ();

		if ( TimeOut() ) { // 超时
			return bvl;
		}

		// 边界
		if ( vl > bvl ) {
			bvl = vl;
			bmv = mv;
			if ( bvl >= beta ) {
				Search.nBeta ++;
				InsertInfoToHashTable ( depth, bmv, bvl, HASH_TYPE_BETA );
				InsertHistoryTable ( bmv, depth );
				return bvl;
			}
		}
	}

	// 最后
	InsertInfoToHashTable ( depth, bmv, bvl, HASH_TYPE_ALPHA );
	return bvl;
}

// Alpha-Beta 搜索
int SearchAlphaBeta ( int depth, int alpha, int beta ) {
	int mv, vl;
	int bmv[nBest], bvl[nBest];
	ClearBmvBvl ( bmv, bvl );
	int hash_type = HASH_TYPE_ALPHA;
	MoveSortStruct mvsort;

	if ( TimeOut() ) { // 超时
		return bvl[0];
	}

	// 无害裁剪
	vl = HarmlessPruning ();
	if ( vl > - MATE_VALUE ) {
		return vl;
	}

	// 置换裁剪
	vl = QueryValueInHashTable ( depth, alpha, beta );
	if ( vl > - MATE_VALUE ) {
		Search.bmv[0] = QueryMoveInHashTable ( depth, alpha, beta );
		Search.bvl[0] = vl;
		return vl;
	}

	// 达到极限深度
	if ( depth <= 0 ) {
		return SearchQuiesc ( alpha, beta );
	}

	// 内部迭代加深启发
	if ( depth > 2 ) {
		mv = QueryMoveInHashTableWithoutLimit ();
		if ( mv == 0 ) {
			SearchAlphaBeta ( depth / 2, alpha, beta );
			if ( TimeOut() ) { // 超时
				return bvl[0];
			}
		}
	}

	// 生成着法
	int nMoveNum = mvsort.InitMove ();
	Search.nNode ++;

	// 大搜索
	while ( (mv = mvsort.NextMove()) != 0 ) {
		pos.MakeMove ( mv );
		int newDepth = ( pos.checked || nMoveNum == 1 ) ? depth : depth - 1;
		vl = - SearchCut ( newDepth, - alpha );
		if ( vl > alpha && vl < beta ) {
			vl = - SearchAlphaBeta ( newDepth, -beta, -alpha );
		}
		pos.UndoMakeMove ();

		if ( TimeOut() ) {
			return bvl[0];
		}

		UpdateBmvBvl ( bmv, bvl, mv, vl );

		if ( bvl[0] >= beta ) {
			Search.nBeta ++;
			hash_type = HASH_TYPE_BETA;
			break;
		}
		if ( bvl[0] > alpha ) {
			alpha = bvl[0];
			hash_type = HASH_TYPE_PV;
		}
	}

	// 最后
	InsertInfoToHashTable ( depth, bmv[0], bvl[0], hash_type );
	InsertHistoryTable ( bmv[0], depth );
	CopyBmvBvl ( Search.bmv, Search.bvl, bmv, bvl );
	return bvl[0];
}

// 主搜索函数
int SearchMain ( void ) {
	// 特殊情况
	MoveSortStruct mvsort;
	int nMoveNum = mvsort.InitMove ();
	if ( nMoveNum == 0 ) { // 无着法
		printf ( "bestmove a0a1 resign\n" );
		fflush ( stdout );
		return 0;
	}
	else if ( nMoveNum == 1 ) { // 唯一着法
		printf ( "bestmove %s\n", MoveIntToStr(mvsort.move[0]).c_str() );
		fflush ( stdout );
		return mvsort.move[0];
	}
	if ( pos.IsDraw() ) { // 和局
		printf ( "bestmove a0a1 draw\n" );
		fflush ( stdout );
		// 注意不要return
	}

	// 初始化
	ClearHistoryTable ();
	ClearHashTable ();
	ClearHashTableQC ();
	InitBeginTime ( SEARCH_TOTAL_TIME );

	// 迭代加深搜索
	printf("depth   time    nNode  rBeta");
	for ( int i = 0; i < 3; i ++ ) {
		printf("   bvl[%d]  bmv[%d]", i, i);
	}
	printf("\n");
	fflush ( stdout );
	int lastbvl[nBest], lastbmv[nBest];
	ClearBmvBvl ( lastbmv, lastbvl );
	for ( int depth = 1; /*depth <= ?*/; depth ++ ) {
		InitBeginTime ( THIS_SEARCH_TIME );
		InitSearchStruct ();
		SearchAlphaBeta ( depth, - MATE_VALUE, MATE_VALUE );

		if ( TimeOut() ) {
			CopyBmvBvl ( Search.bmv, Search.bvl, lastbmv, lastbvl );
			break;
		}
		else {
			CopyBmvBvl ( lastbmv, lastbvl, Search.bmv, Search.bvl );
		}

		printf( "%5d  %.2fs  %7d    %2.0f%%", depth, TimeCost(SEARCH_TOTAL_TIME), Search.nNode, 100.0*Search.nBeta/Search.nNode);
		for ( int i = 0; i < 3; i ++ ) {
			printf("   %6d    %s", Search.bvl[i], MoveIntToStr(Search.bmv[i]).c_str());
		}
		printf("\n");
		fflush ( stdout );

		if ( Search.bvl[0] <= - BAN_VALUE || Search.bvl[0] >= BAN_VALUE) {
			break;
		}
		if ( Search.bvl[1] <= - BAN_VALUE || Search.bmv[1] == 0 ) {
			break;
		}
	}
	printf ( "totaltime: %.2fs\n", TimeCost(SEARCH_TOTAL_TIME) );
	fflush ( stdout );

	// 输出最优着法
	if ( Search.bmv[0] == 0 || Search.bvl[0] <= - BAN_VALUE ) {
		printf ( "bestmove a0a1 resign\n" ); // 认输
		fflush ( stdout );
		return 0;
	}
	else {
		printf ( "bestmove %s\n", MoveIntToStr(Search.bmv[0]).c_str() );
		fflush ( stdout );
		return Search.bmv[0];
	}
}
