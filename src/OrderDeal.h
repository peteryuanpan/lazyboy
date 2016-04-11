#ifndef ORDERDEAL_H
#define ORDERDEAL_H
#include "HeadFile.h"
#include "Ucci.h"
#include "FenBoard.h"
#include "Search.h"

class OrderDeal{
public:
	bool first;
	void print(string s);
	int f[N], in[N], Ansto[N];

public://引导阶段输出反馈
	void PreparePrint();

public://isready
	void ReadyOK();

public://bye
	void Bye();

public://setoption newgame: 开始新一轮挑战
	void NewGame();

public://position fen ..: 读入棋盘，并在第一次读入后进行搜索
	void PositionFen();

private://第一次读入棋盘后，进行的操作
	void First();

public://go / go ponder: 进入思考状态，最后返回走法
	void GoPrint();
};

extern OrderDeal OrDeal;
#endif
