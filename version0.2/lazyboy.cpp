#include "base.h"
#include "position.h"
#include "hash.h"
#include "move.h"
#include "movesort.h"
#include "search.h"
#include "ucci.h"
#include "time.h"
#include "debug.h"

int main ( int argc, char *argv[] ) {
#ifndef _WIN32
	freopen ( "/Users/peteryuanpan/Desktop/毕业/lazyboy-master/version0.2/data/data.txt", "r", stdin );
#endif

	if ( BootLine() == e_CommUcci ) {
		printf( "id name lazyboy\n" );
		printf( "id version 0.2\n" );
		printf( "id author panyuan\n" );
		printf( "ucciok\n" );
		fflush( stdout );

		while ( true ) {
			CommEnum Order = IdleLine ( Command, 0 );
			if ( Order == e_CommAnnotation ) {
				printf( "%s\n", Command.Annotation.String );
				fflush( stdout );
			}
			else if ( Order == e_CommQuit ) { // quit
				DelHashTable ();
				DelHashTableQC ();
				printf( "bye\n" );
				fflush( stdout );
				break;
			}
			else if ( Order == e_CommIsReady ) { // isready
				printf( "readyok\n" );
				fflush( stdout );
			}
			else if ( Order == e_CommSetOption ) {
				if( Command.Option.Type == e_NewGame ) { // setoption newgame
					roll.Init ();
					InitMove ();
					InitZobrist ();
					InitHashTable ( 28 ); // 256MB
					InitHashTableQC ( 27 ); // 128MB

					SetTimeLimit ( SEARCH_TOTAL_TIME, 20 ); // 总时间上限
					SetTimeLimit ( THIS_SEARCH_TIME, 20 ); // 单次搜索时间上限
				}
			}
			else if ( Order == e_CommPosition ) { // position [ startpos | fen ] moves ...
				pos.Init ( Command.Position.FenStr, Command.Position.MoveStr, Command.Position.MoveNum );
			}
			else if ( Order == e_CommGo || Order == e_CommGoPonder ) { // go ...
				int bmv = SearchMain ();
				pos.MakeMove (bmv);
			}
		}
	}
	return 0;
}
