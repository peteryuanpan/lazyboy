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
	int whatOn[256];	// 记录棋盘上的棋子
	int whereIs[64];	// 记录棋子所在位置
	int row[16];		// 记录行的二进制信息
	int col[16];		// 记录列的二进制信息
	vector<int> move;	// 记录着法
	long long hash_val;	// 局面的hash值
	int turnWho;
	bool turnRed() const;
	bool turnBlack() const;
private:
	void get_whatOn_origin();
	void modify_whatOn(const int move_val);
	void get_whereIs();
	void get_row();
	void get_col();
	void get_hash_val();
	long long cal_single_hash_val(const int seed, const int mod) const;
};

const int RED = 16, BLACK = 32, EMPTY = 0, OBSTACLE = 63;

const int ORIGIN_CHESSBOARD[16][16] = {
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
{ 63, 63, 63, 33, 35, 39, 41, 32, 42, 40, 36, 34, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 00, 00, 00, 00, 00, 00, 00, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 37, 00, 00, 00, 00, 00, 38, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 43, 00, 44, 00, 45, 00, 46, 00, 47, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 00, 00, 00, 00, 00, 00, 00, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 00, 00, 00, 00, 00, 00, 00, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 27, 00, 28, 00, 29, 00, 30, 00, 31, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 21, 00, 00, 00, 00, 00, 22, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 00, 00, 00, 00, 00, 00, 00, 00, 00, 63, 63, 63, 63 },
{ 63, 63, 63, 17, 19, 23, 25, 16, 26, 24, 20, 18, 63, 63, 63, 63 },
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
{ 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
};

#endif