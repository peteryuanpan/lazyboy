/*--------高级操作类--------*/
#ifndef MOVETOFUCKORAVOID_H
#define MOVETOFUCKORAVOID_H
#include "HeadFile.h"
#include "ChessBoard.h"
#include "BasicFunction.h"

union MoveToFuckOrAvoid{
class Object{
private://分别移动将车马炮象士兵，以达到将军或者躲将的目的
    void Move_jiang(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_che(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_ma(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_pao(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_xiang(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_shi(int id, bool red, int colorType, int x, int y, int fx, int fy);
    
    void Move_bin(int id, bool red, int colorType, int x, int y, int fx, int fy);

public://移动(x, y)上的棋子，以达到将军或者躲将的目的
    void Fuck(int id, bool red, int x, int y, int fx, int fy);
    
    void Avoid(int id, bool red, int x, int y, int fx, int fy);
}Object;

class Segment{
public://移动一个棋子严格到(fx,fy)-(jx,jy)线段上，以达到将军或者躲将的目的
    void Fuck(int id, bool red, int fx, int fy, int jx, int jy);
    
    void Avoid(int id, bool red, int fx, int fy, int jx, int jy);
}Segment;

};
extern MoveToFuckOrAvoid Move;
#endif
