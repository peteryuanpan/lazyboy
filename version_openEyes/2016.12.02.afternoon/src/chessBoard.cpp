#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->turnWho = RED;
	this->move.clear();
	get_whatOn_origin();
	get_whereIs();
	get_row();
	get_col();
	get_hash_val();
}

ChessBoard::ChessBoard(const ChessBoard &x, const int move_val)
{
	this->turnWho = x.turnRed() ? BLACK : RED;
	this->move.clear();
	for (int i = 0; i < 256; i ++) this->whatOn[i] = x.whatOn[i];
	modify_whatOn(move_val);
	get_whereIs();
	get_row();
	get_col();
	get_hash_val();
}

bool ChessBoard::turnRed() const
{
	return this->turnWho == RED;
}

bool ChessBoard::turnBlack() const
{
	return this->turnWho == BLACK;
}

void ChessBoard::get_whatOn_origin()
{
	for (int i = 0; i < 16; i ++)
	{
		for (int j = 0; j < 16; j ++)
		{
			this->whatOn[ i * 16 + j ] = ORIGIN_CHESSBOARD[i][j];
		}
	}
}

void ChessBoard::modify_whatOn(const int move_val)
{
	const int from = move_val / 256;
	const int to = move_val % 256;
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
			const int w = this->whatOn[ i * 16 + j ];
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
			const int w = this->whatOn[ i * 16 + j ];
			if (w != EMPTY && w != OBSTACLE)
			{
				this->col[j] |= (1 << i);
			}
		}
	}
}

void ChessBoard::get_hash_val()
{
	const int seed = 65537, mod1 = 1e9 + 7, mod2 = 1e9 + 9;	// 默认值
	const long long val_1 = cal_single_hash_val(seed, mod1);
	const long long val_2 = cal_single_hash_val(seed, mod2);
	this->hash_val = val_1 * mod1 + val_2;
}

long long ChessBoard::cal_single_hash_val(const int seed, const int mod) const
{
	long long hash_val = 0;
	for (int i = 0; i < 16; i ++)
	{
		hash_val = (hash_val * seed + this->row[i]) % mod;
	}
	return hash_val;
}