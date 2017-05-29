#ifndef HASH_H_
#define HASH_H_

#include "base.h"

extern ULL ZobristSP_1 [256][48];
extern ULL ZobristPlayer_1;
extern ULL ZobristSP_2 [256][48];
extern ULL ZobristPlayer_2;

const int HASH_TYPE_ALPHA = 0;
const int HASH_TYPE_BETA = 1;
const int HASH_TYPE_PV = 2;

void InitZobrist ( void );

void InitHashTable ( const int x );
void DelHashTable ( void );
void ClearHashTable ( void );
void InsertInfoToHashTable ( const int depth, const int bmv, const int bvl, const int type );
int QueryValueInHashTable ( const int depth, const int alpha, const int beta );
int QueryMoveInHashTable  ( const int depth, const int alpha, const int beta );
int QueryMoveInHashTableWithoutLimit ( void );

void InitHashTableQC ( const int x );
void DelHashTableQC ( void );
void ClearHashTableQC ( void );
void InsertInfoToHashTableQC ( const int beta, const int alpha );
int QueryValueInHashTableQC ( const int alpha, const int beta );

#endif /* HASH_H_ */
