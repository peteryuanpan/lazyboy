/*--------博弈搜索类--------*/
#ifndef MAINSEARCH_H
#define MAINSEARCH_H
#include "HeadFile.h"
#include "ChessBoard.h"
#include "GraphEdge.h"
#include "TurnRed.h"
#include "TurnBlack.h"

class MainSearch{
private:
    int vis[N], que[N];
    int stopstep;//搜索最大步数，参数可调整!
    
private://求每个点到1号点的最短路
    void spfa(int *inq, int *que);
    
private://逆向拓扑，求出每个点的博弈态
    void Reverse_Topo(int *in, int *que);
    
private://获得新棋盘并建图
    void getNewChessBoard(int id);
    
private://获得一个新棋盘的状态
    int getSta(int sta);
    
private://主博弈搜索函数
    int dfs(int id);
    
public://dfs->拓扑->spfa->ok?->dfs...
    void work(int StopStep);
    
};
#endif
