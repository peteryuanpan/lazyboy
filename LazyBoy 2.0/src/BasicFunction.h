/*--------基本判断+操作函数--------*/
#ifndef BASICFUNCTION_H
#define BASICFUNCTION_H
#include "HeadFile.h"

//将车马象士兵的可能走法
const int JiangX[] = {1, -1, 0, 0};
const int JiangY[] = {0, 0, 1, -1};
const int MaX[] = {-2, -2, -1, 1, 2, 2, 1, -1};
const int MaY[] = {-1, 1, 2, 2, 1, -1, -2, -2};
const int XiangX[] = {2, 2, -2, -2};
const int XiangY[] = {2, -2, 2, -2};
const int ShiX[] = {1, 1, -1, -1};
const int ShiY[] = {1, -1, 1, -1};
const int BinX[] = {1, -1, 0, 0};
const int BinY[] = {0, 0, 1, -1};

//搜索停止标识符
extern bool SearchStop;

//判断新棋盘是否合法，如果合法，加入候搜索区
void Filter(bool red);

//判断当前是否是一个将军局面
bool isFuck(int id, bool red);

//判断(x,y)棋子是否是红色
bool isRed(int id, int x, int y);

//判断(x,y)棋子是否是黑色
bool isBlack(int id, int x, int y);

//求(x,y)棋子颜色种类，1:红色，-1:黑色，0:空格
int color(int id, int x, int y);

//判断(x,y)棋子是否在棋盘内
bool inChess(int x, int y);

//判断(x,y)是否严格在(x1,y1)到(x2,y2)的线段上
bool OnSegment(int x, int y, int x1, int y1, int x2, int y2);

//判断棋子从横坐标x1走到x2是否过河了
bool acrossRiver(int x1, int x2);

//判断当前是否是一个将对将的局面
bool Jvsj(int id, int Jx, int Jy, int jx, int jy);

//寻找老将的坐标
void Jiang_xy(int id, int &Jx, int &Jy, char des);

//判断将能否从(x, y)到达(dx, dy), 只看位置
bool Jiang_object(int id, int x, int y, int dx, int dy);

//判断车能否从(x, y)到达(dx, dy), 只看位置
bool Che_object(int id, int x, int y, int dx, int dy);

//判断马能否从(x, y)到达(dx, dy), 只看位置
bool Ma_object(int id, int x, int y, int dx, int dy);

//判断炮能否从(x, y)到达(dx, dy), 只看位置
bool Pao_object(int id, int x, int y, int dx, int dy);

//判断象能否从(x, y)到达(dx, dy), 只看位置
bool Xiang_object(int id, int x, int y, int dx, int dy);

//判断士能否从(x, y)到达(dx, dy), 只看位置
bool Shi_object(int id, int x, int y, int dx, int dy);

//判断兵能否从(x, y)到达(dx, dy), 只看位置
bool Bin_object(int id, bool red, int x, int y, int dx, int dy);

//判断车能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Che_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断马能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Ma_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断炮能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Pao_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断象能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Xiang_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断士能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Shi_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断兵能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Bin_segment(int id, bool red, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n);

//判断老将是否被车将军
bool Che_Jiang(int id, bool red, int jx, int jy);

//判断老将是否被马将军
bool Ma_Jiang(int id, bool red, int jx, int jy);

//判断老将是否被炮将军
bool Pao_Jiang(int id, bool red, int jx, int jy);

//判断老将是否被兵将军
bool Bin_Jiang(int id, bool red, int jx, int jy);

//判断当前是否是一个将军局面
bool isFuck(int id, bool red);

//判断线段(x1,y1)-(x2,y2)上是否有A-B个点(不含端点)，如果有，则求出从(x1,y1)严格开始走的第k个点
bool find_OnLine(int id, int x1, int y1, int x2, int y2, int A, int B, int k, int &ansx, int &ansy);

//求每个点到1号点的最短路
void spfa_ShortestPath(int *inq);
    
//逆向拓扑，求出每个点的博弈态
void ReverseTopo(int *in);

//求出在博弈情况下红棋的最短路径
/*
前提：博弈图已建好，每个点博弈态已确认
前提：红方从1号点开始走，一定能走到一个叶子点，使得黑棋无路可走
描述：红方希望从1号点到叶子点路径越短越好，而黑方希望越长越好，双方都用最优策略
目的：求出红方每个必胜点应该到达哪个必败点，存储在Ansto数组中
*/
bool spfaGP_canUpdate(int now, int Valnow, int fr, int Valfr, int AnstoFr);
void spfa_GamePath(int *f, int *inq, int *Ansto);
#endif
