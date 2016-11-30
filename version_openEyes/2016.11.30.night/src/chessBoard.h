#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include <vector>
using namespace std;

const int RED = 16, BLACK = 32;

class ChessBoard
{
public:
	ChessBoard();
	ChessBoard(ChessBoard &x, int move);
public: 
	// 记录棋盘上的棋子
	int whatOn[256];
	// 记录棋子所在位置
	int whereIs[64];
	// 记录行的二进制信息
	int row[16];
	// 记录列的二进制信息
	int col[16];
public:
	// 记录着法
	vector<int> move;
public:
	// 记录当前走子方
	int turnWho;
	// 当前该红方走
	bool turnRed();
	// 当前该黑方走
	bool turnBlack();
};

#endif