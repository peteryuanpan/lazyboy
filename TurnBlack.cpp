/*--------红将黑后，处理出不被将的棋盘--------*/
#include "TurnBlack.h"

TurnBlack Black;

//尝试吃掉危险红棋子
void TurnBlack::Eat_fucker(int id, int fx, int fy){
    string str = "jcmpxsb";
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(!isBlack(id, i, j)) continue;
            char c = d[id].At(i, j);
            int t = -1;
            if(c == str[0] && Jiang_object(id, i, j, fx, fy)) t = 0;
            else if(c == str[1] && Che_object(id, i, j, fx, fy)) t = 1;
            else if(c == str[2] && Ma_object(id, i, j, fx, fy)) t = 2;
            else if(c == str[3] && Pao_object(id, i, j, fx, fy)) t = 3;
            else if(c == str[3] &&
                    d[id].At(fx, fy) == '_' && Che_object(id, i, j, fx, fy)) t = -3;//有用
            else if(c == str[4] && Xiang_object(id, i, j, fx, fy)) t = 4;
            else if(c == str[5] && Shi_object(id, i, j, fx, fy)) t = 5;
            else if(c == str[6] && Bin_object(id, false, i, j, fx, fy)) t = 6;
            else continue;
            Newd(id, -1, -1, i, j, fx, fy);
            Filter(false);
        }
    }
}

//移动黑将，躲避将军
void TurnBlack::Move_j(int id, int jx, int jy){
    for(int k = 0; k < 4; k++){
        int zx = jx + JiangX[k], zy = jy + JiangY[k];
        if(!Jiang_object(id, jx, jy, zx, zy)) continue;
        if(isBlack(id, zx, zy)) continue;
        Newd(id, -1, -1, jx, jy, zx, zy);
        Filter(false);
    }
}

//堵住红马
void TurnBlack::Block_M(int id, int jx, int jy, int fx, int fy){
    int dx = fx + (jx - fx) / 2, dy = fy + (jy - fy) / 2;
    Eat_fucker(id, dx, dy);//堵马可以等于吃危子
}

//处理黑棋，使得其不被将军
void TurnBlack::Avoid_Red(int id){
    int jx, jy, fx, fy, dx, dy;
    Jiang_xy(id, jx, jy, 'j');
    fx = d[id].fx, fy = d[id].fy;
    Eat_fucker(id, fx, fy);
    Move_j(id, jx, jy);
    char c = d[id].At(fx, fy);
    if(c == 'C') Move.Segment.Avoid(id, false, fx, fy, jx, jy);
    else if(c == 'M') Block_M(id, jx, jy, fx, fy);
    else if(c == 'P'){
        Move.Segment.Avoid(id, false, fx, fy, jx, jy);//堵红炮可以等于堵红车
        if(find_OnLine(id, fx, fy, jx, jy, 1, 1, dx, dy)){//移动黑子来躲
            Move.Object.Avoid(id, false, dx, dy, fx, fy);
        }
    }
    //else if c == 'B': 兵无法堵住或躲避
}
