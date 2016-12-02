#include "chessBoard.h"
#include "hash.h"
#include "base.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main()
{
	ChessBoard x;
	for (int i = 0; i < 64; i ++)
	{
		cout << x.whereIs[i] << " ";
	}
	cout << endl;
	return 0;
}