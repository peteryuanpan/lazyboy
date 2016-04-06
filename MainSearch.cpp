/*--------博弈搜索类--------*/
#include "MainSearch.h"

//求每个点到1号点的最短路, spfa算法
void MainSearch::spfa(int *inq, int *que){
    for(int i = 1; i <= nd; i++) inq[i] = 0, d[i].step = 1e9;
    int t1 = -1, t2 = 0;
    d[1].step = 0, que[++t2] = 1, inq[1] = 1;
    while(t2 - t1 + 1 > 0){
        int now = que[t1++]; inq[now] = 0;
        for(int i = h1[now]; i != -1; i = e1[i].next){
            if(!e1[i].ok) continue;
            int to = e1[i].to;
            if(d[now].step + 1 < d[to].step){
                d[to].step = d[now].step + 1;
                if(!inq[to]) que[++t2] = to, inq[to] = 1;
            }
        }
    }
}

//逆向拓扑，求出每个点的博弈态
void MainSearch::Reverse_Topo(int *in, int *que){
    for(int i = 1; i <= nd; i++) in[i] = 0, d[i].sta = 8;
    for(int i = 1; i <= ne2; i++) if(e2[i].ok) in[e2[i].to]++;
    int t1 = 0, t2 = -1;
    for(int i = 1; i <= nd; i++) if(in[i] == 0) que[++t2] = i, d[i].sta = 2;
    while(t2 - t1 + 1 > 0){
        int id = que[t1++];
        for(int i = h2[id]; i != -1; i = e2[i].next){
            int to = e2[i].to;
            if(!e2[i].ok || in[to] <= 0) continue;
            in[to]--;
            if(d[id].sta == 2) in[to] = -1, que[++t2] = to, d[to].sta = 1;//必胜
            else if(in[to] == 0) que[++t2] = to, d[to].sta = 2;//必败
        }
    }
    for(int i = 1; i <= nd; i++) if(d[i].sta == 8) d[i].sta = 4;
}

//获得新棋盘并建图
void MainSearch::getNewChessBoard(int id){
    int fr = ntoid + 1;
    if(d[id].red){//Turn red
        if(d[id].step >= stopstep){//控制步数上限
            vis[id] = -1, d[id].sta = 2;
            return;
        }
        for(int i = 0; i < nrow; i++){
            for(int j = 0; j < ncol; j++){
                if(isRed(id, i, j)) Red.FuckBlack(id, i, j);
            }
        }
    }
    else Black.Avoid_Red(id);//Turn black
    int to = ntoid;
    for(int i = to; i >= fr; i--){//建立虚边
        addGraphEdge(id, toid[i], false, e1, ne1, h1);
        addGraphEdge(toid[i], id, false, e2, ne2, h2);
    }
}

//获得一个新棋盘的状态
int MainSearch::getSta(int sta){
    if(sta & 2) return 1;
    else if(sta & 8) return 8;
    else if(sta & 4) return 4;
    else return 2;
}

//主博弈搜索函数
int MainSearch::dfs(int id){
    vis[id] = 1;
    int sta = 0;
    if(h1[id] == -1) getNewChessBoard(id);
    for(int i = h1[id]; i != -1 && !(sta&2); i = e1[i].next){//搜索后继
        e1[i].ok = e2[i].ok = true;
        int t = e1[i].to;
        sta |= vis[t]? d[t].sta : dfs(t);
    }
    return d[id].sta = getSta(sta);
}

//dfs->拓扑->spfa->ok?->dfs...
void MainSearch::work(int StopStep){
    stopstep = StopStep;
    initChessBoard(), initGraphEdge();//先初始化
    while(true){
        for(int i = 1; i <= nd; i++) vis[i] = 0;
        ntoid = 0, dfs(1);//先搜索建图
        Reverse_Topo(toid, que);//逆向拓扑求博弈态
        spfa(toid, que);//求最短步数
        if(d[1].sta != 1){//循环搜索
            bool f = 0;
            for(int i = 1; i <= nd && !f; i++){
                if(vis[i] == -1 && d[i].step < stopstep) f = 1;
            }
            if(f == 1) continue;
        }
        break;
    }
    return;//work done
}
