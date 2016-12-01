#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->turnWho = RED;
	this->move.clear();
	get_origin_whatOn();
	get_whereIs();
	get_row();
	get_col();
}

ChessBoard::ChessBoard(ChessBoard &x, int move)
{

}

bool ChessBoard::turnRed()
{
	return this->turnWho == RED;
}

bool ChessBoard::turnBlack()
{
	return this->turnWho == BLACK;
}

void ChessBoard::get_origin_whatOn()
{
	for (int i = 0; i < 16; i ++)
	{
		for (int j = 0; j < 16; j ++)
		{
			this->whatOn[ i * 16 + j ] = ORIGIN_CHESSBOARD[i][j];
		}
	}
}

void ChessBoard::get_whereIs()
{
	memset(this->whereIs, 0, sizeof this->whereIs);
	for (int i = 0; i < 256; i ++)
	{
		this->whereIs[ this->whatOn[i] ] = i;
	}
	this->whereIs[EMPTY] = 0;
	this->whereIs[OBSTACLE] = 0;
}

void ChessBoard::get_row()
{
	for (int i = 0; i < 16; i ++)
	{
		this->row[i] = 0;
		for (int j = 0; j < 16; j ++)
		{
			int w = this->whatOn[ i * 16 + j ];
			if (w != EMPTY && w != OBSTACLE)
			{
				this->row[i] |= (1 << j);
			}
		}
	}
}

void ChessBoard::get_col()
{
	for (int j = 0; j < 16; j ++)
	{
		this -> col[j] = 0;
		for (int i = 0; i < 16; i ++)
		{
			int w = this->whatOn[ i * 16 + j ];
			if (w != EMPTY && w != OBSTACLE)
			{
				this->col[j] |= (1 << i);
			}
		}
	}
}