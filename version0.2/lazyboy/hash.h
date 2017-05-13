#ifndef HASH_H_
#define HASH_H_

#include "base.h"

const int HASH_TYPE_ALPHA = 0;
const int HASH_TYPE_BETA = 1;
const int HASH_TYPE_PV = 2;

void InitZobrist ( void );

struct HashTableStruct {
	int depth; // 深度
	ULL zorb1; // zobrist值1
	ULL zorb2; // zobrist值2
	int bmv; // 最优着法
	int bvl; // 最优分值
	int type; // 最优着法的节点类型
};

void InitHashTable ( const int x );
void DelHashTable ( void );
void ClearHashTable ( void );
void InsertMoveToHashTable ( const int depth, const int bstmv, const int bstval, const int type );
int QueryValueInHashTable ( const int depth, const int alpha, const int beta );
int QueryMoveInHashTable  ( const int depth, const int alpha, const int beta );
int QueryMoveInHashTableWithoutLimit ( void );

void InitHashTableTR ( const int x );
void DelHashTableTR ( void );
void ClearHashTableTR ( void );
void InsertMoveToHashTableTR ( const int depth, const int bstmv, const int bstval, const int type );
int QueryValueInHashTableTR ( const int depth, const int alpha, const int beta );
int QueryMoveInHashTableTR  ( const int depth, const int alpha, const int beta );

#endif /* HASH_H_ */
