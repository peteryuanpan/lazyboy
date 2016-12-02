#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->turnWho = RED;
	this->move.clear();
	this->get_origin_whatOn();
	this->get_whereIs();
	this->get_row();
	this->get_col();
}

ChessBoard::ChessBoard(const ChessBoard &x, const int move_val)
{
	this->turnWho = x.turnRed() ? BLACK : RED;
	this->move.clear();
	copy_array(this->whatOn, x.whatOn, 256);
	this->get_whatOn_by_move(move_val);
	this->get_whereIs();
	this->get_row();
	this->get_col();
}

bool ChessBoard::turnRed() const
{
	return this->turnWho == RED;
}

bool ChessBoard::turnBlack() const
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

void ChessBoard::get_whatOn_by_move(const int move_val)
{
	int from = move_val / 256;
	int to = move_val % 256;
	this->whatOn[to] = this->whatOn[from];
	this->whatOn[from] = EMPTY;
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
		this->col[j] = 0;
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