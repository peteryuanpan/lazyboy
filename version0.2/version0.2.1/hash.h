#ifndef HASH_H_
#define HASH_H_

#include "base.h"

void InitZobrist ();

struct HashTableStruct {
	int depth; // 深度
	ULL zorb1; // zobrist值1
	ULL zorb2; // zobrist值2
	int value;   // 分值
	int move; // 着法
};

const int HashTableNum = 1 << 23;
const int HashTableMask = HashTableNum - 1;
extern HashTableStruct HashTable[ HashTableNum + 100 ];

void ClearHashTable ( void );
void InsertHashTable ( const int depth, const int val, const int mv );
int QueryValueInHashTable ( const int depth );
int QueryMoveInHashTable ( void );

#endif /* HASH_H_ */
