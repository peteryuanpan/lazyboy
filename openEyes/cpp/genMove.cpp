#include "chessBoard.h"

void ChessBoard::gen_move_all()
{
	gen_move_King();
	gen_move_Rook();
	gen_move_Knight();
	gen_move_Cannon();
	gen_move_Bishop();
	gen_move_Advisor();
	gen_move_Pawn();
}

void ChessBoard::gen_move_check()
{
	gen_move_all();
	vector<int> new_move;
	for (int i = 0; i < this->move.size(); i ++)
	{
		if ( ThisSideCheck() && OppSideCheck() == false )
		{
			new_move.push_back( this->move[i] );
		}
	}
	this->move = new_move;
}

void ChessBoard::gen_move_checked()
{
	gen_move_all();
	vector<int> new_move;
	for (int i = 0; i < this->move.size(); i ++)
	{
		if ( OppSideCheck() == false )
		{
			new_move.push_back( this->move[i] );
		}
	}
	this->move = new_move;
}

bool ChessBoard::ThisSideCheck() const
{
	return true;
}

bool ChessBoard::OppSideCheck() const
{
	return false;
}

void ChessBoard::move_push_back(const int from_pos, const int to_pos)
{
	const int move_val = from_pos * N_PIECE + to_pos;
	this->move.push_back( move_val );
}

void ChessBoard::gen_move_King()
{
	for (int i = 0; i < this->King.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_KING; j ++)
		{
			const int new_pos = this->King[i] + VECTOR_KING[j];
			if ( CAN_STAY_KING[ new_pos ] )
			{
				if ( chess_color( new_pos ) != ThisSideColor )
				{
					move_push_back(this->King[i], new_pos);
				}
			}
		}
	}
}

void ChessBoard::gen_move_Rook()
{
	for (int i = 0; i < this->Rook.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_ROOK; j ++)
		{
			int new_pos = this->Rook[i] + VECTOR_ROOK[j];
			while ( chess_type( new_pos ) == EMPTY )
			{
				move_push_back(this->Rook[i], new_pos);
				new_pos += VECTOR_ROOK[j];
			}
			if ( chess_color( new_pos ) == OppSideColor )
			{
				move_push_back(this->Rook[i], new_pos);
			}
		}
	}
}

void ChessBoard::gen_move_Knight()
{
	for (int i = 0; i < this->Knight.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_KNIGHT; j ++)
		{
			const int new_pos = Knight[i] + VECTOR_KNIGHT[j];
			const int ban_pos = Knight[i] + VECTOR_BAN_KNIGHT[j];
			if ( chess_type( ban_pos ) == EMPTY )
			{
				int type = chess_type( new_pos );
				if ( type == EMPTY || type == OppSideColor )
				{
					move_push_back(this->Knight[i], new_pos);
				}
			}
		}
	}
}

void ChessBoard::gen_move_Cannon()
{
	for (int i = 0; i < this->Cannon.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_CANNON; j ++)
		{
			int new_pos = this->Cannon[i] + VECTOR_CANNON[j];
			while ( chess_type( new_pos ) == EMPTY )
			{
				move_push_back(this->Cannon[i], new_pos);
				new_pos += VECTOR_CANNON[j];
			}
			new_pos += VECTOR_CANNON[j];
			while ( chess_type( new_pos ) == EMPTY ) new_pos += VECTOR_CANNON[j];
			if ( chess_color( new_pos ) == OppSideColor )
			{
				move_push_back(this->Cannon[i], new_pos);
			}
		}
	}
}

void ChessBoard::gen_move_Bishop()
{
	for (int i = 0; i < this->Bishop.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_BISHOP; j ++)
		{
			const int new_pos = this->Bishop[i] + VECTOR_BISHOP[j];
			const int ban_pos = this->Bishop[i] + VECTOR_BAN_BISHOP[j];
			if ( chess_type( ban_pos ) == EMPTY )
			{
				if ( CAN_STAY_BISHOP[ new_pos ] )
				{
					if ( chess_color( new_pos ) != ThisSideColor )
					{
						move_push_back(this->Bishop[i], new_pos);
					}
				} 
			}
		}
	}
}

void ChessBoard::gen_move_Advisor()
{
	for (int i = 0; i < this->Advisor.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_ADVISOR; j ++)
		{
			const int new_pos = Advisor[i] + VECTOR_ADVISOR[j];
			if ( CAN_STAY_ADVISOR[ new_pos ] )
			{
				if ( chess_color( new_pos ) != ThisSideColor )
				{
					move_push_back(this->Advisor[i], new_pos);
				}
			}
		}
	}
}

void ChessBoard::gen_move_Pawn()
{
	if( ThisSideColor == RED ) gen_move_Pawn_RED();
	else gen_move_Pawn_BLACK();
}

void ChessBoard::gen_move_Pawn_RED()
{
	for (int i = 0; i < this->Pawn.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_RED_PAWN; j ++)
		{
			const int new_pos = this->Pawn[i] + VECTOR_RED_PAWN[j];
			if ( CAN_STAY_RED_PAWN[ new_pos ] )
			{
				if ( chess_color( new_pos ) != ThisSideColor )
				{
					move_push_back(this->Pawn[i], new_pos);
				}
			}
		}
	}
}

void ChessBoard::gen_move_Pawn_BLACK()
{
	for (int i = 0; i < this->Pawn.size(); i ++)
	{
		for (int j = 0; j < SIZE_VECTOR_BLACK_PAWN; j ++)
		{
			const int new_pos = this->Pawn[i] + VECTOR_BLACK_PAWN[j];
			if ( CAN_STAY_BLACK_PAWN[ new_pos ] )
			{
				if ( chess_color( new_pos ) != ThisSideColor )
				{
					move_push_back(this->Pawn[i], new_pos);
				}
			}
		}
	}
}
