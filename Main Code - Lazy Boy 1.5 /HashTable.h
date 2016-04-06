/*--------哈希表--------*/
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "HeadFile.h"
#include "ChessBoard.h"

class HashTable{
public:
    HashTable(){
        s = "_JCMPXSBjcmpxsb";
        seed = 23333, p1 = 1e9 + 7, p2 = 1e9 + 9;
        hashto.clear();
        find(1);
    }
    
private:
    map<ll, int>hashto;//hash值->棋盘id
    string s;//字符集
    int seed, p1, p2;//hash种子数、模数1、模数2
    
private://字符映射到数字
    int idx(char c);
    
private://获得一个棋盘的单hash值
    ll getSingleHash(int id, int p);
    
private://获得一个棋盘的双hash值
    ll getDoubleHash(int id);
    
public://判断该棋盘是否存在，存在则返回编号，否则返回0
    int find(int id);
    
};
extern HashTable Hash;
#endif
