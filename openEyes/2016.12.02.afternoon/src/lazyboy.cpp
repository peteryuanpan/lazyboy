#include "chessBoard.h"
#include "genMove.h"
#include "search.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main()
{
	ChessBoard x;
	Search y = Search(30);
	int result = y.search(x);
	cout << result << endl;
	const GenMove z;
	z.gen_defense(x);
	return 0;
}