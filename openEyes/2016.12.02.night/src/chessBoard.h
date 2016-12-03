#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <vector>
#include <cstring>
using namespace std;

class ChessBoard
{
public:
	ChessBoard();
	ChessBoard(const ChessBoard &x, const int move_val);
public: 
	// position of chess piece
	vector<int> King, opp_King;		// 帅 ( 将 )
	vector<int> Rook, opp_Rook;		// 车
	vector<int> Knight, opp_Knight;	// 马
	vector<int> Cannon, opp_Cannon;	// 炮
	vector<int> Bishop, opp_Bishop;	// 相 ( 象 )
	vector<int> Advisor, opp_Advisor;	// 士
	vector<int> Pawn, opp_Pawn;		// 兵 ( 卒 )

	vector<int> move;	// 着法
	long long hash_val;	// 局面的hash值
	int ThisSideColor, OppSideColor;	// RED, BLACK
	bool turnRed() const;
	bool turnBlack() const;
	int chess_color(int pos) const;	// RED, BLACK, EMPTY, OBSTACLE
	int chess_type(int pos) const;	// KING, ROOK, ..., PAWN, EMPTY, OBSTACLE
private:
	int whatOn[256];	// 16x16的棋盘
	void init_pos_chessPiece();
	void init_pos_ThisSidePiece();
	void init_pos_OppSidePiece();
	void init_whatOn();
	void modify_whatOn(const int move_val);
	void init_hash_val();
	long long cal_single_hash_val(const int seed, const int mod) const;
};

const int N_PIECE = 256;

const int RED = 16, BLACK = 32, EMPTY = 0, OBSTACLE = 11;
const int KING = 0, ROOK = 1, KNIGHT = 2, CANNON = 3, BISHOP = 4, ADVISOR = 5, PAWN = 6;

const int ORIGIN_CHESSBOARD[16][16] = {
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
{ 11, 11, 11, 33, 34, 36, 37, 32, 37, 36, 34, 33, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 35, 00, 00, 00, 00, 00, 35, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 38, 00, 38, 00, 38, 00, 38, 00, 38, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 22, 00, 22, 00, 22, 00, 22, 00, 22, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 19, 00, 00, 00, 00, 00, 19, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 00, 00, 00, 00, 00, 00, 00, 00, 00, 11, 11, 11, 11 },
{ 11, 11, 11, 17, 18, 20, 21, 16, 21, 20, 18, 17, 11, 11, 11, 11 },
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
{ 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11 },
};

#endif