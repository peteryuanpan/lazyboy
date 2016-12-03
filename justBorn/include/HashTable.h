/*--------哈希表--------*/
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "HeadFile.h"

class HashTable{
public:
    HashTable();//构造函数
    string str;//字符集
    int idx(char c);//字符映射到数字
    map<ll, int>hashto;//hash值->棋盘id
    int seed, p1, p2;//hash种子数、模数1、模数2
    
private://获得一个棋盘的单hash值
    ll getSingleHash(int id, int p);
    
private://获得一个棋盘的双hash值
    ll getDoubleHash(int id);
    
public://判断该棋盘是否存在，存在则返回编号，否则返回0，且更新HASH表
	int find(int id);
};
extern HashTable Hash;
#endif
