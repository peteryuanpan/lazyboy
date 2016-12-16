#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <cstring>
#include <vector>
#include <map>
using namespace std;

const int N_PIECE = 14 * 13;	// 14x13的棋盘

class ChessBoard 
{
public:
	ChessBoard();
	ChessBoard(const ChessBoard &x, const int move_val);
	bool turnRed() const;
	bool turnBlack() const;
	long long hash_val;
private:
	int ThisSideColor, OppSideColor; // RED, BLACK
	int whatOn[ N_PIECE ];
	vector<int> King, opp_King; // 棋子的位置，下同
	vector<int> Rook, opp_Rook;
	vector<int> Knight, opp_Knight;
	vector<int> Cannon, opp_Cannon;
	vector<int> Bishop, opp_Bishop;
	vector<int> Advisor, opp_Advisor;
	vector<int> Pawn, opp_Pawn;
	void init_pos_chessPiece();
	void init_pos_ThisSidePiece();
	void init_pos_OppSidePiece();
	void modify_whatOn(const int move_val);
	void init_hash_val();
	long long cal_single_hash_val(const int seed, const int mod) const;
	int chess_color(const int this_pos) const; // RED, BLACK, EMPTY, OBSTACLE
	int chess_type(const int this_pos) const; // KING, ROOK, ..., PAWN, EMPTY, OBSTACLE

public:
	vector<int> move; // 着法
	void gen_move_all();
	void gen_move_check();
	void gen_move_checked();
private:
	bool ThisSideCheck() const;
	bool OppSideCheck() const;
	void move_push_back(const int from_pos, const int to_pos);
private:
	void gen_move_King();
	void gen_move_Rook();
	void gen_move_Knight();
	void gen_move_Cannon();
	void gen_move_Bishop();
	void gen_move_Advisor();
	void gen_move_Pawn();
	void gen_move_Pawn_RED();
	void gen_move_Pawn_BLACK();
private:
	bool canMove_King();
	bool canMove_Rook();
	bool canMove_Knight();
	bool canMove_Cannon();
	bool canMove_Bishop();
	bool canMove_Advisor();
	bool canMove_Pawn();
	bool canMove_Pawn_RED();
	bool canMove_Pawn_BLACK();
};

const int XX = 11;

const int RED = 16, BLACK = 32, EMPTY = 0, OBSTACLE = XX;
const int KING = 0, ROOK = 1, KNIGHT = 2, CANNON = 3, BISHOP = 4, ADVISOR = 5, PAWN = 6;
const int ORIGIN_CHESSBOARD[ N_PIECE ] = {
XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX,
XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX,
XX, XX, 33, 34, 36, 37, 32, 37, 36, 34, 33, XX, XX,
XX, XX, 00, 00, 00, 00, 00, 00, 00, 00, 00, XX, XX,
XX, XX, 00, 35, 00, 00, 00, 00, 00, 35, 00, XX, XX,
XX, XX, 38, 00, 38, 00, 38, 00, 38, 00, 38, XX, XX,
XX, XX, 00, 00, 00, 00, 00, 00, 00, 00, 00, XX, XX,
XX, XX, 00, 00, 00, 00, 00, 00, 00, 00, 00, XX, XX,
XX, XX, 22, 00, 22, 00, 22, 00, 22, 00, 22, XX, XX,
XX, XX, 00, 19, 00, 00, 00, 00, 00, 19, 00, XX, XX,
XX, XX, 00, 00, 00, 00, 00, 00, 00, 00, 00, XX, XX,
XX, XX, 17, 18, 20, 21, 16, 21, 20, 18, 17, XX, XX,
XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX,
XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX, XX
};

const int VECTOR_KING [] = {-16, +1, +16, -1};
const int VECTOR_ROOK [] = {-16, +1, +16, -1};
const int VECTOR_KNIGHT [] = {-31, -14, +18, +33, +31, +14, -18, -33};
const int VECTOR_CANNON [] = {-16,  +1, +16,  -1};
const int VECTOR_BISHOP [] = {-30, +34, +30, -34};
const int VECTOR_ADVISOR [] = {-15, +17, +15, -17};
const int VECTOR_RED_PAWN [] = {-16, +1, -1};
const int VECTOR_BLACK_PAWN [] = {+16, +1, -1};

const int VECTOR_BAN_KNIGHT [] = {-16, +1, +1, +16, +16, -1, -1, -16};
const int VECTOR_BAN_BISHOP [] = {-15, +17, +15, -17};

const int SIZE_VECTOR_KING = 4;
const int SIZE_VECTOR_ROOK = 4;
const int SIZE_VECTOR_KNIGHT = 8;
const int SIZE_VECTOR_CANNON = 4;
const int SIZE_VECTOR_BISHOP = 4;
const int SIZE_VECTOR_ADVISOR = 4;
const int SIZE_VECTOR_RED_PAWN = 3;
const int SIZE_VECTOR_BLACK_PAWN = 3;

const int CAN_STAY_KING[ N_PIECE ] = {
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, XX, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};
const int CAN_STAY_BISHOP[ N_PIECE ] = {
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, XX, 00, 00, 00, XX, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, XX, 00, 00, 00, 00, 00, 00, 00, XX, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, XX, 00, 00, 00, XX, 00, 00, 00, 00,
00, 00, 00, 00, XX, 00, 00, 00, XX, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, XX, 00, 00, 00, XX, 00, 00, 00, XX, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, XX, 00, 00, 00, XX, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};
const int CAN_STAY_ADVISOR[ N_PIECE ] = {
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, 00, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, XX, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, 00, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, 00, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, XX, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, XX, 00, XX, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};
const int CAN_STAY_RED_PAWN[ N_PIECE ] = {
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, 00, XX, 00, XX, 00, XX, 00, XX, 00, 00,
00, 00, XX, 00, XX, 00, XX, 00, XX, 00, XX, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};
const int CAN_STAY_BLACK_PAWN[ N_PIECE ] = {
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, XX, 00, XX, 00, XX, 00, XX, 00, XX, 00, 00,
00, 00, XX, 00, XX, 00, XX, 00, XX, 00, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, XX, XX, XX, XX, XX, XX, XX, XX, XX, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00
};

#endif