#include "chessBoard.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	ChessBoard x;
	for (int i = 0; i < 64; i ++)
	{
		cout << x.whereIs[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 16; i ++)
	{
		cout << x.row[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < 16; i ++)
	{
		cout << x.col[i] << " ";
	}
	cout << endl;
	return 0;
}