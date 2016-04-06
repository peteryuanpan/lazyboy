#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<vector>
#include<map>
using namespace std;
typedef long long ll;
#define mem(a) memset(a, 0, sizeof(a))
#define glen(s) (int)strlen(s)
#define slen(s) (int)s.length()
#define peter cout << "i am peter" << endl
const char adress[] = {"/Users/peteryuanpan/Chess/12-data.txt"};
const int N = 10010;
int JiangX[] = {1, -1, 0, 0};
int JiangY[] = {0, 0, 1, -1};
int XiangX[] = {2, 2, -2, -2};
int XiangY[] = {2, -2, 2, -2};
int ShiX[] = {1, 1, -1, -1};
int ShiY[] = {1, -1, 1, -1};
int BinX[] = {-1, 0, 0};
int BinY[] = {0, 1, -1};


const int nrow = 10, ncol = 9;
class Data{
public:
    char s[nrow][ncol + 1];//棋盘
    bool red;//是否轮到红棋走
    int fx, fy;//起将军作用棋子的x,y坐标
    int state;//0:未确定状态, 1:必胜状态, 2:必败状态
    int fa;//父亲节点
    void mv(int x, int y, char c){s[x][y] = c;}
    char At(int x, int y){return s[x][y];}
    void print(){
        for(int i = 0; i < nrow; i++) printf("%s\n", s[i]);
        printf("red = %d\nsta = %d\n", red, state);
        printf("f = (%d, %d)\n", fx, fy);
        printf("faid = %d\n", fa);
    }
}d[N];
int nd, dtmp[N], ndtmp;
void Newd(int id, int fx, int fy, int x1, int y1, int x2, int y2){//复制构造新棋盘
    d[++nd] = d[id];
    d[nd].red = !d[id].red;
    d[nd].fx = fx, d[nd].fy = fy;
    d[nd].state = 0, d[nd].fa = -1;
    //旧点(x1, y1) -> 新点(x2, y2);
    d[nd].mv(x2, y2, d[nd].At(x1, y1));
    d[nd].mv(x1, y1, '_');
}


class Edge{
public:
    int fr, to;
    int next;
    int x1, y1;//父图的移动棋子的坐标
    int x2, y2;//移动后的棋子坐标
}edge[N * 5];
int head[N], nedge;
void init_Edge(){
    nedge = 0;
    memset(head, -1, sizeof(head));
}
void add_Edge(int fr, int to){//建树边
    int t = ++nedge;
    edge[t].fr = fr, edge[t].to = to;
    edge[t].next = head[fr], head[fr] = nedge;
    d[to].fa = fr;
    
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
    if(d[fr].At(x2, y2) != '_' && d[to].At(x2, y2) == '_'){
        swap(x1, x2), swap(y1, y2);
    }
    edge[t].x1 = x1, edge[t].y1 = y1;
    edge[t].x2 = x2, edge[t].y2 = y2;
}


/*--------Hash集--------*/
class Hash{
public:
    string qi;//将车马炮相士兵
    int seed, p1, p2;
    Hash(){
        qi = "JCMPXSB";
        hashto.clear();
        seed = 23333, p1 = 1e9 + 7, p2 = 1e9 + 9;
    }
    int idx(char c){// _JCMPXSBjsmpxsb
        if(c == '_') return 0;
        int res = 0;
        if(islower(c)) c = c - 'a' + 'A', res += 7;
        for(int i = 0; i < slen(qi); i++) if(qi[i] == c){res += i + 1; break;}
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
        ll h1 = get_hash(id, p1);
        ll h2 = get_hash(id, p2);
        return h1 * max(p1, p2) + h2;
    }
    map<ll, int>hashto;
    int existed(int id){//判断该棋盘是否存在，存在则返回编号
        ll h = get_d_hash(id);
        if(hashto.find(h) != hashto.end()) return hashto[h];
        hashto[h] = id;
        return 0;
    }
}Hash;


/*--------判断类+基本操作类--------*/
class Basic_judge_operation{
public://基本判断集
    bool isRed(int id, int x, int y){return isupper(d[id].s[x][y]);}
    bool isBlack(int id, int x, int y){return islower(d[id].s[x][y]);}
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
        if(Fuck_Jiang(nd, !red)) nd--;
        else{
            int t = Hash.existed(nd);
            if(t) nd--, dtmp[++ndtmp] = t;
            else dtmp[++ndtmp] = nd;
        }
    }
    void Jiang_xy(int id, int &Jx, int &Jy, char des){//找将
        Jx = -1;
        if(des == 'J'){
            for(int i = nrow - 1; i > nrow - 4 && Jx == -1; i--){
                for(int j = 0; j < ncol && Jx == -1; j++){
                    if(d[id].s[i][j] == des) Jx = i, Jy = j;
                }
            }
        }
        else if(des == 'j'){
            for(int i = 0; i < 3 && Jx == -1; i++){
                for(int j = 0; j < ncol && Jx == -1; j++){
                    if(d[id].s[i][j] == des) Jx = i, Jy = j;
                }
            }
        }
        else {printf("Wrong for Jiang_xy!\n"); while(1);}
    }
    
public://分别判断将、车、马、炮、象、士、兵能否走到dx, dy
    bool Jiang_object(int id, int x, int y, int dx, int dy){
        if(abs(x - dx) + abs(y - dy) == 1){
            if(dy < 3 || dy > 5) return false;
            if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
            return true;
        }
        else return false;
    }
    bool Che_object(int id, int x, int y, int dx, int dy){
        if(x == dx){
            for(int k = min(y, dy) + 1; k < max(y, dy); k++){
                if(d[id].s[x][k] != '_') return false;
            }
            return true;
        }
        else if(y == dy){
            for(int k = min(x, dx) + 1; k < max(x, dx); k++){
                if(d[id].s[k][y] != '_') return false;
            }
            return true;
        }
        else return false;
    }
    bool Ma_object(int id, int x, int y, int dx, int dy){
        return false;
    }
    bool Pao_object(int id, int x, int y, int dx, int dy){
        if(d[id].At(dx, dy) == '_') return false;//对炮来说，(dx, dy)不可以是空地
        int n = 0;
        if(x == dx){
            for(int k = min(y, dy) + 1; k < max(y, dy) && n <= 1; k++){
                if(d[id].s[x][k] != '_') n++;
            }
        }
        else if(y == dy){
            for(int k = min(x, dx) + 1; k < max(x, dx) && n <= 1; k++){
                if(d[id].s[k][y] != '_') n++;
            }
        }
        else n = 100;
        return n == 1;
    }
    bool Xiang_object(int id, int x, int y, int dx, int dy){
        if(abs(x - dx) == 2 && abs(y - dy) == 2){
            if(acrossRiver(x, dx)) return false;
            int mx = (x + dx) >> 1, my = (y + dy) >> 1;
            if(d[id].At(mx, my) != '_') return false;
            return true;
        }
        else return false;
    }
    bool Shi_object(int id, int x, int y, int dx, int dy){
        if(abs(x - dx) == 1 && abs(y - dy) == 1){
            if(dy < 3 || dy > 5) return false;
            if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
            return true;
        }
        else return false;
    }
    bool Bin_object(int id, int x, int y, int dx, int dy){
        if(abs(x - dx) + abs(y - dy) == 1){
            if(x < dx) return false;//兵不能后退
            if(x == 5 && x == dx) return false;//未过河不能横走
            return true;
        }
        else return false;
    }
    
public://分别对判断车、马、炮、象、士、兵能否走到一个线段上(不含端点), 如果可以，求出dx,dy
    bool Che_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx = x1, dy = y;
        else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx = x, dy = y1;
        else return false;
        return Che_object(id, x, y, dx, dy);
    }
    bool Ma_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        return false;
    }
    bool Pao_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx = x1, dy = y;
        else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx = x, dy = y1;
        else return false;
        return Pao_object(id, x, y, dx, dy);
    }
    bool Xiang_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        for(int k = 0; k < 4; k++){
            dx = x + XiangX[k], dy = y + XiangY[k];
            if(!inChess(dx, dy) || !Xiang_object(id, x, y, dx, dy)) continue;
            if(OnSegment(dx, dy, x1, y1, x2, y2)) return true;
        }
        return false;
    }
    bool Shi_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        for(int k = 0; k < 4; k++){
            dx = x + ShiX[k], dy = y + ShiY[k];
            if(dy < 3 || dy > 5) continue;
            if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) continue;
            if(OnSegment(dx, dy, x1, y1, x2, y2)) return true;
        }
        return false;
    }
    bool Bin_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
        for(int k = 0; k < 3; k++){
            dx = x + BinX[k], dy = y + BinY[k];
            if(!inChess(dx, dy)) continue;
            if(!Bin_object(id, x, y, dx, dy)) continue;
            if(OnSegment(dx, dy, x1, y1, x2, y2)) return true;
        }
        return false;
    }
    
public://判断是否是一种将军局面
    bool Fuck_Jiang(int id, bool red){
        //red为true表示红将黑，否则轮黑将红
        int Jx, Jy, jx, jy;
        Jiang_xy(id, Jx, Jy, 'J'), Jiang_xy(id, jx, jy, 'j');
        if(Jvsj(id, Jx, Jy, jx, jy)) return true;//将对将
        
        string str = red? "JCMPXSB" : "jcmpxsb";//攻击棋子
        if(!red) swap(Jx, jx), swap(Jy, jy);
        
        //车将
        for(int i = 0, t = 1; i < nrow; i++){
            if(d[id].At(i, jy) == str[t] && Che_object(id, i, jy, jx, jy)) return true;
        }
        for(int j = 0, t = 1; j < ncol; j++){
            if(d[id].At(jx, j) == str[t] && Che_object(id, jx, j, jx, jy)) return true;
        }
        //马将
        //..
        //炮将
        for(int i = 0, t = 3; i < nrow; i++){
            if(d[id].At(i, jy) == str[t] && Pao_object(id, i, jy, jx, jy)) return true;
        }
        for(int j = 0, t = 3; j < ncol; j++){
            if(d[id].At(jx, j) == str[t] && Pao_object(id, jx, j, jx, jy)) return true;
        }
        //兵将
        int zx, zy;
        for(int k = 0, t = 6; k < 3; k++){
            zx = jx + BinX[k], zy = jy + BinY[k];
            if(!inChess(zx, zy)) continue;
            if(d[id].At(zx, zy) == str[t] && Bin_object(id, zx, zy, jx, jy)) return true;
        }
        return false;
    }
    
public://判断(x1,y1)到(x2,y2)线段s上是否有n个点(不包括端点)，如果有，则求出从(x1,y1)开始走第k个点
    bool find_OnLine(int id, int x1, int y1, int x2, int y2, int n, int k, int &ansx, int &ansy){
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
                int t = -1, dx = 0, dy = 0;
                if(c == str[1] && Che_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 1;
                //if c == str[2]
                else if(c == str[3] && Che_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 3;
                else if(c == str[4] && Xiang_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 4;
                else if(c == str[5] && Shi_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 5;
                else if(c == str[6] && Bin_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 6;
                if(t == -1) continue;
                if(t == 3 && d[id].At(dx, dy) != '_') continue;
                Newd(id, -1, -1, i, j, dx, dy);
                ok_Newd(red);
            }
        }
    }
    
private://分别移动车、马、炮、象、士、兵来躲炮
    int id, red, fx, fy, jx, jy;
    void Move_che(int x, int y){
        if(fx == jx){
            for(int i = 0; i < nrow; i++){
                if(Che_object(id, x, y, i, y) && !isBlack(id, i, y)){
                    Newd(id, -1, -1, x, y, i, y);
                    ok_Newd(red);
                }
            }
        }
        else if(fy == jy){
            for(int j = 0; j < ncol; j++){
                if(Che_object(id, x, y, x, j) && !isBlack(id, x, j)){
                    Newd(id, -1, -1, x, y, x, j);
                    ok_Newd(red);
                }
            }
        }
        else printf("Wrong for Move_c!\n");
    }
    void Move_ma(int x, int y){
    }
    void Move_pao(int x, int y){
        if(fx == jx){
            for(int i = 0; i < nrow; i++){
                if((Che_object(id, x, y, i, y)&&d[id].At(i, y)=='_') ||
                   (Pao_object(id, x, y, i, y)&&isRed(id, i, y))){
                    Newd(id, -1, -1, x, y, i, y);
                    ok_Newd(red);
                }
            }
        }
        else if(fy == jy){
            for(int j = 0; j < ncol; j++){
                if((Che_object(id, x, y, x, j)&&d[id].At(x, j)=='_') ||
                   (Pao_object(id, x, y, x, j)&&isRed(id, x, j))){
                    Newd(id, -1, -1, x, y, x, j);
                    ok_Newd(red);
                }
            }
        }
        else printf("Wrong for Move_p!\n");
    }
    void Move_xiang(int x, int y){
        int zx, zy;
        for(int k = 0; k < 4; k++){
            zx = x + XiangX[k], zy = y + XiangY[k];
            if(!inChess(zx, zy) || !Xiang_object(id, x, y, zx, zy)) continue;
            Newd(id, -1, -1, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_shi(int x, int y){
        int zx, zy;
        for(int k = 0; k < 4; k++){
            zx = x + ShiX[k], zy = y + ShiY[k];
            if(zy < 3 || zy > 5) continue;
            if(zx < 0 || zx >= nrow || (zx >= 3 && zx < nrow - 3)) continue;
            Newd(id, -1, -1, x, y, zx, zy);
            ok_Newd(red);
        }
    }
    void Move_bin(int x, int y){
        int zx, zy;
        for(int k = 0; k < 3; k++){
            zx = x + BinX[k], zy = y + BinY[k];
            if(!inChess(zx, zy)) continue;
            if(!Bin_object(id, x, y, zx, zy)) continue;
            Newd(id, -1, -1, x, y, zx, zy);
            ok_Newd(red);
        }
    }
public://移动(fx, fy)到(jx, jy)之间的棋子，以达到未将军的局面
    void Avoid_Pao(int nowid, bool red1, int fx1, int fy1, int jx1, int jy1){
        //red为true表示移动红棋子来躲
        id = nowid, red = red1, fx = fx1, fy = fy1, jx = jx1, jy = jy1;
        int mx, my;
        find_OnLine(id, fx, fy, jx, jy, 1, 1, mx, my);
        if(red && !isRed(id, mx, my)) return;
        else if(!red && !isBlack(id, mx, my)) return;
        char c = d[id].At(mx, my);
        if(c == 'c') Move_che(mx, my);
        //if c == 'm'
        else if(c == 'p') Move_pao(mx, my);
        else if(c == 'x') Move_xiang(mx, my);
        else if(c == 's') Move_shi(mx, my);
        else if(c == 'b') Move_bin(mx, my);
    }
}bjo;


/*--------处理出能红将黑的棋盘--------*/
class RedFuckBlack{
private:
    int jx, jy, id;
private://分别用车、马、炮、兵来将军
    void C_fuck(int x, int y){
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
        //车不动，移动其他棋子来打
        //..
    }
    void M_fuck(int x, int y){
    }
    void P_fuck(int x, int y){
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
        //炮直线吃子后打
        int mx = 0, my = 0;
        if(bjo.find_OnLine(id, x, y, jx, jy, 3, 2, mx, my)){//能走能将
            if(bjo.isBlack(id, mx, my)){//能吃
                Newd(id, mx, my, x, y, mx, my);
                bjo.ok_Newd(true);
            }
        }
        //炮不动，移动其他棋子到将军线上来打
        if(bjo.Che_object(id, x, y, jx, jy)) bjo.Block_Che(id, true, x, y, jx, jy);
        //炮不动，将其他棋子从将军线上移开来打
        //..
    }
    void B_fuck(int x, int y){
        int zx, zy;
        for(int k = 0; k < 3; k++){
            zx = x + BinX[k], zy = y + BinY[k];
            if(!bjo.inChess(zx, zy)) continue;
            if(bjo.Bin_object(id, x, y, zx, zy) && !bjo.isRed(id, zx, zy)){//能走/能吃
                if(bjo.Bin_object(id, zx, zy, jx, jy)){//能将
                    Newd(id, zx, zy, x, y, zx, zy);
                    bjo.ok_Newd(true);
                }
            }
        }
    }
public://处理红棋将军
    void Fuck_Black(int nowid, int x, int y){
        id = nowid;
        bjo.Jiang_xy(id, jx, jy, 'j');
        char c = d[id].At(x, y);
        if(c == 'C') C_fuck(x, y);
        else if(c == 'M') M_fuck(x, y);
        else if(c == 'P') P_fuck(x, y);
        else if(c == 'B') B_fuck(x, y);
    }
}red;


/*--------红将黑后，处理出不被将的棋盘(暂时不考虑反将)--------*/
class BlackAvoidRed{
private:
    int id, jx, jy, fx, fy;
private:
    void Eat_fucker(int fx, int fy){//判断危险红棋子能否被吃掉
        string str = "jcmpxsb";
        for(int i = 0; i < nrow; i++){
            for(int j = 0; j < ncol; j++){
                if(!bjo.isBlack(id, i, j)) continue;
                char c = d[id].At(i, j);
                int t = -1;
                if(c == 'j' && bjo.Jiang_object(id, i, j, fx, fy)) t = 0;
                //if c == 'm'
                else if(c == 'c' && bjo.Che_object(id, i, j, fx, fy)) t = 1;
                else if(c == 'p' && bjo.Pao_object(id, i, j, fx, fy)) t = 3;
                else if(c == 'x' && bjo.Xiang_object(id, i, j, fx, fy)) t = 4;
                else if(c == 's' && bjo.Shi_object(id, i, j, fx, fy)) t = 5;
                else if(c == 'b' && bjo.Bin_object(id, i, j, fx, fy)) t = 6;
                if(t == -1) continue;
                Newd(id, -1, -1, i, j, fx, fy);
                bjo.ok_Newd(false);
            }
        }
    }
    void Move_j(){//移动黑将
        for(int k = 0; k < 4; k++){
            int zx = jx + JiangX[k], zy = jy + JiangY[k];
            if(zy < 3 || zy > 5) continue;
            if(zx < 0 || zx >= nrow || (zx >= 3 && zx < nrow - 3)) continue;
            if(bjo.isBlack(id, zx, zy)) continue;
            Newd(id, -1, -1, jx, jy, zx, zy);
            bjo.ok_Newd(false);
        }
    }
public://处理黑棋，使得其不被将军
    void Avoid_Red(int nowid){
        id = nowid;
        bjo.Jiang_xy(id, jx, jy, 'j');
        fx = d[id].fx, fy = d[id].fy;
        Eat_fucker(fx, fy);
        Move_j();
        char c = d[id].At(fx, fy);
        if(c == 'C') bjo.Block_Che(id, false, fx, fy, jx, jy);
        else if(c == 'P'){
            bjo.Block_Che(id, false, fx, fy, jx, jy);//堵红炮可以等于堵红车
            bjo.Avoid_Pao(id, false, fx, fy, jx, jy);
        }
    }
}black;


/*--------博弈搜索--------*/
int vis[N];
bool dfs(int nowid){
    vis[nowid] = 1;
    int fr, to, sta = 0;
    if(d[nowid].red){//Turn red
        for(int i = 0; i < nrow && !(sta & 2); i++){
            for(int j = 0; j < ncol && !(sta & 2); j++){
                if(!bjo.isRed(nowid, i, j)) continue;
                fr = ndtmp + 1, red.Fuck_Black(nowid, i, j), to = ndtmp;
                for(int k = fr; k <= to && !(sta & 2); k++){
                    int toid = dtmp[k];
                    add_Edge(nowid, toid);
                    if(vis[toid] == 1) sta |= 1;
                    else if(vis[toid] == -1) sta |= d[toid].state;
                    else sta |= dfs(toid)? 1 : 2;
                }
            }
        }
    }
    else{//Turn black
        fr = ndtmp + 1, black.Avoid_Red(nowid), to = ndtmp;
        for(int k = fr; k <= to && !(sta & 2); k++){
            int toid = dtmp[k];
            add_Edge(nowid, toid);
            if(vis[toid] == 1) sta |= 2;
            else if(vis[toid] == -1) sta |= d[toid].state;
            else sta |= dfs(toid)? 1 : 2;
        }
    }
    d[nowid].state = (sta & 2)? 1 : 2;
    vis[nowid] = -1;
    return d[nowid].state == 1;
}


/*--------基本函数--------*/
string order;
void init(){//初始化
    mem(vis);
    mem(d), Newd(0, -1, -1, 0, 0, 0, 0);
    ndtmp = 0, dtmp[++ndtmp] = 1;
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
void work(int nowid);
void bfs_print(){//按顺序输出棋盘
    vector<int>v;
    for(int nowid = 1; nowid <= nd; nowid++){
        printf("nowid = %d\n", nowid);
        d[nowid].print();
        for(int i = head[nowid]; i != -1; i = edge[i].next) v.push_back(edge[i].to);
        if(v.size()) printf("to = ");
        for(int i = (int)v.size() - 1; i >= 0; i--) printf("%d%s", v[i], i == 0? "\n" : ", ");
        printf("\n\n");
        v.clear();
    }
}
int main(){
    init();
    input();
    bool ok = dfs(1);
    //bfs_print();
    printf("sta[1] = %d\n", d[1].state);
    printf("nd = %d\n", nd);
    if(ok){
        printf("Successful!\n");
        while(true){
            printf("Type 'y' to begin\n");
            cin >> order;
            if(order == "y"){
                printf("\n");
                work(1);
                break;
            }
        }
    }
    else printf("No solution!\n");
    //bfs_print();
    return 0;
}


/*--------调控指令--------*/
void printTwo(int nowid, int toid){
    for(int i = 0; i < nrow; i++){
        printf("%s", d[nowid].s[i]);
        if(i == 4) printf("  --->  ");
        else printf("        ");
        printf("%s\n", d[toid].s[i]);
    }
    printf("nowid = %d    ", nowid);
    printf("    toid = %d\n", toid);
}
void work(int nowid){
    int toid = -1, eid = -1;
    if(d[nowid].red){
        printf("[Turn red to go]\n");
        for(int i = head[nowid]; i != -1 && toid == -1; i = edge[i].next){
            int to = edge[i].to;
            if(d[to].state == 2) eid = i, toid = to;
        }
        if(toid == -1){printf("Unsuccessful to go!\n"); return;}
        printf("Red can go like that: ");
        printf("(%d, %d) -> ", edge[eid].x1, edge[eid].y1);
        printf("(%d, %d)\n", edge[eid].x2, edge[eid].y2);
        printTwo(nowid, toid), printf("\n\n");
        work(toid);
    }
    else{
        printf("[Turn black to go]\n");
        if(head[nowid] == -1){printf("Unsuccessful to go!\n"); return;}
        int x1, y1, x2, y2;
        while(true){
            printf("Type black's movement\n");
            printf("(x1, y1) -> (x2, y2)\n");
            cin >> x1 >> y1 >> x2 >> y2;
            for(int i = head[nowid]; i != -1 && toid == -1; i = edge[i].next){
                int to = edge[i].to;
                if(edge[i].x1 == x1 && edge[i].y1 == y1){
                    if(edge[i].x2 == x2 && edge[i].y2 == y2) toid = to;
                }
            }
            if(toid == -1){
                printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
                printf("Wrong input!\n\n");
            }
            else{
                printf("Black goes like that: ");
                printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
                printTwo(nowid, toid);
                while(true){
                    printf("Type 'y' to go next or 'n' to reType.\n");
                    cin >> order;
                    if(order == "y") break;
                    else if(order == "n") {toid = -1; break;}
                }
                if(toid == -1) continue;
                else {printf("\n\n"), work(toid); break;}
            }
        }
    }
    return;//work done
}
