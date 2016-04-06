/*--------棋盘类--------*/
#include "ChessBoard.h"

//N个棋盘
ChessBoard d[N];

//nd:棋盘数
int nd;

//toid:存储后继棋盘id, 建图时起辅助作用
int toid[M], ntoid;

//将(x, y)上的棋子改为c
void ChessBoard::mv(int x, int y, char c){
    s[x][y] = c;
}

//查询(x, y)上的棋子
char ChessBoard::At(int x, int y){
    return s[x][y];
}

//初始化棋盘
void initChessBoard(){
    nd = 0;
    Newd(0, -1, -1, 0, 0, 0, 0);
}

//生成新棋盘
void Newd(int id, int fx, int fy, int x1, int y1, int x2, int y2){//复制构造新棋盘
    assert(nd + 1 < N);
    d[++nd] = d[id];
    d[nd].red = !d[id].red;
    d[nd].fx = fx, d[nd].fy = fy;
    d[nd].sta = 8;
    d[nd].step = d[id].step + 1;
    //旧点(x1, y1) -> 新点(x2, y2);
    d[nd].mv(x2, y2, d[nd].At(x1, y1));
    d[nd].mv(x1, y1, '_');
}
