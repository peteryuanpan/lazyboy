#include "HeadFile.h"
#include "MainSearch.h"
//#include "ShellOrder.cpp"

const char adress[] = {"/Users/peteryuanpan/Chess/XQWS/198-神龙跃海.txt"};
void input(){//输入数据
    memset(d, 0, sizeof(d));
    FILE *F = fopen(adress, "r");
    char s[100];
    for(int i = 0; i < nrow; i++){
        fgets(s, ncol + 10, F);
        s[glen(s) - 1] = '\0';
        strcpy(d[0].s[i], s);
    }
    fclose(F);
    return;//input done;
}

int main(){
    input();
    MainSearch ThisSearch;
    ThisSearch.work(25);
    
    printf("nd = %d\n", nd);
    if(d[1].sta == 1) printf("Must Win!\n");
    else if(d[1].sta == 2) printf("Must Lose!\n");
    else if(d[1].sta == 4) printf("Tie!\n");
    return 0;
}
