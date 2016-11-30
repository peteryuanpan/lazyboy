/*--------棋盘类--------*/
#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "HeadFile.h"

class ChessBoard{
public:
    ChessBoard();

    char s[nrow][ncol + 1];//棋盘
    
    bool red;//是否轮到红棋走
    
    int fx, fy;//起将军作用棋子的x,y坐标
    
    int sta;//1:必胜状态, 2:必败状态, 4:和局状态, 8:未确定状态
    
    int step;//从1走到当前的最短步数
    
    int vis;//该点可以访问
    
    //将(x, y)上的棋子改为c
    void mv(int x, int y, char c);
    
    //查询(x, y)上的棋子
    char At(int x, int y);
    
};

//N个棋盘
extern ChessBoard d[N];

//nd:棋盘数
extern int nd;

//生成新棋盘
void Newd(int id, int fx, int fy, int x1, int y1, int x2, int y2);
#endif
