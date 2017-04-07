#include "OrderDeal.h"
#include "ShellWork.h"

OrderDeal OrDeal;

//setoption newgame: 开始新一轮挑战
void OrderDeal::NewGame(){
	first = true;
}

//position fen ..: 读入棋盘，并在第一次读入后进行搜索
void OrderDeal::PositionFen(){
	d[0] = ChessBoard();
    FB.FenToBoard(Command.Position.FenStr, d[0].s);
    FB.MoveBoard(Command.Position.MoveStr, 
                 Command.Position.MoveNum, d[0].s);
    //for(int i = 0; i < nrow; i++) printf("%s\n", d[0].s[i]);
    First();
}

//第一次读入棋盘后，进行的操作
void OrderDeal::First(){
	if(!first) return;
	first = false;
    ThisSearch.WorkWithAB(stopFr, stopTo);//搜出博弈图
    ReverseTopo(in);//逆向拓扑求博弈态
    spfa_GamePath(f, in, Ansto);//求出每个必胜红点的最佳走向点
}

void OrderDeal::GoPrint(){ //go / go ponder: 进入思考状态，最后返回走法
    int id = Hash.find(0);
    if(id == 0) printf("nobestmove\n");
    else{
    	int e = -1;
    	for(int i = h1[id]; i != -1 && e == -1; i = e1[i].next){
    		if(e1[i].to == Ansto[id]) e = i;
    	}
    	if(e == -1) printf("nobestmove\n");
    	else{
    		char ans[10];
    		int x1, y1, x2, y2;
    		getMoveXY(e1[e], x1, y1, x2, y2);
    		FB.getMoveStr(ans, x1, y1, x2, y2);
    		printf("bestmove %.4s\n", ans);
    	}
    }
    fflush(stdout);
}


void OrderDeal::print(string s){
	cout << s << endl;
	fflush(stdout);
}

void OrderDeal::ReadyOK(){
	print("readyok");
}

void OrderDeal::Bye(){
	print("bye");
}

void OrderDeal::PreparePrint(){
	// 显示引擎的名称、版本、作者和使用者
	print("id name lazyboy");
    print("id version 0.1");
	print("id author peterpan");
    
	//设置参数
	print("option batch type check default false");
	print("option debug type check default false");
	print("option bookfiles type string default ");
	print("option egtbpaths type string default null");
	print("option hashsize type spin default 0 MB");
	print("option threads type spin default 0");
	print("option drawmoves type spin default 0");
	print("option pruning type check 0");
	print("option knowledge type check 0");
	print("option selectivity type spin min 0 max 3 default 0");
	print("option style type combo var solid var normal var risky default normal"); 
	print("copyprotection ok");
	print("ucciok\n");

	return;//Prepare Print done
}
