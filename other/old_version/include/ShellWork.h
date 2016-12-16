#ifndef SHELLWORK_H
#define SHELLWORK_H
#include "HeadFile.h"
#include "OrderDeal.h"
#include "FenBoard.h"
#include "Search.h"

class ShellWork{
public:
    string ord;
    bool quit;
    
public:
    //读取Shell下的命令
    string getord();
    
    //将Number转为字符串
    string to_string(int id);
    
    //判断字符串是否是个Number
    bool isNumber(string s);
    
    //将字符串转为Number
    int StrToNumber(string s);
    
    //判断p是否是s的前缀
    bool is_prefix(string p, string s);
    
    //输出一个局面的所有信息
    void print(int id);

    //输出一个局面的所有信息但不输出棋盘
    void printButs(int id);

    //输出棋盘a->棋盘b
    void printTwo(int nowid, int toid);
    
    //暂停处理器
    int Type_y_go_next();
    
    //输出棋盘走法
    int TypeMovement(int nowid, bool red);

    //debug函数
    void Debug(int nowid);

    //input函数
    void input();

    //主执行函数
    void work();
};

extern ShellWork Shell;
#endif
