#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->turnWho = RED;
	this->move.clear();
	// ..
	// ..
}

ChessBoard::ChessBoard(ChessBoard &x, int move)
{
	// .......
}

bool ChessBoard::turnRed()
{
	return this->turnWho == RED;
}

bool ChessBoard::turnBlack()
{
	return this->turnWho == BLACK;
}