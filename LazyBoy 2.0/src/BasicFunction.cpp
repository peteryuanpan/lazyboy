/*--------基本判断+操作函数--------*/
#include "BasicFunction.h"

//搜索停止标识符
bool SearchStop;

//判断新棋盘是否合法，如果合法，加入候搜索区
void Filter(bool red){
    //red表示当前是走红
    if(isFuck(nd, !red) || (red&&!isFuck(nd, red))) nd--;
    else{
        if(ntoid + 1 >= M) {SearchStop = true; return;}//！
        int t = Hash.find(nd);
        if(t) nd--, toid[++ntoid] = t;
        else toid[++ntoid] = nd;
    }
}

//判断(x,y)棋子是否是红色
bool isRed(int id, int x, int y){
    return isupper(d[id].s[x][y]);
}

//判断(x,y)棋子是否是黑色
bool isBlack(int id, int x, int y){
    return islower(d[id].s[x][y]);
}

//求(x,y)棋子颜色种类，1:红色，-1:黑色，0:空格
int color(int id, int x, int y){
    if(isRed(id, x, y)) return 1;
    else if(isBlack(id, x, y)) return -1;
    else return 0;
}

//判断(x,y)棋子是否在棋盘内
bool inChess(int x, int y){
    return x >= 0 && x < nrow && y >= 0 && y < ncol;
}

//判断(x,y)是否严格在(x1,y1)到(x2,y2)的线段上
bool OnSegment(int x, int y, int x1, int y1, int x2, int y2){
    if(x1 == x2 && x == x1 && y > min(y1, y2) && y < max(y1, y2)) return true;
    else if(y1 == y2 && y == y1 && x > min(x1, x2) && x < max(x1, x2)) return true;
    else return false;
}

//判断棋子从横坐标x1走到x2是否过河了
bool acrossRiver(int x1, int x2){
    int t1 = x1 < 5 ? -1 : 1, t2 = x2 < 5 ? -1 : 1;
    return t1 * t2 < 0;
}

//判断当前是否是一个将对将的局面
bool Jvsj(int id, int Jx, int Jy, int jx, int jy){
    if(Jy == jy){
        for(int i = min(Jx, jx) + 1; i < max(Jx, jx); i++){
            if(d[id].s[i][jy] != '_') return false;
        }
        return true;
    }
    else return false;
}

//寻找老将的坐标
void Jiang_xy(int id, int &Jx, int &Jy, char des){//找将
    Jx = -1;
    if(des == 'J'){
        for(int i = nrow - 1; i > nrow - 4 && Jx == -1; i--){
            for(int j = 0; j < ncol && Jx == -1; j++){
                if(d[id].At(i, j) == des) Jx = i, Jy = j;
            }
        }
    }
    else if(des == 'j'){
        for(int i = 0; i < 3 && Jx == -1; i++){
            for(int j = 0; j < ncol && Jx == -1; j++){
                if(d[id].At(i, j) == des) Jx = i, Jy = j;
            }
        }
    }
}

//判断将能否从(x, y)到达(dx, dy), 只看位置
bool Jiang_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(abs(x - dx) + abs(y - dy) == 1){
        if(dy < 3 || dy > 5) return false;
        if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
        return true;
    }
    else return false;
}

//判断车能否从(x, y)到达(dx, dy), 只看位置
bool Che_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(x == dx){
        for(int k = min(y, dy) + 1; k < max(y, dy); k++){
            if(d[id].At(x, k) != '_') return false;
        }
        return true;
    }
    else if(y == dy){
        for(int k = min(x, dx) + 1; k < max(x, dx); k++){
            if(d[id].At(k, y) != '_') return false;
        }
        return true;
    }
    else return false;
}

//判断马能否从(x, y)到达(dx, dy), 只看位置
bool Ma_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if((abs(x - dx) == 2&&abs(y - dy) == 1) ||
       (abs(x - dx) == 1&&abs(y - dy) == 2)){
        int mx = x + (dx - x) / 2, my = y + (dy - y) / 2;
        if(d[id].At(mx, my) != '_') return false;//马脚被堵
        return true;
    }
    else return false;
}

//判断炮能否从(x, y)到达(dx, dy), 只看位置
bool Pao_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(d[id].At(dx, dy) == '_') return false;//对炮来说，(dx, dy)不可以是空地
    int n = 0;
    if(x == dx){
        for(int k = min(y, dy) + 1; k < max(y, dy) && n <= 1; k++){
            if(d[id].At(x, k) != '_') n++;
        }
    }
    else if(y == dy){
        for(int k = min(x, dx) + 1; k < max(x, dx) && n <= 1; k++){
            if(d[id].At(k, y) != '_') n++;
        }
    }
    else n = 100;
    return n == 1;
}

//判断象能否从(x, y)到达(dx, dy), 只看位置
bool Xiang_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(abs(x - dx) == 2 && abs(y - dy) == 2){
        if(acrossRiver(x, dx)) return false;
        int mx = (x + dx) >> 1, my = (y + dy) >> 1;
        if(d[id].At(mx, my) != '_') return false;
        return true;
    }
    else return false;
}

//判断士能否从(x, y)到达(dx, dy), 只看位置
bool Shi_object(int id, int x, int y, int dx, int dy){
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(abs(x - dx) == 1 && abs(y - dy) == 1){
        if(dy < 3 || dy > 5) return false;
        if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
        return true;
    }
    else return false;
}

//判断兵能否从(x, y)到达(dx, dy), 只看位置
bool Bin_object(int id, bool red, int x, int y, int dx, int dy){
    //red为true表示判断红兵
    if(!inChess(dx, dy) || !inChess(x, y)) return false;
    if(abs(x - dx) + abs(y - dy) == 1){
        if(red && x < dx) return false;
        if(red && x > 4 && x == dx) return false;
        if(!red && x > dx) return false;
        if(!red && x < 5 && x == dx) return false;
        return true;
    }
    else return false;
}

//判断车能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Che_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx[n] = x1, dy[n++] = y;
    else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx[n] = x, dy[n++] = y1;
    if(!Che_object(id, x, y, dx[n - 1], dy[n - 1])) n--;
}

//判断马能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Ma_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    for(int i = 0; i < 8; i++){
        dx[n] = x + MaX[i], dy[n] = y + MaY[i];
        if(!Ma_object(id, x, y, dx[n], dy[n])) continue;
        if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
    }
}

//判断炮能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Pao_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx[n] = x1, dy[n++] = y;
    else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx[n] = x, dy[n++] = y1;
    if(!Pao_object(id, x, y, dx[n - 1], dy[n - 1])) n--;
}

//判断象能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Xiang_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    for(int i = 0; i < 4; i++){
        dx[n] = x + XiangX[i], dy[n] = y + XiangY[i];
        if(!Xiang_object(id, x, y, dx[n], dy[n])) continue;
        if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
    }
}

//判断士能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Shi_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    for(int i = 0; i < 4; i++){
        dx[n] = x + ShiX[i], dy[n] = y + ShiY[i];
        if(!Shi_object(id, x, y, dx[n], dy[n])) continue;
        if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
    }
}

//判断兵能否严格从(x, y)到达线段(x1,y1)-(x2,y2), 若可以求出到达坐标(dx, dy)
void Bin_segment(int id, bool red, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
    for(int i = 0; i < 4; i++){
        dx[n] = x + BinX[i], dy[n] = y + BinY[i];
        if(!Bin_object(id, red, x, y, dx[n], dy[n])) continue;
        if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
    }
}

//判断老将是否被车将军
bool Che_Jiang(int id, bool red, int jx, int jy){
    char t = red? 'C' : 'c';
    for(int i = 0; i < nrow; i++){
        if(Che_object(id, i, jy, jx, jy) && d[id].At(i, jy) == t) return true;
    }
    for(int j = 0; j < ncol; j++){
        if(Che_object(id, jx, j, jx, jy) && d[id].At(jx, j) == t) return true;
    }
    return false;
}

//判断老将是否被马将军
bool Ma_Jiang(int id, bool red, int jx, int jy){
    char t = red? 'M' : 'm';
    for(int k = 0; k < 8; k++){
        int zx = jx + MaX[k], zy = jy + MaY[k];
        if(Ma_object(id, zx, zy, jx, jy) && d[id].At(zx, zy) == t) return true;
    }
    return false;
}

//判断老将是否被炮将军
bool Pao_Jiang(int id, bool red, int jx, int jy){
    char t = red? 'P' : 'p';
    for(int i = 0; i < nrow; i++){
        if(Pao_object(id, i, jy, jx, jy) && d[id].At(i, jy) == t) return true;
    }
    for(int j = 0; j < ncol; j++){
        if(Pao_object(id, jx, j, jx, jy) && d[id].At(jx, j) == t) return true;
    }
    return false;
}

//判断老将是否被兵将军
bool Bin_Jiang(int id, bool red, int jx, int jy){
    char t = red? 'B' : 'b';
    for(int k = 0; k < 4; k++){//!
        int zx = jx + BinX[k], zy = jy + BinY[k];
        if(Bin_object(id, red, zx, zy, jx, jy) && d[id].At(zx, zy) == t) return true;
    }
    return false;
}

//判断当前是否是一个将军局面
bool isFuck(int id, bool red){
    //red为true表示红将黑，否则黑将红
    int Jx, Jy, jx, jy;
    Jiang_xy(id, Jx, Jy, 'J'), Jiang_xy(id, jx, jy, 'j');
    if(Jvsj(id, Jx, Jy, jx, jy)) return true;//将对将
    
    if(!red) swap(Jx, jx), swap(Jy, jy);
    
    if(Che_Jiang(id, red, jx, jy)) return true;
    else if(Ma_Jiang(id, red, jx, jy)) return true;
    else if(Pao_Jiang(id, red, jx, jy)) return true;
    else if(Bin_Jiang(id, red, jx, jy)) return true;
    else return false;
}


//判断线段(x1,y1)-(x2,y2)上是否有A-B个点(不含端点)，如果有，则求出从(x1,y1)严格开始走的第k个点
bool find_OnLine(int id, int x1, int y1, int x2, int y2, int A, int B, int k, int &ansx, int &ansy){
    ansx = ansy = -1;
    int coun = 0;
    if(x1 == x2 && y1 < y2){
        for(int j = y1 + 1; j < y2; j++){
            if(d[id].At(x1, j) != '_'){coun++; if(coun == k) ansx = x1, ansy = j;}
        }
    }
    else if(x1 == x2 && y1 > y2){
        for(int j = y1 - 1; j > y2; j--){
            if(d[id].At(x1, j) != '_'){coun++; if(coun == k) ansx = x1, ansy = j;}
        }
    }
    else if(y1 == y2 && x1 < x2){
        for(int i = x1 + 1; i < x2; i++){
            if(d[id].At(i, y1) != '_'){coun++; if(coun == k) ansx = i, ansy = y1;}
        }
    }
    else if(y1 == y2 && x1 > x2){
        for(int i = x1 - 1; i > x2; i--){
            if(d[id].At(i, y1) != '_'){coun++; if(coun == k) ansx = i, ansy = y1;}
        }
    }
    else coun = -100;
    return coun >= A && coun <= B;
}

//求每个点到1号点的最短路, spfa算法
void spfa_ShortestPath(int *inq){
    for(int i = 1; i <= nd; i++) inq[i] = 0, d[i].step = 1e9;
    Queue q; q.clear();
    q.push(1), inq[1] = 1, d[1].step = 0;
    while(!q.empty()){
        int now = q.front();
        q.pop(), inq[now] = 0;
        for(int i = h1[now]; i != -1; i = e1[i].next){
            int to = e1[i].to;
            if(d[now].step + 1 < d[to].step){
                d[to].step = d[now].step + 1;
                if(!inq[to]) q.push(to), inq[to] = 1;
            }
        }
    }
}

//逆向拓扑，求出每个点的博弈态
void ReverseTopo(int *in){
    for(int i = 1; i <= nd; i++) in[i] = 0, d[i].sta = 8;
    for(int i = 1; i <= ne2; i++) in[e2[i].to]++;
    Queue q; q.clear();
    for(int i = 1; i <= nd; i++) if(d[i].vis && !in[i]) q.push(i), d[i].sta = 2;
    while(!q.empty()){
        int id = q.front();
        q.pop();
        for(int i = h2[id]; i != -1; i = e2[i].next){
            int to = e2[i].to;
            if(!d[to].vis || in[to] <= 0) continue; 
            in[to]--;
            if(d[id].sta == 2) in[to] = -1, q.push(to), d[to].sta = 1;//必胜
            else if(in[to] == 0) q.push(to), d[to].sta = 2;//必败
        }
    }
    //剩下的且访问过的点是和局态
    for(int i = 1; i <= nd; i++) if(d[i].vis && d[i].sta == 8) d[i].sta = 4;
}

//求出在博弈情况下红棋的最短路径
/*
前提：博弈图已建好，每个点博弈态已确认
前提：红方从1号点开始走，一定能走到一个叶子点，使得黑棋无路可走
描述：红方希望从1号点到叶子点路径越短越好，而黑方希望越长越好，双方都用最优策略
目的：求出红方每个必胜点应该到达哪个必败点，存储在Ansto数组中
*/
bool spfaGP_canUpdate(int now, int Valnow, int fr, int Valfr, int AnstoFr){
	if(!d[fr].vis || AnstoFr == -1) return false;
	if(d[now].red && d[fr].sta != 2) return false;
	if(!d[now].red && d[fr].sta != 1) return false;

	if(d[now].red && Valfr + 1 < Valnow) return true;
	else if(!d[now].red && Valfr + 1 > Valnow) return true;
	else return false;
}
void spfa_GamePath(int *f, int *inq, int *Ansto){
	for(int i = 1; i <= nd; i++){
		f[i] = d[i].red? 1e9 : -1e9, inq[i] = 0, Ansto[i] = -1;
	}
	Queue q; q.clear();
	for(int i = 1; i <= nd; i++){
		if(d[i].vis && !d[i].red && d[i].sta == 2 && h1[i] == -1){
			q.push(i), f[i] = 0, inq[i] = 0, Ansto[i] = 0;
		}
	}
	while(!q.empty()){
		int id = q.front();
		q.pop(), inq[id] = 0;
		for(int i = h2[id]; i != -1; i = e2[i].next){
			int to = e2[i].to;
			if(!d[to].vis) continue;
			if(d[id].red && d[to].sta != 2) continue;
			if(!d[id].red && d[to].sta != 1) continue;

			if(Ansto[to] != id){
				if(spfaGP_canUpdate(to, f[to], id, f[id], Ansto[id])){
					f[to] = f[id] + 1;
					Ansto[to] = id;
					if(!inq[to]) q.push(to), inq[to] = 1;
				}
			}
			else{//Ansto[to] equals id
				f[to] = d[to].red? 1e9 : -1e9, Ansto[to] = -1;
				for(int j = h1[to]; j != -1; j = e1[j].next){
					int t = e1[j].to;
					if(spfaGP_canUpdate(to, f[to], t, f[t], Ansto[t])){
						f[to] = f[t] + 1;
						Ansto[to] = t;
					}
				}
				if(!inq[to]) q.push(to), inq[to] = 1;
			}
		}
	}
}
