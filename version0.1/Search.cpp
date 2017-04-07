/*--------博弈搜索类--------*/
#include "Search.h"

Search ThisSearch;

//初始化棋盘
void Search::initChessBoard(){//可优化
    for(int i = 1; i < N; i++) d[i] = ChessBoard();
    nd = 0, Newd(0, -1, -1, -1, -1, -1, -1);
    Hash.hashto.clear();
    Hash.find(1);
}

//初始化GraphEdge
void Search::initGraphEdge(){//可优化
    ne1 = 0, memset(h1, -1, sizeof(h1));
    ne2 = 0, memset(h2, -1, sizeof(h2));
    ntoid = 0, memset(toid, 0, sizeof(toid));
    memset(Fr, 0, sizeof(Fr));
    memset(To, -1, sizeof(To));
}

//搜索前初始化
void Search::SearchInit(){//可优化
    initChessBoard(), initGraphEdge();
    SearchStop = 0;
}

//建立每个点的后继表
void Search::getNewChessBoard(int id){
    Fr[id] = ntoid+ 1;
    if(d[id].red) Red.FuckBlack(id);//Turn red
    else Black.AvoidRed(id);//Turn black
    To[id] = ntoid;
}

//获得一个新棋盘的状态
int Search::getSta(int sta){
    if(sta & 2) return 1;
    else if(sta & 8) return 8;
    else if(sta & 4) return 4;
    else return 2;
}

//博弈搜索并建图
int Search::dfs(int id){
    if(d[id].red && d[id].step > stopstep){//控制步数上限
        d[id].vis = -1;
        return d[id].sta = 2;
    }

    d[id].vis = 1;
    if(To[id] - Fr[id] + 1 == 0) getNewChessBoard(id);//建立后继表
    if(SearchStop) return 0;//!!
    int sta = 0;
    for(int i = Fr[id]; i <= To[id] && !(sta&2); i++){//搜索后继, 同时建图
        int t = toid[i];
    	addGraphEdge(id, t, e1, ne1, h1);
    	addGraphEdge(t, id, e2, ne2, h2);
        sta |= d[t].vis? d[t].sta : dfs(t);
        if(SearchStop) return 0;//!!
    }
    return d[id].sta = getSta(sta);
}

//dfs->spfa->逆向拓扑->ok?->dfs...
void Search::SearchWork(){
    ne1 = ne2 = 0;//初始化
    for(int i = 1; i <= nd; i++){
        d[i].vis = 0, d[i].sta = 8, h1[i] = h2[i] = -1;
    }
    dfs(1);//搜索建图
    if(SearchStop) return;//!!
    spfa_ShortestPath(in);//求最短步数
    ReverseTopo(in);//逆向拓扑求博弈态
}

//控制最短步数区间搜索
bool Search::WorkWithAB(int l, int r){
    bool ok = 0;
    while(l <= r){
        stopstep = (l + r) >> 1;
        printf("stopstep = %d\n", stopstep);
        SearchInit();
        for(int k = 1; k <= 30 && !ok; k++){
            SearchWork();
            printf("k = %d nd = %d SS = %d\n", k, nd, SearchStop);
            if(SearchStop) break;//!!
            if(d[1].sta == 1) ok = 1;//成功！
            else{//循环搜索
                bool f = 0;
                for(int i = 1; i <= nd && !f; i++){
                    if(d[i].vis == -1 && d[i].step < stopstep) f = 1;
                }
                if(f == 0) break;
            }
        }
        if(ok) break;
        if(SearchStop) r = stopstep - 1;
        else l = stopstep + 1;
    }
    if(SearchStop || !ok) SearchInit();
    return ok;
}
