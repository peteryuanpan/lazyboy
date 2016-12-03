#ifndef GENMOVE_H
#define GENMOVE_H
#include "chessBoard.h"

class GenMove
{
public:
	GenMove();
public:
	void gen_attack(ChessBoard &x) const;
	void gen_defense(ChessBoard &x) const;
};

#endif