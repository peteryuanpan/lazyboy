/*--------哈希表--------*/
#include "HashTable.h"

HashTable Hash;

//字符映射到数字，_JCMPXSBjsmpxsb
int HashTable::idx(char c){
    int res = -1;
    for(int i = 0; i < slen(s) && res == -1; i++){
        if(s[i] == c) res = i;
    }
    return res;
}

//获得一个棋盘的单hash值
ll HashTable::getSingleHash(int id, int p){
    ll h = 0;
    for(int i = nrow - 1; i >= 0; i--){
        for(int j = ncol - 1; j >= 0; j--){
            h = h * seed + idx(d[id].s[i][j]);
            if(h >= p) h %= p;
        }
    }
    return h;
}

//获得一个棋盘的双hash值
ll HashTable::getDoubleHash(int id){
    ll h1 = getSingleHash(id, p1);
    ll h2 = getSingleHash(id, p2);
    return h1 * max(p1, p2) + h2;
}

//判断该棋盘是否存在，存在则返回编号，否则返回0
int HashTable::find(int id){
    ll h = getDoubleHash(id);
    if(hashto.find(h) != hashto.end()) return hashto[h];
    else{
        hashto[h] = id;
        return 0;
    }
}
