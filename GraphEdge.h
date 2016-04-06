/*--------博弈图边集--------*/
#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#include "HeadFile.h"
#include "ChessBoard.h"

class GraphEdge{
public:
    int fr, to, next;
    
    int x1, y1;//父图的移动棋子的坐标
    
    int x2, y2;//移动后的棋子坐标
    
    bool ok;//这条边是否真实存在
    
    void build_info(int fr1, int to1, int next1, bool ok1);
    
    void build_xy(int x11, int y11, int x22, int y22);
    
};

//*e1:存储博弈图的边，*e2:存储*e1反图的边
extern GraphEdge e1[M], e2[M];

//ne:图边的数目，*h:临接表头节点
extern int ne1, ne2, h1[N], h2[N];

//初始化GraphEdge
void initGraphEdge();

//建立新的边
void addGraphEdge(int fr, int to, bool ok, GraphEdge *edge, int &nedge, int *head);
#endif
