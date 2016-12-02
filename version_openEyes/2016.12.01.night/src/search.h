#ifndef SEARCH_H
#define SEARCH_H
#include "chessBoard.h"
#include <map>
using namespace std;

class Search
{
public:
	Search();
public:
	// 记录红方必胜局面的落子法，long long: hash值，int: 着法
	map<long long, int> howMove;
	// 主dfs函数，参数：局面，深度
	void dfs(ChessBoard &x, int depth);
};

#endif