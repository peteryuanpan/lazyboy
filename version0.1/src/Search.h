/*--------博弈搜索类--------*/
#ifndef SEARCH_H
#define SEARCH_H
#include "HeadFile.h"
#include "TurnRed.h"
#include "TurnBlack.h"

const int stopFr = 10, stopTo = 30;//参数可调整！

class Search{
public:
    int in[N];
    
private://搜索最大步数，参数可调整!
    int stopstep;

private://初始化棋盘
	void initChessBoard();

private://初始化GraphEdge
	void initGraphEdge();

private://搜索前初始化
    void SearchInit();
    
private://建立每个点的后继表
    void getNewChessBoard(int id);
    
private://获得一个新棋盘的状态
    int getSta(int sta);
    
private://博弈搜索并建图
    int dfs(int id);

private://dfs->spfa->逆向拓扑->ok?->dfs...
    void SearchWork();
    
public://控制最短步数区间搜索
    bool WorkWithAB(int stopfr, int stopto);

};

extern Search ThisSearch;
#endif
