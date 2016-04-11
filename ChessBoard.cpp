/*--------棋盘类--------*/
#include "ChessBoard.h"

//N个棋盘
ChessBoard d[N];

//nd:棋盘数
int nd;

ChessBoard::ChessBoard(){
    memset(s, 0, sizeof(s));
    red = 0;
    fx = fy = -1;
    sta = 8;
    step = -1;
    vis = 0;
}

//将(x, y)上的棋子改为c
void ChessBoard::mv(int x, int y, char c){
    s[x][y] = c;
}

//查询(x, y)上的棋子
char ChessBoard::At(int x, int y){
    return s[x][y];
}

//生成新棋盘
void Newd(int id, int fx, int fy, int x1, int y1, int x2, int y2){//复制构造新棋盘
    if(nd + 1 >= N) {SearchStop = true; return;}//！
    d[++nd] = ChessBoard();
    for(int i = 0; i < nrow; i++) strcpy(d[nd].s[i], d[id].s[i]);
    d[nd].red = !d[id].red;
    d[nd].fx = fx, d[nd].fy = fy;
    d[nd].step = d[id].step + 1;
    //旧点(x1, y1) -> 新点(x2, y2);
    if(!inChess(x1, y1) || !inChess(x2, y2)) return;
    d[nd].mv(x2, y2, d[nd].At(x1, y1));
    d[nd].mv(x1, y1, '_');
}
