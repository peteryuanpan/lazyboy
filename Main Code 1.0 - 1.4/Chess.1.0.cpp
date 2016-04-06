#include<cstdio>
#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<map>
using namespace std;
#define mem(a) memset(a, 0, sizeof(a))
#define glen(s) (int)strlen(s)
#define slen(s) (int)s.length()
#define peter cout << "i am peter" << endl
void work(int nowid);
typedef long long ll;
const int N = 5010, M = 1000;
const char adress[] = {"/Users/peteryuanpan/Chess/7-data.txt"};
char s[M + 10];

const int nrow = 10, ncol = 9;
class Data{
public:
    char s[nrow][ncol + 1];//棋盘
    bool red;//是否轮到红棋走
    int fx, fy;//起将军作用棋子的x,y坐标
    int state;//0:未确定状态, 1:必胜状态, 2:必败状态
    void mv(int x, int y, char c){s[x][y] = c;}
    char At(int x, int y){return s[x][y];}
    void print(){
        for(int i = 0; i < nrow; i++) printf("%s\n", s[i]);
        printf("red = %d fx = %d fy = %d\n", red, fx, fy);
    }
}d[N];
int nd, dtmp[N], ndtmp;
void Newd(int id, int fx, int fy){//复制构造新棋盘
    d[++nd] = d[id];
    d[nd].red = !d[id].red;
    d[nd].fx = fx, d[nd].fy = fy;
    d[nd].state = 0;
}
void input(){//输入数据
    FILE *F = fopen(adress, "r");
    for(int i = 0, len; i < nrow; i++){
        fgets(s, M, F);
        len = glen(s), s[len - 1] = '\0';
        strcpy(d[1].s[i], s);
    }
    fclose(F);
    return;//input done;
}

struct Edge{
    int fr, to;
    int next;
    int x1, y1;//父图的移动棋子的坐标
    int x2, y2;//移动后的棋子坐标
}edge[N];
int head[N], nedge;
void init_Edge(){
    nedge = 0;
    memset(head, -1, sizeof(head));
}
void add_Edge(int fr, int to){//建树边
    int t = ++nedge;
    edge[t].fr = fr, edge[t].to = to;
    edge[t].next = head[fr];
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
    if(d[fr].At(x2, y2) != '_' && d[to].At(x2, y2) == '_'){
        swap(x1, x2), swap(y1, y2);
    }
    edge[t].x1 = x1, edge[t].y1 = y1;
    edge[t].x2 = x2, edge[t].y2 = y2;
}
void build_Edge(int id, int frid, int toid){//一个棋盘建向另多个棋盘
    for(int i = frid; i <= toid; i++) add_Edge(id, i);
}


/*--------hash集--------*/
const string qi = "JCMPXSB";//将车马炮相士兵
int idx(char c){
    // _JCMPXSBjsmpxsb
    if(c == '_') return 0;
    int res = 0;
    if(islower(c)) c = c - 'a' + 'A', res += 7;
    for(int i = 0; i < slen(qi); i++) if(qi[i] == c){res += i + 1; break;}
    return res;
}
const int seed = 23333, p1 = 1e9 + 7, p2 = 1e9 + 9;
ll get_hash(int id, int p){
    int l = 0;
    for(int i = 0; i < nrow; i++){//统一到字符串s
        strcpy(s + l, d[id].s[i]);
        l += glen(s + l);
    }
    ll h = 0;
    for(int i = l - 1; i >= 0; i--){//获得hash值
        h = h * seed + idx(s[i]);
        if(h >= p) h %= p;
    }
    return h;
}
ll get_d_hash(int id){//将一个棋盘hash
    ll h1 = get_hash(id, p1);
    ll h2 = get_hash(id, p2);
    return h1 * max(p1, p2) + h2;
}
map<ll, int>hashto;
bool existed(int id){//判断该棋盘是否存在
    ll h = get_d_hash(id);
    if(hashto.find(h) != hashto.end()){
        return true;
    }
    hashto[h] = id;
    return false;
}


/*--------判断集--------*/
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
bool Jiang_object(int id, int x, int y, int dx, int dy){//判断将能否走到dx, dy
    if(abs(x - dx) + abs(y - dy) == 1){
        if(dy < 3 || dy > 5) return false;
        if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
        return true;
    }
    else return false;
}
bool Che_object(int id, int x, int y, int dx, int dy){//判断车能否走到dx, dy
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
bool Ma_object(int id, int x, int y, int dx, int dy){//判断马能否走到dx, dy
    return false;
}
bool Pao_object(int id, int x, int y, int dx, int dy){//判断炮能否跳到dx,dy。(dx,dy)不可以是空地
    if(d[id].At(dx, dy) == '_') return false;//!
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
bool Xiang_object(int id, int x, int y, int dx, int dy){//判断象能否走到dx, dy
    if(abs(x - dx) == 2 && abs(y - dy) == 2){
        if(acrossRiver(x, dx)) return false;
        int mx = (x + dx) >> 1, my = (y + dy) >> 1;
        if(d[id].At(mx, my) != '_') return false;
        return true;
    }
    else return false;
}
bool Shi_object(int id, int x, int y, int dx, int dy){//判断士能否走到dx, dy
    if(abs(x - dx) == 1 && abs(y - dy) == 1){
        if(dy < 3 || dy > 5) return false;
        if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) return false;
        return true;
    }
    else return false;
}
bool Bin_object(int x, int y){//判断兵能否走到dx, dy
    return false;
}
bool Che_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
    //判断车能否走到一个线段上(不含端点), 如果可以，求出dx,dy
    if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx = x1, dy = y;
    else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx = x, dy = y1;
    else return false;
    return Che_object(id, x, y, dx, dy);
}
bool Pao_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
    //判断炮能否走到一个线段上(不含端点), 如果可以，求出dx,dy
    if(x1 == x2 && y > min(y1, y2) && y < max(y1, y2)) dx = x1, dy = y;
    else if(y1 == y2 && x > min(x1, x2) && x < max(x1, x2)) dx = x, dy = y1;
    else return false;
    return Pao_object(id, x, y, dx, dy);
}
bool Xiang_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
    //判断象能否走到一个线段上(不含端点), 如果可以，求出dx,dy
    int kx[] = {2, 2, -2, -2};
    int ky[] = {-2, 2, -2, 2};
    for(int k = 0; k < 4; k++){
        dx = x + kx[k], dy = y + ky[k];
        if(!inChess(dx, dy) || acrossRiver(x, dx)) continue;
        if(OnSegment(dx, dy, x1, y1, x2, y2)) return true;
    }
    return false;
}
bool Shi_segment(int id, int x, int y, int x1, int y1, int x2, int y2, int &dx, int &dy){
    //判断士能否走到一个线段上(不含端点), 如果可以，求出dx,dy
    int kx[] = {1, 1, -1, -1};
    int ky[] = {-1, 1, -1, 1};
    for(int k = 0; k < 4; k++){
        dx = x + kx[k], dy = y + ky[k];
        if(dy < 3 || dy > 5) continue;
        if(dx < 0 || dx >= nrow || (dx >= 3 && dx < nrow - 3)) continue;
        if(OnSegment(dx, dy, x1, y1, x2, y2)) return true;
    }
    return false;
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
bool Fuck_Jiang(int id, bool red){//判断是否是一种将军局面
    //red为true表示红将黑，否则轮黑将红
    int Jx, Jy, jx, jy;
    Jiang_xy(id, Jx, Jy, 'J'), Jiang_xy(id, jx, jy, 'j');
    if(Jvsj(id, Jx, Jy, jx, jy)) return true;//将对将
    
    string str = red? "JCMPXSB" : "jcmpxsb";//攻击棋子
    if(!red) swap(Jx, jx), swap(Jy, jy);
    //printf("jx = %d jy = %d\n", jx, jy);
    //printf("Jx = %d Jy = %d\n", Jx, Jy);
    
    //车将
    for(int i = 0, t = 1; i < nrow; i++){
        if(d[id].s[i][jy] == str[t] && Che_object(id, i, jy, jx, jy)) return true;
    }
    for(int j = 0, t = 1; j < ncol; j++){
        if(d[id].s[jx][j] == str[t] && Che_object(id, jx, j, jx, jy)) return true;
    }
    //炮将
    for(int i = 0, t = 3; i < nrow; i++){
        if(d[id].s[i][jy] == str[t] && Pao_object(id, i, jy, jx, jy)) return true;
    }
    for(int j = 0, t = 3; j < ncol; j++){
        if(d[id].s[jx][j] == str[t] && Pao_object(id, jx, j, jx, jy)) return true;
    }
    //被马将..
    //被兵将..
    return false;
}
bool find_OnLine(int id, int x1, int y1, int x2, int y2, int n, int k, int &ansx, int &ansy){
    //判断(x1,y1)到(x2,y2)线段s上是否有n个点(不包括端点)
    //如果有，则求出从(x1,y1)开始走第k个点
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


int jx, jy, id;
/*--------处理出能红将黑的棋盘--------*/
void ok_Red(){
    if(Fuck_Jiang(nd, false)) nd--;
    else if(existed(nd)) nd--;
    else dtmp[++ndtmp] = nd;
}
void deal_C(int x, int y){//走红车
    //车转弯打
    if(Che_object(id, x, y, jx, y) && !isRed(id, jx, y)){//能走
        if(Che_object(id, jx, y, jx, jy)){//能将
            Newd(id, jx, y);
            d[nd].mv(x, y, '_');
            d[nd].mv(jx, y, 'C');
            ok_Red();
        }
    }
    if(Che_object(id, x, y, x, jy) && !isRed(id, x, jy)){//能走
        if(Che_object(id, x, jy, jx, jy)){//能将
            Newd(id, x, jy);
            d[nd].mv(x, y, '_');
            d[nd].mv(x, jy, 'C');
            ok_Red();
        }
    }
    //车直线吃子后打
    int mx = 0, my = 0;
    if(find_OnLine(id, x, y, jx, jy, 1, 1, mx, my)){
        if(isBlack(id, mx, my)){
            Newd(id, mx, my);
            d[nd].mv(x, y, '_');
            d[nd].mv(mx, my, 'C');
            ok_Red();
        }
    }
}
void deal_M(int x, int y){//走红马
}
void deal_P(int x, int y){//走红炮
    //炮拐弯打
    if((Che_object(id, x, y, jx, y)&&d[id].At(jx, y) == '_') || 
       (Pao_object(id, x, y, jx, y)&&isBlack(id, jx, y))){//能走
        if(Pao_object(id, jx, y, jx, jy)){//能将
            Newd(id, jx, y);
            d[nd].mv(x, y, '_');
            d[nd].mv(jx, y, 'P');
            ok_Red();
        }
    }
    if((Che_object(id, x, y, x, jy)&&d[id].At(x, jy) == '_') ||
       (Pao_object(id, x, y, x, jy)&&isBlack(id, x, jy))){//能走
        if(Pao_object(id, x, jy, jx, jy)){//能将
            Newd(id, x, jy);
            d[nd].mv(x, y, '_');
            d[nd].mv(x, jy, 'P');
            ok_Red();
        }
    }
    //炮直线吃子后打
    int mx = 0, my = 0;
    if(find_OnLine(id, x, y, jx, jy, 3, 2, mx, my)){
        if(isBlack(id, mx, my)){
            Newd(id, mx, my);
            d[nd].mv(x, y, '_');
            d[nd].mv(mx, my, 'P');
            ok_Red();
        }
    }
}
void deal_B(int x, int y){//对红兵处理
}
void deal_red(int x, int y){//处理红棋
    Jiang_xy(id, jx, jy, 'j');
    //printf("jx = %d jy = %d\n", jx, jy);
    char c = d[id].At(x, y);
    if(c == 'C') deal_C(x, y);
    else if(c == 'M') deal_M(x, y);
    else if(c == 'P') deal_P(x, y);
    else if(c == 'B') deal_B(x, y);
    else{//其他，移动后仍可将军
        //..相 士 帅
    }
}


/*--------红将黑后，处理出不被将的棋盘(暂时不考虑反将)--------*/
int fx, fy;
bool spjforBlack;
void ok_black(){
    if(Fuck_Jiang(nd, true)) nd--;
    else if(existed(nd)) nd--, spjforBlack = true;//!!!!!need changed
    else dtmp[++ndtmp] = nd;
}
void Move_j(){//移动老将
    int kx[4] = {-1, 1, 0, 0};
    int ky[4] = {0, 0, 1, -1};
    for(int k = 0; k < 4; k++){
        int zx = jx + kx[k], zy = jy + ky[k];
        if(zy < 3 || zy > 5) continue;
        if(zx < 0 || zx >= nrow || (zx >= 3 && zx < nrow - 3)) continue;
        if(isBlack(id, zx, zy)) continue;
        Newd(id, -1, -1);
        d[nd].mv(jx, jy, '_');
        d[nd].mv(zx, zy, 'j');
        ok_black();
    }
}
void Eat_fucker(int fx, int fy){//判断危险红棋子能否被吃掉
    string str = "jcmpxsb";
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(!isBlack(id, i, j)) continue;
            char c = d[id].At(i, j);
            int t = -1;
            if(c == 'j' && Jiang_object(id, i, j, fx, fy)) t = 0;
            else if(c == 'c' && Che_object(id, i, j, fx, fy)) t = 1;
            else if(c == 'p' && Pao_object(id, i, j, fx, fy)) t = 3;
            else if(c == 'x' && Xiang_object(id, i, j, fx, fy)) t = 4;
            else if(c == 's' && Shi_object(id, i, j, fx, fy)) t = 5;
            if(t == -1) continue;
            Newd(id, -1, -1);
            d[nd].mv(i, j, '_');
            d[nd].mv(fx, fy, str[t]);
            ok_black();
        }
    }
}
void Avoid_C(){//堵住红车
    string str = "jcmpxsb";
    for(int i = 0; i < nrow; i++){
        for(int j = 0; j < ncol; j++){
            if(!isBlack(id, i, j)) continue;
            char c = d[id].At(i, j);
            int t = -1, dx = 0, dy = 0;
            if(c == 'c' && Che_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 1;
            else if(c == 'p' && (Che_segment(id, i, j, fx, fy, jx, jy, dx, dy) || 
                                 Pao_segment(id, i, j, fx, fy, jx, jy, dx, dy))) t = 3;
            else if(c == 'x' && Xiang_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 4;
            else if(c == 's' && Shi_segment(id, i, j, fx, fy, jx, jy, dx, dy)) t = 5;
            if(isBlack(id, dx, dy)) continue;//!
            if(t == -1) continue;
            Newd(id, -1, -1);
            d[nd].mv(i, j, '_');
            d[nd].mv(dx, dy, str[t]);
            ok_black();
        }
    }
}
void Avoid_P(){//堵住红炮
    Avoid_C();//堵红炮约等于堵红车
}
void deal_black(){//处理黑棋
    spjforBlack = false;
    Jiang_xy(id, jx, jy, 'j');
    fx = d[id].fx, fy = d[id].fy;
    Eat_fucker(fx, fy);
    Move_j();
    char c = d[id].At(fx, fy);
    if(c == 'C') Avoid_C();
    else if(c == 'P') Avoid_P();
}


/*--------博弈搜索--------*/
bool dfs(int nowid){
    //printf("\nnowid = %d\n", nowid);
    //d[nowid].print();
    int fromn, ton;
    d[nowid].state = 1;
    if(d[nowid].red){//Turn red
        for(int i = 0; i < nrow; i++){//可优化
            for(int j = 0; j < ncol; j++){//可优化
                if(!isRed(nowid, i, j)) continue;
                fromn = nd + 1, id = nowid, deal_red(i, j), ton = nd;
                build_Edge(nowid, fromn, ton);
                //printf("nowid = %d i = %d j = %d fromn = %d ton = %d\n", nowid, i, j, fromn, ton);
                for(int k = fromn; k <= ton; k++){
                    if(!dfs(k)) return true;
                }
            }
        }
    }
    else{//Turn black
        fromn = nd + 1, id = nowid, deal_black(), ton = nd;
        build_Edge(nowid, fromn, ton);
        //printf("nowid = %d fromn = %d ton = %d\n", nowid, fromn, ton);
        for(int k = fromn; k <= ton; k++) if(!dfs(k)) return true;
        if(spjforBlack) return true;
    }
    d[nowid].state = 2;
    return false;
}


/*--------初始化--------*/
void init(){
    memset(d, 0, sizeof(d));
    Newd(0, -1, -1);
    ndtmp = 0, dtmp[++ndtmp] = 1;
    hashto.clear();
    existed(1);
    init_Edge();
}


string order;
int main(){
    init();
    input();
    if(dfs(1)){
        d[32].print();
        printf("sta[32] = %d\n", d[32].state);
        printf("Successful!\n");
        printf("sta[1] = %d\n", d[1].state);
        while(true){
            printf("Type 'y' to begin\n");
            cin >> order;
            if(order == "y"){
                work(1);
                break;
            }
        }
    }
    else printf("No solution!\n");
    return 0;
}


/*--------调控指令--------*/
void work(int nowid){
    printf("\nnowid = %d\n", nowid);
    d[nowid].print();
    int toid = -1, eid = -1;
    if(d[nowid].red){
        for(int i = head[nowid]; i != -1 && toid == -1; i = edge[i].next){
            int to = edge[i].to;
            if(d[to].state == 2) eid = i, toid = to;
        }
        if(toid == -1){printf("Unsuccessful to go!\n"); return;}
        printf("You can go like that: ");
        printf("(%d %d) -> ", edge[eid].x1, edge[eid].y1);
        printf("(%d %d)\n", edge[eid].x2, edge[eid].y2);
        work(toid);
    }
    else{
        printf("Type black's movement\n");
        int x1, y1, x2, y2;
        printf("(x1 y1) -> (x2 y2)\n");
        cin >> x1 >> y1 >> x2 >> y2;
        printf("(%d %d) -> (%d %d)\n", x1, y1, x2, y2);
        for(int i = head[nowid]; i != -1 && toid == -1; i = edge[i].next){
            int to = edge[i].to;
            if(edge[i].x1 == x1 && edge[i].y1 == y1){
                if(edge[i].x2 == x2 && edge[i].y2 == y2) toid = to;
            }
        }
        if(toid == -1){printf("Wrong input!\n"); work(nowid);}
        else work(toid);
    }
    return;//work done
}
