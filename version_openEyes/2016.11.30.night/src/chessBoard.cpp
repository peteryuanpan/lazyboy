#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->turnWho = RED;
	
}

ChessBoard::ChessBoard(ChessBoard &x, int move)
{

}

bool ChessBoard::turnRed()
{
	return true;
}

bool ChessBoard::turnBlack()
{
	return false;
}