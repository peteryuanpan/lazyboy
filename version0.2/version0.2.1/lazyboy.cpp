#include "base.h"
#include "position.h"
#include "hash.h"
#include "move.h"
#include "movesort.h"
#include "search.h"
#include "ucci.h"
#include "time.h"

int main ( int argc, char *argv[] ) {
	//freopen ("/Users/peteryuanpan/Desktop/毕业设计/lazyboy-master/version0.2/趣味象棋/long_data.txt", "r", stdin);

	printf("Type 'ucci' to begin.\n");
	fflush(stdout);

	if( BootLine() == e_CommUcci ) {
		printf("id name lazyboy\n");
		printf("id version 0.2.1\n");
		printf("id author peterpan\n");
		printf("ucciok\n");
		fflush(stdout);

		while ( true ) {
			CommEnum Order = IdleLine ( Command, 0 );
			if ( Order == e_CommAnnotation ) {
				printf("%s\n", Command.Annotation.String);
				fflush(stdout);
			}
			else if ( Order == e_CommQuit ) { // quit
				break;
			}
			else if( Order == e_CommIsReady ) { // isready
				printf("readyok\n");
				fflush(stdout);
			}
			else if ( Order == e_CommSetOption ) {
				if( Command.Option.Type == e_NewGame ) { // setoption newgame
					roll.Init ();
					InitZobrist ();
					InitMove ();
					InitMoveSort ();
					Search.debug = 0;
				}
			}
			else if ( Order == e_CommPosition ) { // position [ startpos | fen ] moves ...
				pos.Init ( Command.Position.FenStr, Command.Position.MoveStr, Command.Position.MoveNum );
			}
			else if( Order == e_CommGo || Order == e_CommGoPonder ) { // go nodes 10077696
				MainSearch ();
			}
		}

		printf("bye\n");
		fflush(stdout);
	}
	return 0;
}
