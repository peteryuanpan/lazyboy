#ifndef SEARCH_H
#define SEARCH_H
#include "chessBoard.h"
#include <map>
#include <set>
using namespace std;

class Search
{
public:
	Search();
	Search(const int max_depth_val);
	int search(ChessBoard &x);
private:
	map<long long, int> howMove; // 红方必胜局面的着法, long long: hash值, int: 着法
	set<long long> hash_set; // 根节点到当前节点路径上的局面的hash值集合
	int max_depth; // 最大深度限制
	int dfs(ChessBoard &x, const int depth);
	void generate_move(ChessBoard &x) const; 
};

const int WIN = 1, LOSE = -1, TIE = 0;

#endif