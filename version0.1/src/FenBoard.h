#ifndef FENBOARD_H
#define FENBOARD_H
#include "Ucci.h"
#include "HeadFile.h"

class FenBoard{
public://构造函数
    FenBoard();
    string str;//字符集
    int idx(char c);//字符映射到数字
    
public://Fen String转化为棋盘
    void FenToBoard(const char *FenStr, char s[][ncol + 1]);
    
public://棋盘转化为Fen String
    void BoardToFen(char *FenStr, char s[][ncol + 1]);
    
public://两种棋盘坐标表示法下的字符转换
    char ExCharX(char c);
    char ExCharY(char c);
    
public://用Move String移动棋子
    void MoveBoard(const char *MoveStr, int MoveNum, char s[][ncol + 1]);
    
public://以棋子移动得到Move String
    void getMoveStr(char *MoveStr, int x1, int y1, int x2, int y2);
};
extern FenBoard FB;
#endif
