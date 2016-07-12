/*--------高级操作类--------*/
#include "MoveToFuckOrAvoid.h"

MoveToFuckOrAvoid Move;

//移动老将，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_jiang(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int k = 0; k < 4; k++){
        int zx = x + JiangX[k], zy = y + JiangY[k];
        if(!Jiang_object(id, x, y, zx, zy)) continue;
        if(colorType == color(id, zx, zy)) continue;
        Newd(id, fx, fy, x, y, zx, zy);
        Filter(red);
    }
}

//移动车，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_che(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int i = 0; i < nrow; i++){
        if(colorType == color(id, i, y)) continue;
        if(Che_object(id, x, y, i, y)){
            Newd(id, fx, fy, x, y, i, y);
            Filter(red);
        }
    }
    for(int j = 0; j < ncol; j++){
        if(colorType == color(id, x, j)) continue;
        if(Che_object(id, x, y, x, j)){
            Newd(id, fx, fy, x, y, x, j);
            Filter(red);
        }
    }
}

//移动马，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_ma(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int k = 0; k < 8; k++){
        int zx = x + MaX[k], zy = y + MaY[k];
        if(!Ma_object(id, x, y, zx, zy)) continue;
        if(colorType == color(id, zx, zy)) continue;
        Newd(id, fx, fy, x, y, zx, zy);
        Filter(red);
    }
}

//移动炮，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_pao(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int i = 0; i < nrow; i++){
        if(colorType == color(id, i, y)) continue;
        if((Che_object(id, x, y, i, y)&&d[id].At(i, y)=='_') ||
           (Pao_object(id, x, y, i, y))){
            Newd(id, fx, fy, x, y, i, y);
            Filter(red);
        }
    }
    for(int j = 0; j < ncol; j++){
        if(colorType == color(id, x, j)) continue;
        if((Che_object(id, x, y, x, j)&&d[id].At(x, j)=='_') ||
           (Pao_object(id, x, y, x, j))){
            Newd(id, fx, fy, x, y, x, j);
            Filter(red);
        }
    }
}

//移动象，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_xiang(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int k = 0; k < 4; k++){
        int zx = x + XiangX[k], zy = y + XiangY[k];
        if(!Xiang_object(id, x, y, zx, zy)) continue;
        if(colorType == color(id, zx, zy)) continue;
        Newd(id, fx, fy, x, y, zx, zy);
        Filter(red);
    }
}

//移动士，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_shi(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int k = 0; k < 4; k++){
        int zx = x + ShiX[k], zy = y + ShiY[k];
        if(!Shi_object(id, x, y, zx, zy)) continue;
        if(colorType == color(id, zx, zy)) continue;
        Newd(id, fx, fy, x, y, zx, zy);
        Filter(red);
    }
}

//移动兵，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Move_bin(int id, bool red, int colorType, int x, int y, int fx, int fy){
    for(int k = 0; k < 4; k++){
        int zx = x + BinX[k], zy = y + BinY[k];
        if(!Bin_object(id, red, x, y, zx, zy)) continue;
        if(colorType == color(id, zx, zy)) continue;
        Newd(id, fx, fy, x, y, zx, zy);
        Filter(red);
    }
}

//移动(x, y)上的棋子，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Object::
    Fuck(int id, bool red, int x, int y, int fx, int fy){
    //red为true表示移动红棋子
    int colorType = red? 1 : -1;
    if(colorType != color(id, x, y)) return;
    string str = red? "JCMPXSB" : "jcmpxsb";
    if(!red) fx = fy = -1;
    char c = d[id].At(x, y);
    if(c == str[0]) Move_jiang(id, red, colorType, x, y, fx, fy);
    else if(c == str[1]) Move_che(id, red, colorType, x, y, fx, fy);
    else if(c == str[2]) Move_ma(id, red, colorType, x, y, fx, fy);
    else if(c == str[3]) Move_pao(id, red, colorType, x, y, fx, fy);
    else if(c == str[4]) Move_xiang(id, red, colorType, x, y, fx, fy);
    else if(c == str[5]) Move_shi(id, red, colorType, x, y, fx, fy);
    else if(c == str[6]) Move_bin(id, red, colorType, x, y, fx, fy);
}
void MoveToFuckOrAvoid::Object::
    Avoid(int id, bool red, int x, int y, int fx, int fy){
    Fuck(id, red, x, y, fx, fy);
}

//移动一个棋子严格到(fx,fy)-(jx,jy)线段上，以达到将军或者躲将的目的
void MoveToFuckOrAvoid::Segment::
    Fuck(int id, bool red, int fx, int fy, int jx, int jy){
    //red表示当前是走红还是走黑，(fx, fy)表示车，(jx, jy)表示将
    string str = red? "JCMPXSB" : "jcmpxsb";
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(!red && !isBlack(id, i, j)) continue;
            else if(red && !isRed(id, i, j)) continue;
            char c = d[id].At(i, j);
            int dx[5], dy[5], n = 0;
            if(c == str[1]) Che_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
            else if(c == str[2]) Ma_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
            else if(c == str[3]) Che_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);//没错
            else if(c == str[4]) Xiang_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
            else if(c == str[5]) Shi_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
            else if(c == str[6]) Bin_segment(id, red, i, j, fx, fy, jx, jy, dx, dy, n);
            for(int k = 0; k < n; k++){
                if(d[id].At(dx[k], dy[k]) != '_') continue;
                if(red) Newd(id, fx, fy, i, j, dx[k], dy[k]);
                else Newd(id, -1, -1, i, j, dx[k], dy[k]);
                Filter(red);
            }
        }
    }
}
void MoveToFuckOrAvoid::Segment::
    Avoid(int id, bool red, int fx, int fy, int jx, int jy){
    Fuck(id, red, fx, fy, jx, jy);
}


