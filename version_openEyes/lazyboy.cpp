#include "chessBoard.h"
#include <iostream>
using namespace std;

const int test[2] = {1, 2};

int main()
{
	ChessBoard x;
	cout << x.turnWho << endl;
	cout << x.turnRed() << endl;
	cout << "just test" << endl;
	cout << test[0] << endl;
	cout << "just test" << endl;
	return 0;
}