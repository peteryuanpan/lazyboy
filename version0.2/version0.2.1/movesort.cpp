#include "movesort.h"
#include "debug.h"
#include "search.h"
#include "hash.h"

// 历史表、杀手表
int HistoryTable [ 256 * 256 ];
const int MAX_DEPTH_KILLER = 30;
int KillerTable [ 256 * 256 ][ MAX_DEPTH_KILLER ];

// 初始化
void InitMoveSort ( void ) {
	memset ( HistoryTable, 0, sizeof HistoryTable );
	memset ( KillerTable, 0, sizeof KillerTable );
}

// 更新历史表
void InsertHistoryTable ( const int mv, const int depth ) {
	if ( mv != 0 ) {
		HistoryTable[mv] += depth * depth; // 考虑下别的函数？
	}
}

// 更新杀手表
void InsertKillerTable ( const int mv ) {
	if ( pos.nDistance < MAX_DEPTH_KILLER && mv != 0 ) {
		KillerTable[mv][pos.nDistance] += 1;
	}
}

// 生成着法
void MoveSortStruct::InitAlphaBetaMove ( void ) {
	// 1. 初始化
	memset ( move, 0, sizeof move );
	nMoveIndex = 0;
	nMoveNum = 0;

	// 2. 生成所有着法
	pos.GenAllMove ( move, nMoveNum );
	pos.DelMeaningLessMove ( move, nMoveNum );

	// 3. 获得置换表、杀手表的着法
	int hashmv = QueryMoveInHashTable ();
	int killmv1 = 0, killmv2 = 0;
	int MAX1 = 0, MAX2 = 0;
	if ( pos.nDistance < MAX_DEPTH_KILLER ) { // !
		for ( int i = 0; i < nMoveNum; i ++ ) {
			if ( KillerTable[move[i]][pos.nDistance] > MAX1 ) {
				killmv2 = killmv1;
				MAX2 = MAX1;
				killmv1 = move[i];
				MAX1 = KillerTable[move[i]][pos.nDistance];
			}
			else if ( KillerTable[move[i]][pos.nDistance] > MAX2 ) {
				killmv2 = move[i];
				MAX2 = KillerTable[move[i]][pos.nDistance];
			}
		}
	}

	// 4. 给着法分类赋值
	int type[128];
	for ( int i = 0; i < nMoveNum; i ++ ) {
		type[i] = SORT_TYPE_OTHER; // 0
		if ( hashmv == move[i] ) { // 5
			type[i] = SORT_TYPE_HASHTABLE;
		}
		else if ( Search.onlyCheck ) { // 4
			pos.MakeMove ( move[i] );
			if ( pos.checked ) {
				type[i] = SORT_TYPE_CHECK;
			}
			pos.UndoMakeMove ();
		}
		else if ( move[i] == killmv1 ) { // 2
			type[i] = SORT_TYPE_KILLER_1;
		}
		else if ( move[i] == killmv2 ) { // 1
			type[i] = SORT_TYPE_KILLER_2;
		}
	}

	// 5. 按赋值降序排序
	int t = 0;
	for ( int k = MAX_SORT_TYPE; k >= 0; k -- ) {
		for ( int i = t; i < nMoveNum; i ++ ) {
			if ( type[i] == k ) {
				SWAP ( move[i], move[t] );
				SWAP ( type[i], type[t] );
				t ++;
			}
		}
	}

	// 6. 若未被将军，且onlyCheck，则只生成不小于将军搜索的
	if ( !pos.checked && Search.onlyCheck ) {
		int n = 0;
		for ( int i = 0; i < nMoveNum; i ++ ) {
			if ( type[i] >= SORT_TYPE_CHECK ) {
				n ++;
			}
		}
		nMoveNum = n;
	}

	// 7. 对每类按历史表排序
	for ( int k = MAX_SORT_TYPE ; k >= 0; k -- ) {
		int s;
		for ( s = 0; s < nMoveNum; s ++ ) { // 找到第一个k的位置
			if ( type[s] == k ) {
				break;
			}
		}
		if ( s >= nMoveNum ) { // 未找到
			continue;
		}
		int t = s;
		for (; t < nMoveNum; t ++ ) { // 找到最后一个k的位置+1
			if ( type[t] != k ) {
				break;
			}
		}

		for ( int i = s; i < t; i ++ ) {
			for ( int j = i + 1; j < t; j ++ ) {
				if ( HistoryTable[move[j]] > HistoryTable[move[i]] ) { // 按历史表排序
					SWAP ( move[i], move[j] );
				}
			}
		}
	}

	// 8. Debug
	if ( false ) {
		PrintChessBoard ();
		printf("nMoveNum = %d\n", nMoveNum);
		for ( int i = 0; i < nMoveNum; i ++ ) {
			pos.MakeMove ( move[i] );
			PrintChessBoard ();
			pos.UndoMakeMove ();
		}
		printf("\n");
	}
}

