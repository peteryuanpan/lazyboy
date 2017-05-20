#include "base.h"
#include "hash.h"
#include "move.h"
#include <ctime>

bool HASH_INIT_FLAG = false;

ULL ZobristSP_1 [256][48];
ULL ZobristPlayer_1;
ULL ZobristSP_2 [256][48];
ULL ZobristPlayer_2;

inline ULL rand64 ( void ) {
	return ((ULL)rand()) ^ ((ULL)rand() << 15) ^ ((ULL)rand() << 30) ^ ((ULL)rand() << 45);
}

void InitZobrist_sub ( ULL ZobristSP [][48], ULL &ZobristPlayer ) {
	for ( int i = 0; i < 256; i ++ ) {
		for ( int j = 0; j < 48; j ++ ) {
			ZobristSP[i][j] = 0;
		}
	}

	for ( int player = 0; player <= 1; player ++ ) {
		int ST = SIDE_TYPE ( player );
		int p = STA_POS;
		while ( p != 0 ) {
			ZobristSP[p][ ST + KING_FROM ] 	= rand64 ();
			ZobristSP[p][ ST + ADVISOR_FROM ] = ZobristSP[p][ ST + ADVISOR_TO ] = rand64 ();
			ZobristSP[p][ ST + BISHOP_FROM ] = ZobristSP[p][ ST + BISHOP_TO ] = rand64 ();
			ZobristSP[p][ ST + KNIGHT_FROM ] = ZobristSP[p][ ST + KNIGHT_TO ] = rand64 ();
			ZobristSP[p][ ST + ROOK_FROM ] = ZobristSP[p][ ST + ROOK_TO ] = rand64 ();
			ZobristSP[p][ ST + CANNON_FROM ] = ZobristSP[p][ ST + CANNON_TO ] =  rand64 ();
			int t = rand64 ();
			for ( int j = ST + PAWN_FROM; j <= ST + PAWN_TO; j ++ ) {
				ZobristSP[p][j] = t;
			}
			p = NEXTPOS ( p );
		}
	}

	ZobristPlayer = rand64 ();
}

void InitZobrist ( void ) {
	if ( HASH_INIT_FLAG ) {
		return;
	}
	HASH_INIT_FLAG = true;

	srand (time(NULL)); // !!
	
	InitZobrist_sub ( ZobristSP_1, ZobristPlayer_1 );
	InitZobrist_sub ( ZobristSP_2, ZobristPlayer_2 );
}

void PositionStruct::GenZobrist () {
	zobrist.first = 0;
	for ( int i = 16; i < 48; i ++ ) {
		if ( piece[i] ) {
			 zobrist.first ^= ZobristSP_1 [ piece[i] ][i];
		}
	}
	zobrist.first ^= ZobristPlayer_1;

	zobrist.second = 0;
	for ( int i = 16; i < 48; i ++ ) {
		if ( piece[i] ) {
			zobrist.second ^= ZobristSP_2 [ piece[i] ][i];
		}
	}
	zobrist.second ^= ZobristPlayer_2;
}

void PositionStruct::ModifyZobrist ( const int mv, const int sqSrc, const int sqDst ) {
	int src = SRC ( mv );
	int dst = DST ( mv );

	zobrist.first ^= ZobristSP_1 [ src ][ sqSrc ];
	zobrist.first ^= ZobristSP_1 [ dst ][ sqDst ];
	zobrist.first ^= ZobristSP_1 [ dst ][ sqSrc ];
	zobrist.first ^= ZobristPlayer_1;

	zobrist.second ^= ZobristSP_2 [ src ][ sqSrc ];
	zobrist.second ^= ZobristSP_2 [ dst ][ sqDst ];
	zobrist.second ^= ZobristSP_2 [ dst ][ sqSrc ];
	zobrist.second ^= ZobristPlayer_2;
}

#include "hash.h"
#include "base.h"
#include "search.h"
#include "evaluate.h"

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

void InsertMoveToHashTable ( const int depth, const int bmv, const int bvl, const int type ) {
	const int t = pos.zobrist.first & HashTableMask;
	if ( bvl > - BAN_VALUE ) {
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