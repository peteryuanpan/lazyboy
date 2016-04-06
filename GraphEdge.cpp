/*--------博弈图边集--------*/
#include "GraphEdge.h"

//*e1:存储博弈图的边，*e2:存储*e1反图的边
GraphEdge e1[M], e2[M];

//ne:图边的数目，*h:临接表头节点
int ne1, ne2, h1[N], h2[N];

void GraphEdge::build_info(int fr1, int to1, int next1, bool ok1){
    fr = fr1, to = to1, next = next1, ok = ok1;
}

void GraphEdge::build_xy(int x11, int y11, int x22, int y22){
    x1 = x11, y1 = y11, x2 = x22, y2 = y22;
}

//初始化GraphEdge
void initGraphEdge(){
    ne1 = ne2 = 0;
    memset(h1, -1, sizeof(h1));
    memset(h2, -1, sizeof(h2));
}

//建立新的边
void addGraphEdge(int fr, int to, bool ok, GraphEdge *edge, int &nedge, int *head){//建图边
    for(int i = head[fr]; i != -1; i = edge[i].next){
        if(edge[i].to == to) return;//去重
    }
    
    assert(nedge + 1 < M);
    int t = ++nedge;
    edge[t].build_info(fr, to, head[fr], ok);
    head[fr] = nedge;
    
    int x1, y1, x2, y2;
    x1 = y1 = x2 = y2 = -1;
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(d[fr].At(i, j) != d[to].At(i, j)){
                if(x1 == -1) x1 = i, y1 = j;
                else if(x2 == -1) x2 = i, y2 = j;
                else printf("Wrong for add_Edge!\n");
            }
        }
    }
    if(d[fr].At(x2, y2) != '_' && d[to].At(x2, y2) == '_') swap(x1, x2), swap(y1, y2);
    edge[t].build_xy(x1, y1, x2, y2);
}
