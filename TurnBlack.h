/*--------红将黑后，处理出不被将的棋盘--------*/
#ifndef TURNBLACK_H
#define TURNBLACK_H
#include "HeadFile.h"
#include "MoveToFuckOrAvoid.h"

class TurnBlack{
private://尝试吃掉危险红棋子
    void Eat_fucker(int id, int fx, int fy);
    
private://移动黑将，躲避将军
    void Move_j(int id, int jx, int jy);
    
private://堵住红马
    void Block_M(int id, int jx, int jy, int fx, int fy);
    
public://处理黑棋，使得其不被将军
    void AvoidRed(int id);
    
};
extern TurnBlack Black;
#endif
