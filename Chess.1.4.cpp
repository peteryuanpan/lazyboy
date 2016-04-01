#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<cassert>
#include<vector>
#include<map>
using namespace std;
typedef long long ll;
#define mem(a) memset(a, 0, sizeof(a))
#define glen(s) (int)strlen(s)
#define slen(s) (int)s.length()
#define peter cout << "i am peter" << endl
const char adress[] = {"/Users/peteryuanpan/Chess/198-神龙跃海.txt"};
const int N = 300010;
int JiangX[] = {1, -1, 0, 0};
int JiangY[] = {0, 0, 1, -1};
int MaX[] = {-2, -2, -1, 1, 2, 2, 1, -1};
int MaY[] = {-1, 1, 2, 2, 1, -1, -2, -2};
int XiangX[] = {2, 2, -2, -2};
int XiangY[] = {2, -2, 2, -2};
int ShiX[] = {1, 1, -1, -1};
int ShiY[] = {1, -1, 1, -1};
int BinX[] = {1, -1, 0, 0};
int BinY[] = {0, 0, 1, -1};


const int nrow = 10, ncol = 9;
class Data{
public:
    char s[nrow][ncol + 1];//棋盘
    bool red;//是否轮到红棋走
    int fx, fy;//起将军作用棋子的x,y坐标
    int sta;//1:必胜状态, 2:必败状态, 4:和局状态, 8:未确定状态
    int step;//从1走到当前的最短步数
    void mv(int x, int y, char c){s[x][y] = c;}
    char At(int x, int y){return s[x][y];}
}d[N];
int nd, toid[N * 2], ntoid;
void print(int id);
void printButs(int id);
void Newd(int id, int fx, int fy, int x1, int y1, int x2, int y2){//复制构造新棋盘
    assert(nd + 1 < N);
    d[++nd] = d[id];
    d[nd].red = !d[id].red;
    d[nd].fx = fx, d[nd].fy = fy;
    d[nd].sta = 8;
    d[nd].step = d[id].step + 1;
    //旧点(x1, y1) -> 新点(x2, y2);
    d[nd].mv(x2, y2, d[nd].At(x1, y1));
    d[nd].mv(x1, y1, '_');
}


class Edge{
public:
    int fr, to, next;
    int x1, y1;//父图的移动棋子的坐标
    int x2, y2;//移动后的棋子坐标
    bool ok;//这条边是否真实存在
    void build(int fr1, int to1, bool ok1){
        fr = fr1, to = to1, ok = ok1;
    }
    void build_xy(int x11, int y11, int x22, int y22){
        x1 = x11, y1 = y11, x2 = x22, y2 = y22;
    }
}e1[N * 2], e2[N * 2];
int ne1, ne2;
int h1[N], h2[N];
void init_Edge(){
    ne1 = ne2 = 0;
    memset(h1, -1, sizeof(h1));
    memset(h2, -1, sizeof(h2));
}
void add_Edge(int fr, int to, bool ok, Edge *edge, int &nedge, int *head){//建图边
    for(int i = head[fr]; i != -1; i = edge[i].next){
        if(edge[i].to == to) return;//去重
    }
    
    int t = ++nedge;
    edge[t].build(fr, to, ok);
    edge[t].next = head[fr], head[fr] = nedge;
    
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


/*--------Hash集--------*/
class Hash{
public:
    string s;
    int seed, p1, p2;
    Hash(){
        s = "JCMPXSBjcmpxsb";
        hashto.clear();
        seed = 23333, p1 = 1e9 + 7, p2 = 1e9 + 9;
    }
    int idx(char c){// _JCMPXSBjsmpxsb
        int res = -1; if(c == '_') res = 0;
        for(int i = isupper(c)? 0 : 7; i < slen(s) && res == -1; i++){
            if(s[i] == c) res = i + 1;
        }
        return res;
    }
    ll get_hash(int id, int p){//获得一个棋盘的单hash值
        ll h = 0;
        for(int i = nrow - 1; i >= 0; i--){
            for(int j = ncol - 1; j >= 0; j--){
                h = h * seed + idx(d[id].s[i][j]);
                if(h >= p) h %= p;
            }
        }
        return h;
    }
    ll get_d_hash(int id){//获得一个棋盘的双hash值
        ll h1 = get_hash(id, p1), h2 = get_hash(id, p2);
        return h1 * max(p1, p2) + h2;
    }
    map<ll, int>hashto;
    int existed(int id){//判断该棋盘是否存在，存在则返回编号
        ll h = get_d_hash(id);
        if(hashto.find(h) != hashto.end()) return hashto[h];
        else {hashto[h] = id; return 0;}
    }
}Hash;


/*--------判断类+基本操作类--------*/
class Basic_judge_operation{
public://基本判断集
    bool isRed(int id, int x, int y){return isupper(d[id].s[x][y]);}
    bool isBlack(int id, int x, int y){return islower(d[id].s[x][y]);}
    int color(int id, int x, int y){
        if(isRed(id, x, y)) return 1;
        else if(isBlack(id, x, y)) return -1;
        else return 0;
    }
    bool inChess(int x, int y){return x >= 0 && x < nrow && y >= 0 && y < ncol;}
    bool OnSegment(int x, int y, int x1, int y1, int x2, int y2){//判断点是否不严格在线段上
        if(x1 == x2 && x == x1 && y > min(y1, y2) && y < max(y1, y2)) return true;
        else if(y1 == y2 && y == y1 && x > min(x1, x2) && x < max(x1, x2)) return true;
        else return false;
    }
    bool acrossRiver(int x1, int x2){//判断x1到x2是否已经过河
        int t1 = x1 < 5 ? -1 : 1, t2 = x2 < 5 ? -1 : 1;
        return t1 * t2 < 0;
    }
    bool Jvsj(int id, int Jx, int Jy, int jx, int jy){//判断是否对将
        if(Jy == jy){
            for(int i = min(Jx, jx) + 1; i < max(Jx, jx); i++){
                if(d[id].s[i][jy] != '_') return false;
            }
            return true;
        }
        else return false;
    }
    
public://基本操作集
    void ok_Newd(bool red){//判断是否可形成一个新局面
        //red表示当前是走红
        if(Fuck_Jiang(nd, !red) || (red&&!Fuck_Jiang(nd, red))) nd--;
        else{
            assert(ntoid + 1 < N);
            int t = Hash.existed(nd);
            if(t) nd--, toid[++ntoid] = t;
            else toid[++ntoid] = nd;
        }
    }
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
        else {printf("Wrong for Jiang_xy!\n"); while(1);}
    }
    
public://分别判断将、车、马、炮、象、士、兵能否走/跳到dx, dy
    bool Jiang_object(int id, int x, int y, int dx, int dy){
        if(!inChess(dx, dy) || !inChess(x, y)) return false;
        if(abs(x - dx) + abs(y - dy) == 1){
            if(dy < 3 || dy > 5) return false;
            if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
            return true;
        }
        else return false;
    }
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
    bool Shi_object(int id, int x, int y, int dx, int dy){
        if(!inChess(dx, dy) || !inChess(x, y)) return false;
        if(abs(x - dx) == 1 && abs(y - dy) == 1){
            if(dy < 3 || dy > 5) return false;
            if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
            return true;
        }
        else return false;
    }
    bool Bin_object(int id, bool red, int x, int y, int dx, int dy){
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
    
public://分别判断车、马、炮、象、士、兵能否走/跳到一个线段上(不含端点), 如果可以，求出dx,dy
    void Che_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx[n] = x1, dy[n++] = y;
        else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx[n] = x, dy[n++] = y1;
        if(!Che_object(id, x, y, dx[n - 1], dy[n - 1])) n--;
    }
    void Ma_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        for(int i = 0; i < 8; i++){
            dx[n] = x + MaX[i], dy[n] = y + MaY[i];
            if(!Ma_object(id, x, y, dx[n], dy[n])) continue;
            if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
        }
    }
    void Pao_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx[n] = x1, dy[n++] = y;
        else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx[n] = x, dy[n++] = y1;
        if(!Pao_object(id, x, y, dx[n - 1], dy[n - 1])) n--;
    }
    void Xiang_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        for(int i = 0; i < 4; i++){
            dx[n] = x + XiangX[i], dy[n] = y + XiangY[i];
            if(!Xiang_object(id, x, y, dx[n], dy[n])) continue;
            if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
        }
    }
    void Shi_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        for(int i = 0; i < 4; i++){
            dx[n] = x + ShiX[i], dy[n] = y + ShiY[i];
            if(!Shi_object(id, x, y, dx[n], dy[n])) continue;
            if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
        }
    }
    void Bin_segment(int id, bool red, int x, int y, int x1, int y1, int x2, int y2, int *dx, int *dy, int &n){
        for(int i = 0; i < 4; i++){
            dx[n] = x + BinX[i], dy[n] = y + BinY[i];
            if(!Bin_object(id, red, x, y, dx[n], dy[n])) continue;
            if(OnSegment(dx[n], dy[n], x1, y1, x2, y2)) n++;
        }
    }
    
private://分别判断车、马、炮、兵能否将到将
    bool Che_Jiang(int id, int t, int jx, int jy, string &str){
        for(int i = 0; i < nrow; i++){
            if(Che_object(id, i, jy, jx, jy) && d[id].At(i, jy) == str[t]) return true;
        }
        for(int j = 0; j < ncol; j++){
            if(Che_object(id, jx, j, jx, jy) && d[id].At(jx, j) == str[t]) return true;
        }
        return false;
    }
    bool Ma_Jiang(int id, int t, int jx, int jy, string &str){
        for(int k = 0; k < 8; k++){
            int zx = jx + MaX[k], zy = jy + MaY[k];
            if(Ma_object(id, zx, zy, jx, jy) && d[id].At(zx, zy) == str[t]) return true;
        }
        return false;
    }
    bool Pao_Jiang(int id, int t, int jx, int jy, string &str){
        for(int i = 0; i < nrow; i++){
            if(Pao_object(id, i, jy, jx, jy) && d[id].At(i, jy) == str[t]) return true;
        }
        for(int j = 0; j < ncol; j++){
            if(Pao_object(id, jx, j, jx, jy) && d[id].At(jx, j) == str[t]) return true;
        }
        return false;
    }
    bool Bin_Jiang(int id, bool red, int t, int jx, int jy, string &str){
        for(int k = 0; k < 4; k++){//!
            int zx = jx + BinX[k], zy = jy + BinY[k];
            if(Bin_object(id, red, zx, zy, jx, jy) && d[id].At(zx, zy) == str[t] ) return true;
        }
        return false;
    }
public://判断是否是一种将军局面
    bool Fuck_Jiang(int id, bool red){
        //red为true表示红将黑，否则轮黑将红
        int Jx, Jy, jx, jy;
        Jiang_xy(id, Jx, Jy, 'J'), Jiang_xy(id, jx, jy, 'j');
        if(Jvsj(id, Jx, Jy, jx, jy)) return true;//将对将
        
        string str = red? "CMPB" : "cmpb";//攻击棋子
        if(!red) swap(Jx, jx), swap(Jy, jy);
        
        if(Che_Jiang(id, 0, jx, jy, str)) return true;
        else if(Ma_Jiang(id, 1, jx, jy, str)) return true;
        else if(Pao_Jiang(id, 2, jx, jy, str)) return true;
        else if(Bin_Jiang(id, red, 3, jx, jy, str)) return true;
        else return false;
    }
    
public://判断(x1,y1)到(x2,y2)线段s上是否有n个点(不包括端点)，如果有，则求出从(x1,y1)开始走的第k个点
    bool find_OnLine(int id, int x1, int y1, int x2, int y2, int n, int k, int &ansx, int &ansy){
        if(k > n) return false;
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
        else coun = -1;
        return coun == n;
    }
    
public://堵住车，达到不将军局面
    void Block_Che(int id, bool red, int fx, int fy, int jx, int jy){
        //red表示当前是走红还是走黑，(fx, fy)表示车，(jx, jy)表示将
        string str = red? "JCMPXSB" : "jcmpxsb";
        for(int i = 0; i < nrow; i++){
            for(int j = 0; j < ncol; j++){
                if(!red && !isBlack(id, i, j)) continue;
                else if(red && !isRed(id, i, j)) continue;
                char c = d[id].At(i, j);
                int dx[5], dy[5], n = 0;
                if(c == str[1]) Che_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
                else if(c == str[2]) Ma_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
                else if(c == str[3]) Che_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);//没错
                else if(c == str[4]) Xiang_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
                else if(c == str[5]) Shi_segment(id, i, j, fx, fy, jx, jy, dx, dy, n);
                else if(c == str[6]) Bin_segment(id, red, i, j, fx, fy, jx, jy, dx, dy, n);
                for(int k = 0; k < n; k++){
                    if(d[id].At(dx[k], dy[k]) != '_') continue;
                    if(red) Newd(id, fx, fy, i, j, dx[k], dy[k]);
                    else Newd(id, -1, -1, i, j, dx[k], dy[k]);
                    ok_Newd(red);
                }
            }
        }
    }
    
private://分别移动帅、车、马、炮、象、士、兵，以达到将军或者躲将的目的
    void Move_jiang(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int k = 0; k < 4; k++){
            int zx = x + JiangX[k], zy = y + JiangY[k];
            if(!Jiang_object(id, x, y, zx, zy)) continue;
            if(colorType == color(id, zx, zy)) continue;
            Newd(id, fx, fy, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_che(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int i = 0; i < nrow; i++){
            if(colorType == color(id, i, y)) continue;
            if(Che_object(id, x, y, i, y)){
                Newd(id, fx, fy, x, y, i, y);
                ok_Newd(red);
            }
        }
        for(int j = 0; j < ncol; j++){
            if(colorType == color(id, x, j)) continue;
            if(Che_object(id, x, y, x, j)){
                Newd(id, fx, fy, x, y, x, j);
                ok_Newd(red);
            }
        }
    }
    void Move_ma(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int k = 0; k < 8; k++){
            int zx = x + MaX[k], zy = y + MaY[k];
            if(!Ma_object(id, x, y, zx, zy)) continue;
            if(colorType == color(id, zx, zy)) continue;
            Newd(id, fx, fy, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_pao(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int i = 0; i < nrow; i++){
            if(colorType == color(id, i, y)) continue;
            if((Che_object(id, x, y, i, y)&&d[id].At(i, y)=='_') ||
               (Pao_object(id, x, y, i, y))){
                Newd(id, fx, fy, x, y, i, y);
                ok_Newd(red);
            }
        }
        for(int j = 0; j < ncol; j++){
            if(colorType == color(id, x, j)) continue;
            if((Che_object(id, x, y, x, j)&&d[id].At(x, j)=='_') ||
               (Pao_object(id, x, y, x, j))){
                Newd(id, fx, fy, x, y, x, j);
                ok_Newd(red);
            }
        }
    }
    void Move_xiang(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int k = 0; k < 4; k++){
            int zx = x + XiangX[k], zy = y + XiangY[k];
            if(!Xiang_object(id, x, y, zx, zy)) continue;
            if(colorType == color(id, zx, zy)) continue;
            Newd(id, fx, fy, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_shi(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int k = 0; k < 4; k++){
            int zx = x + ShiX[k], zy = y + ShiY[k];
            if(!Shi_object(id, x, y, zx, zy)) continue;
            if(colorType == color(id, zx, zy)) continue;
            Newd(id, fx, fy, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_bin(int id, bool red, int colorType, int x, int y, int fx, int fy){
        for(int k = 0; k < 4; k++){
            int zx = x + BinX[k], zy = y + BinY[k];
            if(!Bin_object(id, red, x, y, zx, zy)) continue;
            if(colorType == color(id, zx, zy)) continue;
            Newd(id, fx, fy, x, y, zx, zy);
            ok_Newd(red);
        }
    }
public://移动(x, y)，以达到将军或者躲将的目的
    void MoveToFuckOrAvoid(int id, bool red, int x, int y, int fx, int fy){
        //red为true表示移动红棋子
        int colorType = red? 1 : -1;
        if(colorType != color(id, x, y)) return;
        string str = red? "JCMPXSB" : "jcmpxsb";
        if(!red) fx = fy = -1;
        char c = d[id].At(x, y);
        if(c == str[0]) Move_jiang(id, red, colorType, x, y, fx, fy);
        else if(c == str[1]) Move_che(id, red, colorType, x, y, fx, fy);
        else if(c == str[2]) Move_ma(id, red, colorType, x, y, fx, fy);
        else if(c == str[3]) Move_pao(id, red, colorType, x, y, fx, fy);
        else if(c == str[4]) Move_xiang(id, red, colorType, x, y, fx, fy);
        else if(c == str[5]) Move_shi(id, red, colorType, x, y, fx, fy);
        else if(c == str[6]) Move_bin(id, red, colorType, x, y, fx, fy);
    }
}bjo;


/*--------处理出能红将黑的棋盘--------*/
class RedFuckBlack{
private://分别用车、马、炮、兵来将军
    void C_fuck(int id, int x, int y, int jx, int jy){
        //车转弯打
        if(bjo.Che_object(id, x, y, jx, y) && !bjo.isRed(id, jx, y)){//能走/能吃
            if(bjo.Che_object(id, jx, y, jx, jy)){//能将
                Newd(id, jx, y, x, y, jx, y);
                bjo.ok_Newd(true);
            }
        }
        if(bjo.Che_object(id, x, y, x, jy) && !bjo.isRed(id, x, jy)){//能走/能吃
            if(bjo.Che_object(id, x, jy, jx, jy)){//能将
                Newd(id, x, jy, x, y, x, jy);
                bjo.ok_Newd(true);
            }
        }
        //车直线吃子后打
        int mx = 0, my = 0;
        if(bjo.find_OnLine(id, x, y, jx, jy, 1, 1, mx, my)){//能走能将
            if(bjo.isBlack(id, mx, my)){//能吃
                Newd(id, mx, my, x, y, mx, my);
                bjo.ok_Newd(true);
            }
        }
        //车不动，将其他棋子从将军线上移开来打
        int dx, dy;
        if(bjo.find_OnLine(id, x, y, jx, jy, 1, 1, dx, dy)){
            bjo.MoveToFuckOrAvoid(id, true, dx, dy, x, y);
        }
    }
    void M_fuck(int id, int x, int y, int jx, int jy){
        for(int k = 0; k < 8; k++){
            int zx = x + MaX[k], zy = y + MaY[k];
            if(bjo.Ma_object(id, x, y, zx, zy) && !bjo.isRed(id, zx, zy)){//能走/能吃
                if(bjo.Ma_object(id, zx, zy, jx, jy)){//能将
                    Newd(id, zx, zy, x, y, zx, zy);
                    bjo.ok_Newd(true);
                }
            }
        }
        //马不动，将其他棋子移开马脚来打
        if((abs(x - jx) == 2 && abs(y - jy) == 1) ||
           (abs(x - jx) == 1 && abs(y - jy) == 2)){
            int mx = x + (jx - x) / 2, my = y + (jy - y) / 2;
            bjo.MoveToFuckOrAvoid(id, true, mx, my, x, y);
        }
    }
    void P_fuck(int id, int x, int y, int jx, int jy){
        //炮拐弯打
        if((bjo.Che_object(id, x, y, jx, y)&&d[id].At(jx, y) == '_') ||//能走 
           (bjo.Pao_object(id, x, y, jx, y)&&bjo.isBlack(id, jx, y))){//能吃
            if(bjo.Pao_object(id, jx, y, jx, jy)){//能将
                Newd(id, jx, y, x, y, jx, y);
                bjo.ok_Newd(true);
            }
        }
        if((bjo.Che_object(id, x, y, x, jy)&&d[id].At(x, jy) == '_') ||//能走
           (bjo.Pao_object(id, x, y, x, jy)&&bjo.isBlack(id, x, jy))){//能吃
            if(bjo.Pao_object(id, x, jy, jx, jy)){//能将
                Newd(id, x, jy, x, y, x, jy);
                bjo.ok_Newd(true);
            }
        }
        int mx = 0, my = 0, dx, dy;
        //炮直线吃子后打
        if(bjo.find_OnLine(id, x, y, jx, jy, 3, 2, mx, my)){//能走能将
            if(bjo.isBlack(id, mx, my)){//能吃
                Newd(id, mx, my, x, y, mx, my);
                bjo.ok_Newd(true);
            }
        }
        //炮不动，移动其他棋子到将军线上来打
        if(bjo.Che_object(id, x, y, jx, jy)) bjo.Block_Che(id, true, x, y, jx, jy);
        //炮不动，将其他棋子从将军线上移开来打
        if(bjo.find_OnLine(id, x, y, jx, jy, 2, 1, dx, dy)){
            bjo.MoveToFuckOrAvoid(id, true, dx, dy, x, y);
        }
        //炮不动，将其他棋子在将军线上移动来打
        if(bjo.find_OnLine(id, x, y, jx, jy, 2, 2, dx, dy)){
            bjo.MoveToFuckOrAvoid(id, true, dx, dy, x, y);
        }
    }
    void B_fuck(int id, int x, int y, int jx, int jy){
        for(int k = 0; k < 4; k++){
            int zx = x + BinX[k], zy = y + BinY[k];
            if(bjo.Bin_object(id, true, x, y, zx, zy) && !bjo.isRed(id, zx, zy)){//能走/能吃
                if(bjo.Bin_object(id, true, zx, zy, jx, jy)){//能将
                    Newd(id, zx, zy, x, y, zx, zy);
                    bjo.ok_Newd(true);
                }
            }
        }
    }
public://处理红棋将军
    void Fuck_Black(int id, int x, int y){
        int jx, jy;
        bjo.Jiang_xy(id, jx, jy, 'j');
        char c = d[id].At(x, y);
        if(c == 'C') C_fuck(id, x, y, jx, jy);
        else if(c == 'M') M_fuck(id, x, y, jx, jy);
        else if(c == 'P') P_fuck(id, x, y, jx, jy);
        else if(c == 'B') B_fuck(id, x, y, jx, jy);
    }
}red;


/*--------红将黑后，处理出不被将的棋盘(暂时不考虑反将)--------*/
class BlackAvoidRed{
private://判断危险红棋子能否被吃掉
    void Eat_fucker(int id, int fx, int fy){
        string str = "jcmpxsb";
        for(int i = 0; i < nrow; i++){
            for(int j = 0; j < ncol; j++){
                if(!bjo.isBlack(id, i, j)) continue;
                char c = d[id].At(i, j);
                int t = -1;
                if(c == str[0] && bjo.Jiang_object(id, i, j, fx, fy)) t = 0;
                else if(c == str[1] && bjo.Che_object(id, i, j, fx, fy)) t = 1;
                else if(c == str[2] && bjo.Ma_object(id, i, j, fx, fy)) t = 2;
                else if(c == str[3] && bjo.Pao_object(id, i, j, fx, fy)) t = 3;
                else if(c == str[3] &&
                        d[id].At(fx, fy) == '_' && bjo.Che_object(id, i, j, fx, fy)) t = -3;//有用
                else if(c == str[4] && bjo.Xiang_object(id, i, j, fx, fy)) t = 4;
                else if(c == str[5] && bjo.Shi_object(id, i, j, fx, fy)) t = 5;
                else if(c == str[6] && bjo.Bin_object(id, false, i, j, fx, fy)) t = 6;
                else continue;
                Newd(id, -1, -1, i, j, fx, fy);
                bjo.ok_Newd(false);
            }
        }
    }
private://移动黑将，躲避将军
    void Move_j(int id, int jx, int jy){
        for(int k = 0; k < 4; k++){
            int zx = jx + JiangX[k], zy = jy + JiangY[k];
            if(!bjo.Jiang_object(id, jx, jy, zx, zy)) continue;
            if(bjo.isBlack(id, zx, zy)) continue;
            Newd(id, -1, -1, jx, jy, zx, zy);
            bjo.ok_Newd(false);
        }
    }
private://堵住红马
    void Block_M(int id, int jx, int jy, int fx, int fy){
        int dx = fx + (jx - fx) / 2, dy = fy + (jy - fy) / 2;
        Eat_fucker(id, dx, dy);//堵马可以等于吃危子
    }
public://处理黑棋，使得其不被将军
    void Avoid_Red(int id){
        int jx, jy, fx, fy, dx, dy;
        bjo.Jiang_xy(id, jx, jy, 'j');
        fx = d[id].fx, fy = d[id].fy;
        Eat_fucker(id, fx, fy);
        Move_j(id, jx, jy);
        char c = d[id].At(fx, fy);
        if(c == 'C') bjo.Block_Che(id, false, fx, fy, jx, jy);
        else if(c == 'M') Block_M(id, jx, jy, fx, fy);
        else if(c == 'P'){
            bjo.Block_Che(id, false, fx, fy, jx, jy);//堵红炮可以等于堵红车
            if(bjo.find_OnLine(id, fx, fy, jx, jy, 1, 1, dx, dy)){//移动黑子来躲
                bjo.MoveToFuckOrAvoid(id, false, dx, dy, fx, fy);
            }
        }
        //else if c == 'B': 兵无法堵住或躲避
    }
}black;


/*--------博弈搜索类--------*/
class GameSearch{
private:
    int vis[N], que[N];
private://求每个点到1号点的最短路
    void spfa(int *inq, int *que){
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
private://逆向拓扑，求出每个点的博弈态
    void Reverse_Topo(int *in, int *que){
        for(int i = 1; i <= nd; i++) in[i] = 0, d[i].sta = 8;
        for(int i = 1; i <= ne2; i++) in[e2[i].to]++;
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
private://搜索集
    int get_sta(int sta){
        if(sta & 2) return 1;
        else if(sta & 8) return 8;
        else if(sta & 4) return 4;
        else return 2;
    }
    void getNewChessBoard(int id){//获得新棋盘和建边
        int fr = ntoid + 1;
        if(d[id].red){//Turn red
            if(d[id].step >= stopstep){//控制步数上限
                vis[id] = -1, d[id].sta = 2;
                return;
            }
            for(int i = 0; i < nrow; i++){
                for(int j = 0; j < ncol; j++){
                    if(bjo.isRed(id, i, j)) red.Fuck_Black(id, i, j);
                }
            }
        }
        else black.Avoid_Red(id);//Turn black
        int to = ntoid;
        for(int i = fr; i <= to; i++){//建立虚边
            add_Edge(id, toid[i], false, e1, ne1, h1);
            add_Edge(toid[i], id, false, e2, ne2, h2);
        }
    }
    const static int stopstep = 23;//参数可调整!
    int dfs(int id){//博弈搜索
        vis[id] = 1;
        int sta = 0;
        if(h1[id] == -1) getNewChessBoard(id);
        for(int i = h1[id]; i != -1 && !(sta&2); i = e1[i].next){//搜索后继
            e1[i].ok = e2[i].ok = true;
            int t = e1[i].to;
            sta |= vis[t]? d[t].sta : dfs(t);
        }
        return d[id].sta = get_sta(sta);
        
    }
public://dfs->拓扑->spfa->ok?->dfs...
    void work(){
        for(int i = 1; i <= nd; i++) vis[i] = 0;
        ntoid = 0, dfs(1);
        Reverse_Topo(toid, que);
        spfa(toid, que);
        if(d[1].sta != 1){
            bool f = 0;
            for(int i = 1; i <= nd && !f; i++){
                if(vis[i] == -1 && d[i].step < stopstep) f = 1;
            }
            if(f == 1) work();
        }
        return;//work done
    }
}game;



/*--------基本函数--------*/
string order;
void init(){//初始化
    mem(d), d[0].step = -1;
    Newd(0, -1, -1, 0, 0, 0, 0);
    Hash.existed(1);
    init_Edge();
}
void input(){//输入数据
    FILE *F = fopen(adress, "r");
    char s[100];
    for(int i = 0; i < nrow; i++){
        fgets(s, ncol + 10, F);
        s[glen(s) - 1] = '\0';
        strcpy(d[1].s[i], s);
    }
    fclose(F);
    return;//input done;
}
void debug(int nowid);
void gogogo(int nowid, int last);
int main(){
    init(), input(), game.work();
    
    printf("nd = %d\n", nd);
    if(d[1].sta == 1) printf("Must Win!\n\n");
    else if(d[1].sta == 2) printf("Must Lose!\n\n");
    else if(d[1].sta == 4) printf("Tie!\n\n");
    
    while(true){
        printf("Type 'y' to go or 'n'.\n");
        cin >> order;
        if(order == "y") printf("\n"), gogogo(1, -1);
        else if(order == "n") break;
    }
    return 0;
}


/*--------调控指令集--------*/
void print(int id){
    for(int i = 0; i < nrow; i++) printf("%s\n", d[id].s[i]);
    printButs(id);
}
void printButs(int id){
    printf("id = %5d  sta = %d  red = %d  fx = %d  fy = %d  step = %d\n", 
           id, d[id].sta, d[id].red, d[id].fx, d[id].fy, d[id].step);
    for(int i = h1[id]; i != -1; i = e1[i].next){
        int to = e1[i].to;
        printf("to = %5d  sta = %d  (%d, %d) -> (%d, %d)  E = %s\n",
               to, d[to].sta, e1[i].x1, e1[i].y1, e1[i].x2, e1[i].y2, e1[i].ok? "Y" : "N");
    }
    for(int i = h2[id]; i != -1; i = e2[i].next){
        int to = e2[i].to;
        printf("fa = %5d  sta = %d  (%d, %d) <- (%d, %d)  E = %s\n",
               to, d[to].sta, e2[i].x1, e2[i].y1, e2[i].x2, e2[i].y2, e2[i].ok? "Y" : "N");
    }
    printf("\n");
}
void printTwo(int nowid, int toid){
    printf("Chessbord goes like that:\n");
    for(int i = 0; i < nrow; i++){
        printf("%s", d[nowid].s[i]);
        if(i == 4) printf("  --->  ");
        else printf("        ");
        printf("%s\n", d[toid].s[i]);
    }
    printf("nowid = %d", nowid);
    int t = 8;
    for(int ten = 10; nowid >= ten; ten *= 10) t--;
    while(t--) printf(" ");
    printf("toid = %d\n", toid);
}
void printChessBoard(int id){
    if(id < 1 || id > nd) return;
    printf("\nCheck ChessBoard ");
    cout << to_string(id) + ":"<< endl;
    print(id);
}
int Type_y_go_next(int ty){
    printf("Type 'y' to go next or an Order.\n");
    cin >> order; int x;
    if(order == "y") return 1;
    else if(ty == 1 && order == "n") return -1;
    else if(order == "id") cin >> x, printChessBoard(x);
    return Type_y_go_next(ty);
}
int debug2(int nowid, bool red){
    int x1, y1, x2, y2, toid = -1;
    printf("Type %s's movement\n", red? "red" : "black");
    printf("(x1, y1) -> (x2, y2)\n");
    cin >> x1 >> y1 >> x2 >> y2;
    for(int i = h1[nowid]; i != -1 && toid == -1; i = e1[i].next){
        if(!e1[i].ok) continue;
        int to = e1[i].to;
        if(e1[i].x1 == x1 && e1[i].y1 == y1){
            if(e1[i].x2 == x2 && e1[i].y2 == y2) toid = to;
        }
    }
    if(toid == -1){
        printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
        printf("Wrong input!\n");
        return debug2(nowid, red);
    }
    else{
        printf("%s goes like that: ", red? "Red" : "Black");
        printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
        printTwo(nowid, toid);
        return Type_y_go_next(1) == -1? debug2(nowid, red) : toid;
    }
}
void debug(int nowid){
    int toid;
    if(d[nowid].red){
        printf("[Turn red to go]\n");
        print(nowid);
        toid = debug2(nowid, true);
        printTwo(nowid, toid);
        printf("\n\n\n"), debug(toid);
    }
    else{
        printf("[Turn black to go]\n");
        print(nowid);
        toid = debug2(nowid, false);
        printf("\n\n\n"), debug(toid);
    }
    return;//debug done
}
void gogogo(int nowid, int last){
    int toid, e = -1;
    if(d[nowid].red){
        printf("[Turn red to go]\n");
        print(nowid);
        for(int i = h1[nowid]; i != -1 && e == -1; i = e1[i].next){
            if(!e1[i].ok) continue;
            if(d[e1[i].to].sta == 2){
                if(last != -1 && e1[last].x1 == e1[i].x2 && e1[last].y1 == e1[i].y2){
                    if(e1[last].x2 == e1[i].x1 && e1[last].y2 == e1[i].y1){
                        continue;
                    }
                }
                e = i;
            }
        }
        for(int i = h1[nowid]; i != -1 && e == -1; i = e1[i].next){
            if(!e1[i].ok) continue;
            if(d[e1[i].to].sta == 4) e = i;
        }
        if(toid == -1){printf("Unable to go! What the fuck?!\n"); return;}
        printf("Red can go like that: ");
        printf("(%d, %d) -> (%d, %d)\n", e1[e].x1, e1[e].y1, e1[e].x2, e1[e].y2);
        toid = e1[e].to;
        printTwo(nowid, toid);
        Type_y_go_next(2);
        printf("\n\n\n"), gogogo(toid, e);
    }
    else{
        printf("[Turn black to go]\n");
        print(nowid);
        toid = debug2(nowid, false);
        printf("\n\n\n"), gogogo(toid, last);
    }
    return;//debug done
}
