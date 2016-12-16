#include "search.h"

Search::Search()
{
	this->max_depth = 25; 	// 默认值 25
	this->howMove.clear();
	this->hash_set.clear();
}

Search::Search(const int max_depth_val)
{
	this->max_depth = max_depth_val;
	this->howMove.clear();
	this->hash_set.clear();
}

int Search::search(ChessBoard &x)
{
	return dfs(x, 1);
}

int Search::dfs(ChessBoard &x, const int depth)
{
	if ( depth > this->max_depth ) return x.turnBlack();
	if ( this->hash_set.find( x.hash_val ) != this->hash_set.end() )	 return x.turnBlack();
	this->hash_set.insert( x.hash_val );
	generate_move(x);
	for (int i = 0; i < x.move.size(); i ++)
	{
		ChessBoard y = ChessBoard(x, x.move[i]);
		if ( dfs(y, depth + 1) == LOSE )
		{
			this->howMove[ x.hash_val ] = x.move[i];
			return WIN;
		}
	}
	this->hash_set.erase( x.hash_val );
	return LOSE;
}

void Search::generate_move(ChessBoard &x) const
{
	// wait to write
}