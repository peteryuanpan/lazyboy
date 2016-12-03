#include "search.h"

Search::Search()
{
	this->max_depth = 25; 	// 默认值 25
	this->howMove.clear();
	this->hash_set.clear();
}

Search::Search(const int want_depth)
{
	this->max_depth = want_depth;
	this->howMove.clear();
	this->hash_set.clear();
}

void Search::set_max_depth(const int want_depth)
{
	this->max_depth = want_depth;
}

int Search::search(ChessBoard &x)
{
	return dfs(x, 1);
}

int Search::dfs(ChessBoard &x, const int depth)
{
	if ( depth > this->max_depth ) return x.turnBlack();
	if ( Found(x) ) return x.turnBlack();
	init_hash_set(x);
	generate_move(x);
	for (int i = 0; i < x.move.size(); i ++)
	{
		ChessBoard y = ChessBoard(x, x.move[i]);
		if ( dfs(y, depth + 1) == LOSE )
		{
			insert_into_howMove(x, x.move[i]);
			return WIN;
		}
	}
	return LOSE;
}

void Search::generate_move(ChessBoard &x) const
{
	GenMove y;
	if ( x.turnRed() ) y.gen_move_check(x); 
	else y.gen_move_checked(x);
}

bool Search::Found(const ChessBoard &x) const
{
	return this->hash_set.find( x.hash_val ) != this->hash_set.end();
}

void Search::init_hash_set(const ChessBoard &x)
{
	this->hash_set.insert( x.hash_val );
}

void Search::insert_into_howMove(const ChessBoard &x, const int move_val)
{
	this->howMove[ x.hash_val ] = move_val;
}