#include "FenBoard.h"

FenBoard FB;

//position fen
//rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w - - 0 1 
//moves b0c2 b9c7 c3c4 h9i7

//构造函数
FenBoard::FenBoard(){
    //str = "_JCMPXSBjcmpxsb" -- from HashTable;
    str =   "_KRNCBAPkrncbap";
    //K: King(将); R: Rook(车); N: Knight(马);
    //C: Cannon(炮); B: Bishop(象); A: Advisor(士); P: Pawn(兵)
}

//字符映射到数字
int FenBoard::idx(char c){
    int res = -1;
    for(int i = 0; i < slen(str) && res == -1; i++){
        if(c == str[i]) res = i;
    }
    return res;
}

//Fen String转化为棋盘
void FenBoard::FenToBoard(const char *FenStr, char s[][ncol + 1]){
    int lenstr = glen(FenStr), kr = 0, j = 0;
    for(int i = 0; i < lenstr && FenStr[i] != ' '; i++){
        if(FenStr[i] == '/') kr++, j = 0;
        else if(isdigit(FenStr[i])){
            int to = j + (int)(FenStr[i] - '0');
            for(; j < to; j++) s[kr][j] = Hash.str[idx('_')];
        }
        else s[kr][j++] = Hash.str[idx(FenStr[i])];
    }
}

//棋盘转化为Fen String
void FenBoard::BoardToFen(char *FenStr, char s[][ncol + 1]){
    int t = 0;
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            FenStr[t++] = str[Hash.idx(s[i][j])];
        }
        if(i + 1 != nrow) FenStr[t++] = '/';
    }
    FenStr[t] = '\0';
    
    int lenstr = glen(FenStr); t = 0;
    for(int i = 0, j; i < lenstr;){
        if(FenStr[i] == '_'){
            for(j = i; j < lenstr && FenStr[j] == '_'; j++);
            int n = j - i;
            while(n){
                FenStr[t++] = n % 10 + '0';
                n /= 10;
            }
            i = j;
        }
        else FenStr[t++] = FenStr[i++];
    }
    FenStr[t] = '\0';
}

//两种棋盘坐标表示法下的字符转换
/*
 a9 b9 c9 d9 e9 f9 g9 h9 i9     00 01 02 03 04 05 06 07 08
 a8 b8 c8 d8 e8 f8 g8 h8 i8     10 11 12 13 14 15 16 17 18
 a7 b7 c7 d7 e7 f7 g7 h7 i7     20 21 22 23 24 25 26 27 28
 a6 b6 c6 d6 e6 f6 g6 h6 i6     30 31 32 33 34 35 36 37 38
 a5 b5 c5 d5 e5 f5 g5 h5 i5     40 41 42 43 44 45 46 47 48
 a4 b4 c4 d4 e4 f4 g4 h4 i4     50 51 52 53 54 55 56 57 58
 a3 b3 c3 d3 e3 f3 g3 h3 i3     60 61 62 63 64 65 66 67 68
 a2 b2 c2 d2 e2 f2 g2 h2 i2     70 71 72 73 74 75 76 77 78
 a1 b1 c1 d1 e1 f1 g1 h1 i1     80 81 82 83 84 85 86 87 88
 a0 b0 c0 d0 e0 f0 g0 h0 i0     90 91 92 93 94 95 96 97 98
 */
char FenBoard::ExCharX(char c){
    return 9 - (c - '0') + '0';
}
char FenBoard::ExCharY(char c){
    if(isdigit(c)) return c - '0' + 'a';
    else return c - 'a' + '0';
}

//用Move String移动棋子
void FenBoard::MoveBoard(const char *MoveStr, int MoveNum, char s[][ncol + 1]){
    //b0c2 b9c7 c3c4 h9i7
    int i = 0, x1, y1, x2, y2;
    for(int k = 0; k < MoveNum; k++){
        y1 = ExCharY(MoveStr[i]) - '0';
        x1 = ExCharX(MoveStr[i + 1]) - '0';
        y2 = ExCharY(MoveStr[i + 2]) - '0';
        x2 = ExCharX(MoveStr[i + 3]) - '0';
        s[x2][y2] = s[x1][y1];
        s[x1][y1] = '_';
        i += 5;
    }
}

//以棋子移动得到Move String
void FenBoard::getMoveStr(char *MoveStr, int x1, int y1, int x2, int y2){
    int t = 0;
    MoveStr[t++] = ExCharY(y1 + '0');
    MoveStr[t++] = ExCharX(x1 + '0');
    MoveStr[t++] = ExCharY(y2 + '0');
    MoveStr[t++] = ExCharX(x2 + '0');
    MoveStr[t] = '\0';
}
