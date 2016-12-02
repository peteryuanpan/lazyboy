#ifndef GENMOVE_H
#define GENMOVE_H
#include "chessBoard.h"

class GenMove
{
public:
	void gen_move_check(ChessBoard &x);
	void gen_move_checked(ChessBoard &x);
private:
	ChessBoard board;
	void gen_move_all();
	void gen_move_check();
	void gen_move_checked();

	void gen_move(int pos);
	void gen_move_King(int pos);
	void gen_move_Rook(int pos);
	void gen_move_Knight(int pos);
	void gen_move_Cannon(int pos);
	void gen_move_Bishop(int pos);
	void gen_move_Advisor(int pos);
	void gen_move_Pawn(int pos);

	bool ThisSideCheck();
	bool OppSideCheck();

	bool in_range(int pos);
	bool in_range_King(int pos);
	bool in_range_Rook(int pos);
	bool in_range_Knight(int pos);
	bool in_range_Cannon(int pos);
	bool in_range_Bishop(int pos);
	bool in_range_Advisor(int pos);
	bool in_range_Pawn(int pos);
	bool in_range_UpperHalf(int pos);
	bool in_range_LowerHalf(int pos);
};

const int MOVE_KING [] = {-16, +1, +16, -1};
const int N_MOVE_KING = 4;
const int RANGE_KING_RED [] = {166, 167, 168, 182, 183, 184, 198, 199, 200};
const int N_RANGE_KING_RED = 9;
const int RANGE_KING_BLACK[] = {54, 55, 56, 70, 71, 72, 86, 87, 88};
const int N_RANGE_KING_BLACK = 9;

const int MOVE_ROOK [] = {-16, +1, +16, -1};
const int N_MOVE_ROOK = 4;

const int MOVE_KNIGHT [] = {-31, -14, +18, +33, +31, +14, -18, -33};
const int N_MOVE_KNIGHT = 8;
const int BAN_KNIGHT [] = {-16, +1, +1, -16, -16, -1, -1, -16};
const int N_BAN_KNIGHT = 8;

const int MOVE_CANNON [] = {-16,  +1, +16,  -1};
const int N_MOVE_CANNON = 4;

const int MOVE_BISHOP [] = {-30, +34, +30, -34};
const int N_MOVE_BISHOP = 4;
const int BAN_BISHOP [] = {-15, +17, +15, -17};
const int N_BAN_BISHOP = 4;

const int MOVE_ADVISOR [] = {-15, +17, +15, -17};
const int N_MOVE_ADVISOR = 4;
const int RANGE_ADVISOR_RED [] = {166, 168, 183, 198, 200};
const int N_RANGE_ADVISOR_RED = 5;
const int RANGE_ADVISOR_BLACK [] = {54, 56, 71, 86, 88};
const int N_RANGE_ADVISOR_BLACK = 5;

const int MOVE_FORWARD_PAWN_RED = -16;
const int MOVE_FORWARD_PAWN_BLACK = +16;
const int MOVE_LEFT_RIGHT_PAWN = {-1, +1};
const int N_MOVE_LEFT_RIGHT_PAWN = 2;

#endif