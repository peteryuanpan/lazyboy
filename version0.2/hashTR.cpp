#include "hash.h"
#include "base.h"
#include "search.h"
#include "evaluate.h"

HashTableStruct *HashTableTR;
int HashTableTRNum;
int HashTableTRMask;

void InitHashTableTR ( const int x ) {
	HashTableTRNum = ( 1<<x ) / sizeof ( HashTableStruct );
	HashTableTRMask = HashTableTRNum - 1;
	HashTableTR = new HashTableStruct[ HashTableTRNum ];
}

void DelHashTableTR ( void ) {
	if ( HashTableTR ) {
		delete HashTableTR;
	}
}

void ClearHashTableTR ( void ) {
	for ( int i = 0; i < HashTableTRNum; i ++ ) {
		HashTableTR[i].depth = 0;
		HashTableTR[i].zorb1 = 0;
		HashTableTR[i].zorb2 = 0;
		HashTableTR[i].bmv = 0;
		HashTableTR[i].bvl = - MATE_VALUE;
		HashTableTR[i].type = -1;
	}
}

void InsertMoveToHashTableTR ( const int depth, const int bmv, const int bvl, const int type ) {
	const int t = pos.zobrist.first & HashTableTRMask;
	if ( bvl > - BAN_VALUE ) {
		if ( depth > HashTableTR[t].depth ) {
			HashTableTR[t].depth = depth;
			HashTableTR[t].zorb1 = pos.zobrist.first;
			HashTableTR[t].zorb2 = pos.zobrist.second;
			HashTableTR[t].bmv = bmv;
			HashTableTR[t].bvl = bvl;
			HashTableTR[t].type = type;
		}
		else if ( depth == HashTableTR[t].depth ) {
			if ( bvl > HashTableTR[t].bvl ) {
				HashTableTR[t].zorb1 = pos.zobrist.first;
				HashTableTR[t].zorb2 = pos.zobrist.second;
				HashTableTR[t].bmv = bmv;
				HashTableTR[t].bvl = bvl;
				HashTableTR[t].type = type;
			}
		}
	}
}

int QueryValueInHashTableTR ( const int depth, const int alpha, const int beta ) {
	const int t = pos.zobrist.first & HashTableTRMask;
	if ( HashTableTR[t].zorb1 == pos.zobrist.first && HashTableTR[t].zorb2 == pos.zobrist.second ) {
		if ( HashTableTR[t].depth >= depth ) {
			if ( HashTableTR[t].type == HASH_TYPE_PV ) {
				return HashTableTR[t].bvl;
			}
			else if ( HashTableTR[t].type == HASH_TYPE_BETA && HashTableTR[t].bvl >= beta ) {
				return HashTableTR[t].bvl;
			}
			else if ( HashTableTR[t].type == HASH_TYPE_ALPHA && HashTableTR[t].bvl <= alpha ) {
				return HashTableTR[t].bvl;
			}
		}
	}
	return - MATE_VALUE;
}

int QueryMoveInHashTableTR  ( const int depth, const int alpha, const int beta ) {
	const int t = pos.zobrist.first & HashTableTRMask;
	if ( HashTableTR[t].zorb1 == pos.zobrist.first && HashTableTR[t].zorb2 == pos.zobrist.second ) {
		if ( HashTableTR[t].depth >= depth ) {
			if ( HashTableTR[t].type == HASH_TYPE_PV ) {
				return HashTableTR[t].bmv;
			}
			else if ( HashTableTR[t].type == HASH_TYPE_BETA && HashTableTR[t].bvl >= beta ) {
				return HashTableTR[t].bmv;
			}
			else if ( HashTableTR[t].type == HASH_TYPE_ALPHA && HashTableTR[t].bvl <= alpha ) {
				return HashTableTR[t].bmv;
			}
		}
	}
	return 0;
}
