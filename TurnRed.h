/*--------处理出能红将黑的棋盘--------*/
#ifndef TURNRED_H
#define TURNRED_H
#include "HeadFile.h"
#include "ChessBoard.h"
#include "BasicFunction.h"
#include "MoveToFuckOrAvoid.h"

class TurnRed{
private://用车来将军
    void C_fuck(int id, int x, int y, int jx, int jy);
    
private://用马来将军
    void M_fuck(int id, int x, int y, int jx, int jy);
    
private://用炮来将军
    void P_fuck(int id, int x, int y, int jx, int jy);
    
private://用兵来将军
    void B_fuck(int id, int x, int y, int jx, int jy);
    
public://处理红棋将军
    void FuckBlack(int id, int x, int y);
    
};
extern TurnRed Red;
#endif
