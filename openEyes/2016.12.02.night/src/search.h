#ifndef SEARCH_H
#define SEARCH_H
#include "chessBoard.h"
#include "genMove.h"
#include <map>
#include <set>
using namespace std;

class Search
{
public:
	Search();
	Search(const int want_depth);
public:
	map<long long, int> howMove;	// 红方必胜局面的着法, long long: hash值, int: 着法
	void set_max_depth(const int want_depth);
	int search(ChessBoard &x);
private:
	int max_depth;					// 最大深度限制
	set<long long> hash_set;			// 访问过的局面的hash值
	int dfs(ChessBoard &x, const int depth);	// 主搜索函数
	void generate_move(ChessBoard &x) const;	// 生成着法
	bool Found(const ChessBoard &x) const;	// 局面x在hash_set中
	void init_hash_set(const ChessBoard &x);
	void insert_into_howMove(const ChessBoard &x, const int move_val);
};

const int WIN = 1, LOSE = -1, TIE = 0;

#endif