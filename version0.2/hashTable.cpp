#include "hash.h"
#include "base.h"
#include "search.h"
#include "evaluate.h"

struct HashTableStruct {
	int depth; // 深度
	ULL zorb1; // zobrist值1
	ULL zorb2; // zobrist值2
	int bmv; // 最优着法
	int bvl; // 最优分值
	int type; // 最优着法的节点类型
};

HashTableStruct *HashTable;
int HashTableNum;
int HashTableMask;

void InitHashTable ( const int x ) {
	HashTableNum = ( 1<<x ) / sizeof ( HashTableStruct );
	HashTableMask = HashTableNum - 1;
	HashTable = new HashTableStruct[ HashTableNum ];
}

void DelHashTable ( void ) {
	if ( HashTable ) {
		delete HashTable;
	}
}

void ClearHashTable ( void ) {
	for ( int i = 0; i < HashTableNum; i ++ ) {
		HashTable[i].depth = 0;
		HashTable[i].zorb1 = 0;
		HashTable[i].zorb2 = 0;
		HashTable[i].bmv = 0;
		HashTable[i].bvl = - MATE_VALUE;
		HashTable[i].type = -1;
	}
}

void InsertInfoToHashTable ( const int depth, const int bmv, const int bvl, const int type ) {
	const int t = pos.zobrist.first & HashTableMask;
	if ( bvl > - BAN_VALUE && bvl < BAN_VALUE ) {
		if ( depth > HashTable[t].depth ) {
			HashTable[t].depth = depth;
			HashTable[t].zorb1 = pos.zobrist.first;
			HashTable[t].zorb2 = pos.zobrist.second;
			HashTable[t].bmv = bmv;
			HashTable[t].bvl = bvl;
			HashTable[t].type = type;
		}
		else if ( depth == HashTable[t].depth ) {
			if ( bvl > HashTable[t].bvl ) {
				HashTable[t].zorb1 = pos.zobrist.first;
				HashTable[t].zorb2 = pos.zobrist.second;
				HashTable[t].bmv = bmv;
				HashTable[t].bvl = bvl;
				HashTable[t].type = type;
			}
		}
	}
}

int QueryValueInHashTable ( const int depth, const int alpha, const int beta ) {
	const int t = pos.zobrist.first & HashTableMask;
	if ( HashTable[t].zorb1 == pos.zobrist.first && HashTable[t].zorb2 == pos.zobrist.second ) {
		if ( HashTable[t].depth >= depth ) {
			if ( HashTable[t].type == HASH_TYPE_PV ) {
				return HashTable[t].bvl;
			}
			else if ( HashTable[t].type == HASH_TYPE_BETA && HashTable[t].bvl >= beta ) {
				return HashTable[t].bvl;
			}
			else if ( HashTable[t].type == HASH_TYPE_ALPHA && HashTable[t].bvl <= alpha ) {
				return HashTable[t].bvl;
			}
		}
	}
	return - MATE_VALUE;
}

int QueryMoveInHashTable  ( const int depth, const int alpha, const int beta ) {
	const int t = pos.zobrist.first & HashTableMask;
	if ( HashTable[t].zorb1 == pos.zobrist.first && HashTable[t].zorb2 == pos.zobrist.second ) {
		if ( HashTable[t].depth >= depth ) {
			if ( HashTable[t].type == HASH_TYPE_PV ) {
				return HashTable[t].bmv;
			}
			else if ( HashTable[t].type == HASH_TYPE_BETA && HashTable[t].bvl >= beta ) {
				return HashTable[t].bmv;
			}
			else if ( HashTable[t].type == HASH_TYPE_ALPHA && HashTable[t].bvl <= alpha ) {
				return HashTable[t].bmv;
			}
		}
	}
	return 0;
}

int QueryMoveInHashTableWithoutLimit ( void ) {
	const int t = pos.zobrist.first & HashTableMask;
	if ( HashTable[t].zorb1 == pos.zobrist.first && HashTable[t].zorb2 == pos.zobrist.second ) {
		return HashTable[t].bmv;
	}
	return 0;
}

#include "hash.h"
#include "base.h"
#include "search.h"
#include "evaluate.h"

struct HashTableStructQC {
	ULL zorb1; // zobrist值1
	ULL zorb2; // zobrist值2
	int vlAlpha; // 局面alpha值
	int vlBeta; // 局面beta值
};

HashTableStructQC *HashTableQC;
int HashTableQCNum;
int HashTableQCMask;

void InitHashTableQC ( const int x ) {
	HashTableQCNum = ( 1<<x ) / sizeof ( HashTableStruct );
	HashTableQCMask = HashTableQCNum - 1;
	HashTableQC = new HashTableStructQC[ HashTableQCNum ];
}

void DelHashTableQC ( void ) {
	if ( HashTableQC ) {
		delete HashTableQC;
	}
}

void ClearHashTableQC ( void ) {
	for ( int i = 0; i < HashTableQCNum; i ++ ) {
		HashTableQC[i].zorb1 = 0;
		HashTableQC[i].zorb2 = 0;
		HashTableQC[i].vlAlpha = - MATE_VALUE;
		HashTableQC[i].vlBeta = MATE_VALUE;
	}
}

void InsertInfoToHashTableQC ( const int beta, const int alpha ) {
	const int t = pos.zobrist.first & HashTableQCMask;
	HashTableQC[t].zorb1 = pos.zobrist.first;
	HashTableQC[t].zorb2 = pos.zobrist.second;
	HashTableQC[t].vlAlpha = alpha;
	HashTableQC[t].vlBeta = beta;
}

int QueryValueInHashTableQC ( const int alpha, const int beta ) {
	const int t = pos.zobrist.first & HashTableQCMask;
	if ( HashTableQC[t].zorb1 == pos.zobrist.first && HashTableQC[t].zorb2 == pos.zobrist.second ) {
		if ( HashTableQC[t].vlBeta >= beta ) {
			return HashTableQC[t].vlBeta;
		}
		if ( HashTableQC[t].vlAlpha <= alpha ) {
			return HashTableQC[t].vlAlpha;
		}
	}
	return - MATE_VALUE;
}
