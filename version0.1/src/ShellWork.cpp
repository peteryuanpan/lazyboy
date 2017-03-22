#include "ShellWork.h"

ShellWork Shell;

//读取Shell下的命令
string ShellWork::getord(){
    string s;
    getline(cin, s);
    if(s == "quit") quit = 1;
    return s;
}

//将Number转为字符串
string ShellWork::to_string(int id){
    string ans = "";
    while(id) ans += id % 10 + '0', id /= 10;
    reverse(ans.begin(), ans.end());
    return ans;
}

//判断字符串是否是个Number
bool ShellWork::isNumber(string s){
    for(int i = 0; i < slen(s); i++) if(!isdigit(s[i])) return false;
    return true;
}

//将字符串转为Number
int ShellWork::StrToNumber(string s){
    int ans = 0, ten = 1;
    for(int i = slen(s) - 1; i >= 0; i--){
        ans += (s[i] - '0') * ten;
        ten *= 10;
    }
    return ans;
}

//判断p是否是s的前缀
bool ShellWork::is_prefix(string p, string s){
    int i = 0;
    for(; i < slen(p) && p[i] == s[i]; i++);
    return i == slen(p);
}

//输出一个局面的所有信息
void ShellWork::print(int id){
    for(int i = 0; i < nrow; i++) printf("%s\n", d[id].s[i]);
    printButs(id);
}

//输出一个局面的所有信息但不输出棋盘
void ShellWork::printButs(int id){
    printf("id = %5d  sta = %d  Ansto = %d  fv = %d  red = %d  ", id, d[id].sta, OrDeal.Ansto[id], OrDeal.f[id], d[id].red);
    printf("fx = %d  fy = %d  step = %d\n", d[id].fx, d[id].fy, d[id].step);
    int x1, y1, x2, y2;
    for(int i = h1[id]; i != -1; i = e1[i].next){
        int t = e1[i].to;
        getMoveXY(e1[i], x1, y1, x2, y2);
        printf("to = %5d  sta = %d  (%d, %d) -> (%d, %d)\n", t, d[t].sta, x1, y1, x2, y2);
    }
    
    for(int i = h2[id]; i != -1; i = e2[i].next){
        int t = e2[i].to;
        getMoveXY(e2[i], x1, y1, x2, y2);
        printf("fa = %5d  sta = %d  (%d, %d) <- (%d, %d)\n", t, d[t].sta, x1, y1, x2, y2);
    }
    printf("\n");
}

//输出棋盘a->棋盘b
void ShellWork::printTwo(int id, int to){
    printf("Chessbord goes like that:\n");
    for(int i = 0; i < nrow; i++){
        printf("%s", d[id].s[i]);
        if(i == 4) printf("  --->  ");
        else printf("        ");
        printf("%s\n", d[to].s[i]);
    }
    printf("id = %d", id);
    int t = 8;
    for(int ten = 10; id >= ten; ten *= 10) t--;
    while(t--) printf(" ");
    printf("to = %d\n", to);
}

//暂停处理器
int ShellWork::Type_y_go_next(){
    printf("Type 'y' to go next or an Order.\n");
    ord = getord(); if(quit) return -99999;
    if(ord == "y") return 1;
    else if(ord == "n") return -1;
    else if(is_prefix("id ", ord)){
        ord.erase(0, 3);
        if(isNumber(ord)){
            int x = StrToNumber(ord);
            if(x >= 1 && x <= nd){
                printf("\nCheck ChessBoard ");
                cout << to_string(x) + ":"<< endl;
                print(x);
            }
        }
    }
    return Type_y_go_next();
}

//输出棋盘走法
int ShellWork::TypeMovement(int id, bool red){
    int cx1, cy1, cx2, cy2, to = -1;
    printf("Type %s's movement\n", red? "red" : "black");
    printf("(x1, y1) -> (x2, y2)\n");
    ord = getord(); if(quit) return -99999;
    int tem[4];
    for(int i = 0; i < 4; i++) tem[i] = -1;
    if(is_prefix("go ", ord)){
        ord.erase(0, 3), ord += " ";
        for(int i = 0, t; i < 4; i++){
            for(t = 0; t < slen(ord) && isdigit(ord[t]); t++);
            tem[i] = StrToNumber(ord.substr(0, t));
            ord.erase(0, t + 1);
        }
    }
    cx1 = tem[0], cy1 = tem[1], cx2 = tem[2], cy2 = tem[3];
    
    for(int i = h1[id]; i != -1 && to == -1; i = e1[i].next){
        int t = e1[i].to, x1, y1, x2, y2;
        getMoveXY(e1[i], x1, y1, x2, y2);
        if(x1 == cx1 && y1 == cy1){
            if(x2 == cx2 && y2 == cy2) to = t;
        }
    }
    
    if(to == -1){
        printf("(%d, %d) -> (%d, %d)\n", cx1, cy1, cx2, cy2);
        printf("Wrong input!\n");
        return TypeMovement(id, red);
    }
    else{
        printf("%s goes like that: ", red? "Red" : "Black");
        printf("(%d, %d) -> (%d, %d)\n", cx1, cy1, cx2, cy2);
        printTwo(id, to);
        return Type_y_go_next() == -1? TypeMovement(id, red) : to;
    }
}

//debug函数
void ShellWork::Debug(int id){
    quit = 0;
    printf("[Turn %s to go]\n", d[id].red? "red" : "black");
    print(id);
    int to = TypeMovement(id, d[id].red);
    if(quit) return;
    printf("\n\n\n"), Debug(to);
    return;//debug done
}

//input函数
const char adress[] = {"../XQWSData/QWXQ/024.txt"};
void ShellWork::input(){
    FILE *F = fopen(adress, "r");
    d[0] = ChessBoard();
    for(int i = 0; i < nrow; i++){
        char s[100];
        fgets(s, ncol + 10, F);
        s[glen(s) - 1] = '\0';
        strcpy(d[0].s[i], s);
        printf("%s i = %d\n", d[0].s[i], i);
    }
    printf("\n");
    fclose(F);
    return;//input done;
}

//  将棋盘转为fen串
//  char fen[100];
//  FB.BoardtFen(fen, d[0].s);
//  printf("position fen %s\n", fen);
//  return;

//  ReverseTopo(OrDeal.in);//逆向拓扑求博弈态
//  spfa_GamePath(OrDeal.f, OrDeal.in, OrDeal.Ansto);//求出每个必胜红点的最佳走向点

//主执行函数
void ShellWork::work(){
    input();
    ThisSearch.WorkWithAB(stopFr, stopTo);
    ReverseTopo(OrDeal.in);//逆向拓扑求博弈态
    spfa_GamePath(OrDeal.f, OrDeal.in, OrDeal.Ansto);//求出每个必胜红点的最佳走向点
    
    printf("nd = %d\n", nd);
    if(d[1].sta == 1) printf("Must Win!\n");
    else if(d[1].sta == 2) printf("Must Lose!\n");
    else if(d[1].sta == 4) printf("Tie!\n");
    printf("\n");
    
    Debug(1);
    return;//work done
}
