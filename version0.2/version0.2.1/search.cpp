#include "search.h"
#include "base.h"
#include "hash.h"
#include "position.h"
#include "rollback.h"
#include "movesort.h"
#include "evaluate.h"
#include "debug.h"
#include "time.h"

PositionStruct pos; // 当前搜索局面
RollBackListStruct roll; // 回滚着法表
SearchStruct Search; // 搜索结构体

// 无害裁剪
int HarmlessPruning ( void ) {
	// 1. 和局局面
	if ( pos.IsDraw() ) {
		return 0; // eleeye上表示，为了安全起见，不用pos.DrawValue()
	}

	// 2. 路径重复
	int vRep = roll.RepStatus ();
	if ( vRep != REP_NONE ) {
		return roll.RepValue ( vRep );
	}

	return -MATE_VALUE;
}

// 扩大窗口的 Alpha-Beta 搜索
int AlphaBetaSearch ( int depth, int alpha, int beta ) {
	int val;
	int bestval = - MATE_VALUE;
	int bestmv = 0;
	MoveSortStruct mvsort;

	if ( TimeOut(SEARCH_TOTAL_TIME) ) { // 超时
		return bestval;
	}

	// 1. 无害裁剪
	val = HarmlessPruning ();
	if ( val > - MATE_VALUE ) {
		return val;
	}

	Search.nNode ++;

	// 2. 打分
	if ( depth <= 0 ) {
		return pos.Evaluate ();
	}

	// 3. 置换裁剪
	val = QueryValueInHashTable ( depth );
	if ( val != - MATE_VALUE ) {
		return val;
	}

	// 4. 生成着法
	mvsort.InitAlphaBetaMove ();

	// 5. 递归搜索
	int mv;
	while ( (mv = mvsort.NextMove()) != 0 ) {
		pos.MakeMove ( mv );
		int val = - AlphaBetaSearch ( depth - 1, -beta, -alpha );
		pos.UndoMakeMove ();

		if ( TimeOut(SEARCH_TOTAL_TIME) ) { // 超时
			return bestval;
		}

		if ( val > bestval ) {
			bestval = val;
			bestmv = mv;
			if ( bestval >= beta ) {
				Search.nBeta ++;
				InsertKillerTable ( bestmv );
				break;
			}
			if ( bestval > alpha ) {
				alpha = bestval;
			}
		}
	}

	// 6. 最后
	if ( pos.nDistance == 0 && bestmv != 0 ) {
		Search.bmv = bestmv;
	}
	InsertHashTable ( depth, bestval, bestmv );
	InsertHistoryTable ( bestmv, depth );
	return bestval;
}

// 主搜索函数
void MainSearch ( void ) {
	// 1. 清空置换表，初始化时间计时器
	ClearHashTable ();
	InitBeginTime ( SEARCH_TOTAL_TIME );

	// 2. 大搜索
	if ( Search.debug ) {
		printf("depth   time    nNode  nBeta   value  bestmv\n");
	}
	for ( Search.onlyCheck = 1; Search.onlyCheck >= 0; Search.onlyCheck -- ) {
		// 设置时间限制
		if ( Search.onlyCheck == 1 ) {
			SetTimeLimit (10);
		}
		else {
			SetTimeLimit (30);
		}

		// 迭代加深搜索
		int bvl = - MATE_VALUE;
		int last_bvl = bvl, last_bmv = 0;
		for ( int depth = 1; /*depth <= ?*/; depth ++ ) {
			// 获取 bestmv 及 bestval
			InitBeginTime ( THIS_SEARCH_TIME );
			Search.bmv = 0;
			Search.nNode = Search.nBeta = 0;
			bvl = AlphaBetaSearch ( depth, - MATE_VALUE, MATE_VALUE );

			if ( TimeOut(SEARCH_TOTAL_TIME) ) { // 超时
				bvl = last_bvl;
				Search.bmv = last_bmv;
				break;
			}
			else {
				last_bvl = bvl;
				last_bmv = Search.bmv;
			}

			// 重要信息输出
			if ( Search.debug ) {
				double tc = TimeCost(THIS_SEARCH_TIME);
				if ( tc < 10.0 ) {
					printf( "%5d  %.2fs  %7d    %2.0f%%  %6d    %s\n",
										depth, TimeCost(THIS_SEARCH_TIME), Search.nNode,
										100.0*Search.nBeta/Search.nNode, bvl, MoveIntToStr(Search.bmv).c_str() );
				}
				else {
					printf( "%5d  %.1fs  %7d    %2.0f%%  %6d    %s\n",
														depth, TimeCost(THIS_SEARCH_TIME), Search.nNode,
														100.0*Search.nBeta/Search.nNode, bvl, MoveIntToStr(Search.bmv).c_str() );
				}
			}

			// 搜到杀棋或无解
			if ( bvl <= - MATE_VALUE || bvl >= MATE_VALUE) {
				break;
			}
		}

		if ( bvl >= MATE_VALUE ) {
			break;
		}
	}
	printf( "totaltime: %.2fs\n", TimeCost(SEARCH_TOTAL_TIME) );

	// 3. 输出最优着法
	if ( Search.bmv == 0 ) {
		printf ( "bestmove a0a1 resign\n" ); // 认输
	}
	else {
		printf ( "bestmove %s\n", MoveIntToStr(Search.bmv).c_str() );
	}
}
