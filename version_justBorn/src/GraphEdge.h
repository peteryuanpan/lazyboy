/*--------博弈图边集--------*/
#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H
#include "HeadFile.h"

class GraphEdge{
public:
    int fr, to, next;
    void build_info(int fr1, int to1, int next1);
};

//*e1:存储博弈图的边，*e2:存储*e1反图的边
extern GraphEdge e1[M], e2[M];

//ne:图边的数目，*h:临接表头节点
extern int ne1, ne2, h1[N], h2[N];

//toid:存储后继棋盘id, 建图时起辅助作用
extern int toid[M], ntoid;

//记录每个棋盘的toid的起末位置
extern int Fr[N], To[N];

//建立新的边
void addGraphEdge(int fr, int to, GraphEdge *e, int &ne, int *h);

//得出d1->d2的移动棋子坐标(x1,y1)->(x2,y2)
void getMoveXY(GraphEdge e, int &x1, int &y1, int &x2, int &y2);
#endif
