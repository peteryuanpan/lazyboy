#include <string>
#include <iostream>
#include "Ucci.h"

int n;
const char *BoolValue[2] = { "false", "true" };
const char *ChessStyle[3] = { "solid", "normal", "risky" };
char *BackSlashPtr;
char BookFile[1024];
CommEnum order;
CommDetail Command;
int ThisMove;
void PreparePrint();

int main(){
	printf("Please Type UCCI order:\n");
    
	// 引擎接收"ucci"指令
	if(BootLine() == e_CommUcci){
		//引导状态
		PreparePrint();

		//空闲状态+思考状态
		while(true){
			order = IdleLine(Command, 0);
			if(order == e_CommQuit) break;//quit
			if(order == e_CommIsReady){//isready
				printf("readyok\n");
				fflush(stdout);
			}
			else if(order == e_CommStop){//stop
				//return 0;
			}
            //position fen ..
			else if(order == e_CommPosition){
				
			}
            //go / go ponder
			else if(order == e_CommGo || order == e_CommGoPonder){
				
			}
			//else return 0;
		}
        
		printf("bye\n");
		fflush(stdout);
	}
	return 0;
}

//引导状态
void PreparePrint(){
	// 显示引擎的名称、版本、作者和使用者
	printf("\n");
	printf("id name Lazy Boy\n");
	fflush(stdout);
	printf("id copyright Open Source\n");
	fflush(stdout);
	printf("id author peterpan\n");
	fflush(stdout);
	printf("id user Lazier Boy\n\n");
	fflush(stdout);
        
	// 显示引擎ucci指令的反馈信息，表示引擎所支持的选项
	// option batch %d
	printf("option batch type check default %s\n", BoolValue[0]);
	fflush(stdout);
        
	// option debug 让引擎输出详细的搜索信息，并非真正的调试模式。
	printf("option debug type check default %s\n", BoolValue[0]);
	fflush(stdout);
        
	// 指定开局库文件的名称，可指定多个开局库文件，用分号“;”隔开，如不让引擎使用开局库，可以把值设成空
	printf("option bookfiles type string default %s\n", BookFile);
	fflush(stdout);
    
	// 残局库名称
	printf("option egtbpaths type string default null\n");
	fflush(stdout);
    
	// 显示Hash表的大小
	printf("option hashsize type spin default %d MB\n", 0);
	fflush(stdout);
    
	// 引擎的线程数
	printf("option threads type spin default %d\n", 0);
	fflush(stdout);
        
	// 引擎达到自然限着的半回合数
	printf("option drawmoves type spin default %d\n", 0);
	fflush(stdout);
        
	// 棋规
	printf("option repetition type spin default %d 1999 Chinese Chess Rules\n", e_RepetitionChineseRule);
	fflush(stdout);
        
	// 空着裁减是否打开
	printf("option pruning type check %d\n", 0);
	fflush(stdout);
        
	// 估值函数的使用情况
	printf("option knowledge type check %d\n", 0);
	fflush(stdout);
        
	// 指定选择性系数，通常有0,1,2,3四个级别。给估值函数加减一定范围内的随机数，让引擎每次走出不相同的棋。
	printf("option selectivity type spin min 0 max 3 default %d\n", 0);
	fflush(stdout);
        
	// 指定下棋的风格，通常有solid(保守)、normal(均衡)和risky(冒进)三种
	printf("option style type combo var solid var normal var risky default %s\n", ChessStyle[1]);
	fflush(stdout);		
        
	// copyprotection 显示版权检查信息(正在检查，版权信息正确或版权信息错误)。 
	printf("copyprotection ok\n\n");
	fflush(stdout);
        
	// ucciok 这是ucci指令的最后一条反馈信息，表示引擎已经进入用UCCI协议通讯的状态。
	printf("ucciok\n\n");
	fflush(stdout);
}
