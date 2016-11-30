/*--------博弈图边集--------*/
#include "GraphEdge.h"

//*e1:存储博弈图的边，*e2:存储*e1反图的边
GraphEdge e1[M], e2[M];

//ne:图边的数目，*h:临接表头节点
int ne1, ne2, h1[N], h2[N];

//toid:存储后继棋盘id, 建图时起辅助作用
int toid[M], ntoid;

//记录每个棋盘的toid的起末位置
int Fr[N], To[N];

void GraphEdge::build_info(int fr1, int to1, int next1){
    fr = fr1, to = to1, next = next1;
}

//建立新的边
void addGraphEdge(int fr, int to, GraphEdge *e, int &ne, int *h){//建图边
    for(int i = h[fr]; i != -1; i = e[i].next){
        if(e[i].to == to) return;//去重
    }
    if(ne + 1 >= M) {SearchStop = true; return;}//！
    int t = ++ne;
    e[t].build_info(fr, to, h[fr]);
    h[fr] = ne;
}

//得出d1->d2的移动棋子坐标(x1,y1)->(x2,y2)
void getMoveXY(GraphEdge e, int &x1, int &y1, int &x2, int &y2){
    x1 = y1 = x2 = y2 = -1;
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(d[e.fr].At(i, j) != d[e.to].At(i, j)){
                if(x1 == -1) x1 = i, y1 = j;
                else if(x2 == -1) x2 = i, y2 = j;
                else printf("Wrong for getMoveXY!\n");
            }
        }
    }
    if(d[e.fr].At(x2, y2) != '_' && d[e.to].At(x2, y2) == '_'){
        swap(x1, x2), swap(y1, y2);
    }
}
