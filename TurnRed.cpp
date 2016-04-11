/*--------处理出能红将黑的棋盘--------*/
#include "TurnRed.h"

TurnRed Red;

//用车来将军
void TurnRed::C_fuck(int id, int x, int y, int jx, int jy){
    //车转弯将军
    if(Che_object(id, x, y, jx, y) && !isRed(id, jx, y)){//能走/能吃
        if(Che_object(id, jx, y, jx, jy)){//能将
            Newd(id, jx, y, x, y, jx, y);
            Filter(true);
        }
    }
    if(Che_object(id, x, y, x, jy) && !isRed(id, x, jy)){//能走/能吃
        if(Che_object(id, x, jy, jx, jy)){//能将
            Newd(id, x, jy, x, y, x, jy);
            Filter(true);
        }
    }
    
    //车直线吃子后将军
    int mx = 0, my = 0;
    if(find_OnLine(id, x, y, jx, jy, 1, 1, 1, mx, my)){//能走能将
        if(isBlack(id, mx, my)){//能吃
            Newd(id, mx, my, x, y, mx, my);
            Filter(true);
        }
    }
    
    //车不动，将其他棋子从将军线上移开来将
    int dx, dy;
    if(find_OnLine(id, x, y, jx, jy, 1, 1, 1, dx, dy)){
        Move.Object.Fuck(id, true, dx, dy, x, y);
    }
}

//用马来将军
void TurnRed::M_fuck(int id, int x, int y, int jx, int jy){
    //马走“日”字来将军
    for(int k = 0; k < 8; k++){
        int zx = x + MaX[k], zy = y + MaY[k];
        if(Ma_object(id, x, y, zx, zy) && !isRed(id, zx, zy)){//能走/能吃
            if(Ma_object(id, zx, zy, jx, jy)){//能将
                Newd(id, zx, zy, x, y, zx, zy);
                Filter(true);
            }
        }
    }
    //马不动，将其他棋子移开马脚来将
    if((abs(x - jx) == 2 && abs(y - jy) == 1) ||
       (abs(x - jx) == 1 && abs(y - jy) == 2)){
        int mx = x + (jx - x) / 2, my = y + (jy - y) / 2;
        Move.Object.Fuck(id, true, mx, my, x, y);
    }
}

//用炮将军
void TurnRed::P_fuck(int id, int x, int y, int jx, int jy){
    //炮拐弯将军
    if((Che_object(id, x, y, jx, y)&&d[id].At(jx, y) == '_') ||//能走 
       (Pao_object(id, x, y, jx, y)&&isBlack(id, jx, y))){//能吃
        if(Pao_object(id, jx, y, jx, jy)){//能将
            Newd(id, jx, y, x, y, jx, y);
            Filter(true);
        }
    }
    if((Che_object(id, x, y, x, jy)&&d[id].At(x, jy) == '_') ||//能走
       (Pao_object(id, x, y, x, jy)&&isBlack(id, x, jy))){//能吃
        if(Pao_object(id, x, jy, jx, jy)){//能将
            Newd(id, x, jy, x, y, x, jy);
            Filter(true);
        }
    }
    
    int mx = 0, my = 0, dx, dy;
    //炮向前吃子后将军
    if(find_OnLine(id, x, y, jx, jy, 3, 3, 2, mx, my)){//能走能将
        if(isBlack(id, mx, my)){//能吃
            Newd(id, mx, my, x, y, mx, my);
            Filter(true);
        }
    }

    //炮向后吃子后将军
    if(Che_object(id, x, y, jx, jy)){//前提在一条直线上
        int zx = 0, zy = 0;
        if(x == jx && y < jy) zx = x, zy = -1;
        else if(x == jx && y > jy) zx = x, zy = ncol;
        else if(y == jy && x < jx) zx = -1, zy = y;
        else if(y == jy && x > jx) zx = nrow, zy = y;
        if(find_OnLine(id, x, y, zx, zy, 2, 100, 2, mx, my)){
            if(isBlack(id, mx, my)){
                Newd(id, mx, my, x, y, mx, my);
                Filter(true);
            }
        }
    }
    
    //炮不动，移动其他棋子到将军线上来将
    if(Che_object(id, x, y, jx, jy)) Move.Segment.Fuck(id, true, x, y, jx, jy);
    
    //炮不动，将其他棋子从将军线上移开或移动来将
    if(find_OnLine(id, x, y, jx, jy, 2, 2, 1, dx, dy)){
        Move.Object.Fuck(id, true, dx, dy, x, y);
    }
    if(find_OnLine(id, x, y, jx, jy, 2, 2, 2, dx, dy)){
        Move.Object.Fuck(id, true, dx, dy, x, y);
    }
}

//用兵将军
void TurnRed::B_fuck(int id, int x, int y, int jx, int jy){
    //移动兵来将军
    for(int k = 0; k < 4; k++){
        int zx = x + BinX[k], zy = y + BinY[k];
        if(Bin_object(id, true, x, y, zx, zy) && !isRed(id, zx, zy)){//能走/能吃
            if(Bin_object(id, true, zx, zy, jx, jy)){//能将
                Newd(id, zx, zy, x, y, zx, zy);
                Filter(true);
            }
        }
    }
}

//处理红棋将军
void TurnRed::FuckBlack(int id){
    int jx, jy;
    Jiang_xy(id, jx, jy, 'j');
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            char c = d[id].At(i, j);
            if(c == 'C') C_fuck(id, i, j, jx, jy);
            else if(c == 'M') M_fuck(id, i, j, jx, jy);
            else if(c == 'P') P_fuck(id, i, j, jx, jy);
            else if(c == 'B') B_fuck(id, i, j, jx, jy);
        }
    }
}
