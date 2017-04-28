#ifndef DEBUG_H_
#define DEBUG_H_

#include "base.h"
#include "position.h"
#include "search.h"

inline void PrintChessBoard ( void ) {
	printf ( "%s\n", pos.player == 0 ? "Turn Red" : "Turn Black" );
	printf ( "nDistance = %d\n", pos.nDistance);
	printf ( "LastMove = %d\n", roll.LastMove() );
	for ( int i = 51; i < 204 ; i += 16 ) {
		for ( int j = i; j < i + 9; j ++ ) {
			printf ("%3d  ", pos.square[j]);
		}
		printf("\n");
	}
}

#endif /* DEBUG_H_ */
