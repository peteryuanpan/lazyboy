#include "ChessBoard.h"

ChessBoard::ChessBoard()
{
	this->ThisSideColor = RED;
	this->OppSideColor = BLACK;
	for (int i = 0; i < N_PIECE; i ++) this->whatOn[i] = ORIGIN_CHESSBOARD[i];
	init_pos_chessPiece();
	init_hash_val();
	this->move.clear();
}

ChessBoard::ChessBoard(const ChessBoard &x, const int move_val)
{
	this->ThisSideColor = x.OppSideColor;
	this->OppSideColor = x.ThisSideColor;
	for (int i = 0; i < N_PIECE; i ++) this->whatOn[i] = x.whatOn[i];
	modify_whatOn(move_val);
	init_pos_chessPiece();
	init_hash_val();
	this->move.clear();
}

bool ChessBoard::turnRed() const
{
	return this->ThisSideColor == RED;
}

bool ChessBoard::turnBlack() const
{
	return this->ThisSideColor == BLACK;
}

void ChessBoard::init_pos_chessPiece()
{
	this->King.clear(),	this->opp_King.clear();
	this->Rook.clear(),	this->opp_Rook.clear();
	this->Knight.clear(),	this->opp_Knight.clear();
	this->Cannon.clear(),	this->opp_Cannon.clear();
	this->Bishop.clear(),	this->opp_Bishop.clear();
	this->Advisor.clear(),	this->opp_Advisor.clear();
	this->Pawn.clear(),	this->opp_Pawn.clear();
	init_pos_ThisSidePiece();
	init_pos_OppSidePiece();
}

void ChessBoard::init_pos_ThisSidePiece()
{
	for (int i = 0; i < N_PIECE; i ++)
	{
		if ( chess_color(i) == this->ThisSideColor )
		{
			int type = chess_type(i);
			if ( type == KING )	this->King.push_back(i);
			else if ( type == ROOK )	this->Rook.push_back(i);
			else if ( type == KNIGHT )	this->Knight.push_back(i);
			else if ( type == CANNON )	this->Cannon.push_back(i);
			else if ( type == BISHOP )	this->Bishop.push_back(i);
			else if ( type == ADVISOR )	this->Advisor.push_back(i);
			else if ( type == PAWN )	this->Pawn.push_back(i);
		}
	}
}

void ChessBoard::init_pos_OppSidePiece()
{
	for (int i = 0; i < N_PIECE; i ++)
	{
		if ( chess_color(i) == this->OppSideColor )
		{
			int type = chess_type(i);
			if ( type == KING )	this->opp_King.push_back(i);
			else if ( type == ROOK )	this->opp_Rook.push_back(i);
			else if ( type == KNIGHT )	this->opp_Knight.push_back(i);
			else if ( type == CANNON )	this->opp_Cannon.push_back(i);
			else if ( type == BISHOP )	this->opp_Bishop.push_back(i);
			else if ( type == ADVISOR )	this->opp_Advisor.push_back(i);
			else if ( type == PAWN )	this->opp_Pawn.push_back(i);
		}
	}
}

void ChessBoard::modify_whatOn(const int move_val)
{
	const int from_pos = move_val / N_PIECE;
	const int to_pos = move_val % N_PIECE;
	this->whatOn[ to_pos ] = this->whatOn[ from_pos ];
	this->whatOn[ from_pos ] = EMPTY;
}

void ChessBoard::init_hash_val()
{
	const int seed = 65537, mod_1 = 1e9 + 7, mod_2 = 1e9 + 9;	// 默认值
	const long long val_1 = cal_single_hash_val(seed, mod_1);
	const long long val_2 = cal_single_hash_val(seed, mod_2);
	this->hash_val = val_1 * mod_1 + val_2;
}

long long ChessBoard::cal_single_hash_val(const int seed, const int mod) const
{
	long long hash_val = 0;
	for (int i = 0; i < N_PIECE; i ++)
	{
		hash_val = ( hash_val * seed + this->whatOn[i] ) % mod;
	}
	return hash_val;
}

int ChessBoard::chess_color(const int this_pos) const
{
	const int w = this->whatOn[ this_pos ];
	if ( w == EMPTY || w == OBSTACLE ) return w;
	else if ( w & RED ) return RED;
	else return BLACK;
}

int ChessBoard::chess_type(const int this_pos) const
{
	const int w = this->whatOn[ this_pos ];
	if ( w == EMPTY || w == OBSTACLE ) return w;
	else if ( w & RED ) return w - RED;
	else  return w - BLACK;
}