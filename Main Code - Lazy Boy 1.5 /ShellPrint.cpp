/*--------调控指令集--------*/
#include "HeadFile.h"

string order;
void print(int id);

void printButs(int id);

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

int TypeMovement(int nowid, bool red){
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
        return TypeMovement(nowid, red);
    }
    else{
        printf("%s goes like that: ", red? "Red" : "Black");
        printf("(%d, %d) -> (%d, %d)\n", x1, y1, x2, y2);
        printTwo(nowid, toid);
        return Type_y_go_next(1) == -1? TypeMovement(nowid, red) : toid;
    }
}

void Debug(int nowid){
    int toid;
    if(d[nowid].red){
        printf("[Turn red to go]\n");
        print(nowid);
        toid = TypeMovement(nowid, true);
        printTwo(nowid, toid);
        printf("\n\n\n"), Debug(toid);
    }
    else{
        printf("[Turn black to go]\n");
        print(nowid);
        toid = TypeMovement(nowid, false);
        printf("\n\n\n"), Debug(toid);
    }
    return;//debug done
}

void TypeBestMove(int nowid, int last){
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
        printf("\n\n\n"), TypeBestMove(toid, e);
    }
    else{
        printf("[Turn black to go]\n");
        print(nowid);
        toid = TypeMovement(nowid, false);
        printf("\n\n\n"), TypeBestMove(toid, last);
    }
    return;//debug done
}
