#include "evaluate.h"
#include "search.h"
#include "position.h"
#include "move.h"

int vlPiece[2][7][256];

// 预评估，给 vlRed 及 vlBlk 赋值
void PositionStruct::PreEvaluate ( void ) {
	// 1. 计算局面阶段分值
	int val = 0;
	for ( int sd = 0; sd < 2; sd ++ ) {
		const int ST = (sd == 0) ? RED_TYPE : BLACK_TYPE;
		// 计算马的分值
		for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
			if ( piece[i+ST] ) {
				val += KNIGHT_CANNON_GAME_VALUE;
			}
		}
		// 计算车的分值
		for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
			if ( piece[i+ST] ) {
				val += ROOK_GAME_VALUE;
			}
		}
		// 计算炮的分值
		for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
			if ( piece[i+ST] ) {
				val += KNIGHT_CANNON_GAME_VALUE;
			}
		}
		// 计算其他子的分值，不计算将的
		for ( int i = ADVISOR_FROM; i <= BISHOP_TO; i ++ ) {
			if ( piece[i+ST] ) {
				val += OTHER_GAME_VALUE;
			}
		}
		for ( int i = PAWN_FROM; i <= PAWN_TO; i ++ ) {
			if ( piece[i+ST] ) {
				val += OTHER_GAME_VALUE;
			}
		}
	}
	const int MIDGAME_VALUE = ( 2 * TOTAL_GAME_VALUE - val ) * val / TOTAL_GAME_VALUE; // 使用二次函数，子力很少时才认为接近残局
	const int ENDGAME_VALUE = TOTAL_GAME_VALUE - MIDGAME_VALUE;

	// 2. 计算攻击/被攻击状态分值
	int redAttack = 0;
	int blkAttack = 0;
	// 计算马和车的过河分值
	for ( int i = KNIGHT_FROM; i <= ROOK_TO; i ++ ) {
		if ( piece[i+RED_TYPE] && IN_OPP_SIDE_BOARD(i+RED_TYPE, piece[i+RED_TYPE]) ) {
			redAttack += 2;
		}
		if ( piece[i+BLACK_TYPE] && IN_OPP_SIDE_BOARD(i+BLACK_TYPE, piece[i+BLACK_TYPE]) ) {
			blkAttack += 2;
		}
	}
	// 计算炮和兵的过河分值
	for ( int i = CANNON_FROM; i <= PAWN_TO; i ++ ) {
		if ( piece[i+RED_TYPE] && IN_OPP_SIDE_BOARD(i+RED_TYPE, piece[i+RED_TYPE]) ) {
			redAttack += 1;
		}
		if ( piece[i+BLACK_TYPE] && IN_OPP_SIDE_BOARD(i+BLACK_TYPE, piece[i+BLACK_TYPE]) ) {
			blkAttack += 1;
		}
	}
	// 计算轻子数差
	int d = 0;
	for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
		d += (piece[i+RED_TYPE] > 0) - (piece[i+BLACK_TYPE] > 0);
	}
	for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
		d += (piece[i+RED_TYPE] > 0) - (piece[i+BLACK_TYPE] > 0);
		d += (piece[i+RED_TYPE] > 0) - (piece[i+BLACK_TYPE] > 0);
	}
	for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
		d += (piece[i+RED_TYPE] > 0) - (piece[i+BLACK_TYPE] > 0);
	}
	// 再更新攻击状态分值
	if ( d > 0 ) {
		redAttack += abs(d) * 2;
	}
	if ( d < 0 ) {
		blkAttack += abs(d) * 2;
	}
	redAttack = MIN ( redAttack, TOTAL_ATTACK_VALUE ); // 最多不超过8
	blkAttack = MIN ( blkAttack, TOTAL_ATTACK_VALUE );

	// 3. 计算子力平衡分值
	for ( int sd = 0; sd < 2; sd ++ ) {
		const int ATTACKING_VALUE = (sd == 0) ? redAttack : blkAttack; // 本方进攻值
		const int ATTACKLESS_VALUE = TOTAL_ATTACK_VALUE - ATTACKING_VALUE; // 本方非进攻值
		const int THREATENED_VALUE = (sd == 0) ? blkAttack : redAttack; // 本方被进攻值
		const int THREATLESS_VALUE = TOTAL_ATTACK_VALUE - THREATENED_VALUE; // 本方非被进攻值

		for ( int p = 0; p < 256; p ++ ) {
			if ( IN_BOARD(p) ) {
				// 将、兵
				const int t1 = ( MIDGAME_VALUE * MIDGAME_ATTACKING_KING_PAWN[p] + ENDGAME_VALUE * ENDGAME_ATTACKING_KING_PAWN[p] ) / TOTAL_GAME_VALUE;
				const int t2 = ( MIDGAME_VALUE * MIDGAME_ATTACKLESS_KING_PAWN[p] + ENDGAME_VALUE * ENDGAME_ATTACKLESS_KING_PAWN[p] ) / TOTAL_GAME_VALUE;
				vlPiece[sd][KING_TYPE][p] = ( t1 * ATTACKING_VALUE + t2 * ATTACKLESS_VALUE ) / TOTAL_ATTACK_VALUE;
				vlPiece[sd][PAWN_TYPE][p] = vlPiece[sd][KING_TYPE][p];
				// 士、象
				vlPiece[sd][ADVISOR_TYPE][p] = ( THREATENED_VALUE * THREATENED_ADVISOR_BISHOP[p] + THREATLESS_VALUE * THREATLESS_ADVISOR_BISHOP[p]) / TOTAL_ATTACK_VALUE;
				vlPiece[sd][BISHOP_TYPE][p] = vlPiece[sd][ADVISOR_TYPE][p];
				// 马
				vlPiece[sd][KNIGHT_TYPE][p] = ( MIDGAME_VALUE * MIDGAME_KNIGHT[p] + ENDGAME_VALUE * ENDGAME_KNIGHT[p]) / TOTAL_GAME_VALUE;
				// 车
				vlPiece[sd][ROOK_TYPE][p] = ( MIDGAME_VALUE * MIDGAME_ROOK[p] + ENDGAME_VALUE * ENDGAME_ROOK[p]) / TOTAL_GAME_VALUE;
				// 炮
				vlPiece[sd][CANNON_TYPE][p] = ( MIDGAME_VALUE * MIDGAME_CANNON[p] + ENDGAME_VALUE * ENDGAME_CANNON[p]) / TOTAL_GAME_VALUE;
			}
		}
	}

	// 4. 计算红黑的子力值
	// 调整不受威胁方少掉的士、象分值
	this->vlRed = ADVISOR_BISHOP_ATTACKLESS_VALUE * ( TOTAL_ATTACK_VALUE - blkAttack ) / TOTAL_ATTACK_VALUE;
	this->vlBlk = ADVISOR_BISHOP_ATTACKLESS_VALUE * ( TOTAL_ATTACK_VALUE - redAttack ) / TOTAL_ATTACK_VALUE;
	// 再计算
	for ( int i = 16; i < 32; i ++ ) {
		if ( piece[i] ) {
			this->vlRed += vlPiece[0][PIECE_TYPE(i)][piece[i]];
		}
	}
	for ( int i = 32; i < 48; i ++ ) {
		if ( piece[i] ) {
			this->vlBlk += vlPiece[1][PIECE_TYPE(i)][piece[i]];
		}
	}
}

// 打分，车的灵活性
int PositionStruct::RookMobility ( void ) const {
	int value[2];
	for ( int sd = 0; sd < 2; sd ++ ) {
		value[sd] = 0;
		int ST = SIDE_TYPE (sd);
		for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
			if ( piece[i+ST] ) {
				int r = ROW (piece[i+ST]);
				int c = COL (piece[i+ST]);
				int p;
				p = LOWER_P[ bitCol[c] ][r][0];  value[sd] += ( p == 0 ) ? r - 3 : p - 1;
				p = HIGHER_P[ bitCol[c] ][r][0]; value[sd] += ( p == 0 ) ? 12 - r : p - 1;
				p = LOWER_P[ bitRow[r] ][c][0];  value[sd] += ( p == 0 ) ? c - 3 : p - 1;
				p = HIGHER_P[ bitRow[r] ][c][0]; value[sd] += ( p == 0 ) ? 11 - c : p - 1;
			}
		}
	}
	return SIDE_VALUE ( this->player, value[0] - value[1] ) >> 1; // 取一半
}

// 打分，马的阻碍
int PositionStruct::KnightTrap ( void ) const {
	int value[2];
	for ( int sd = 0; sd < 2; sd ++ ) {
		value[sd] = 0;
		int ST = SIDE_TYPE (sd);
		for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
			if ( piece[i+ST] ) {
				int nMovable = 0;
				int k = 0;
				while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
					int hit = KNIGHT_HIT[ piece[i+ST] ][k];
					int pin = KNIGHT_PIN[ piece[i+ST] ][k];
					if ( square[hit] == 0 && square[pin] == 0 && !Protected(1-sd, hit) ) {
						if ( KNIGHT_TRAP[hit] == 0 ) {
							nMovable ++;
							if ( nMovable > 1 ) {
								break;
							}
						}
					}
					k ++;
				}
				if ( nMovable == 0 ) { // 罚10分
					value[sd] -= 10;
				}
				else if ( nMovable == 1 ) { // 罚5分
					value[sd] -= 5;
				}
			}
		}
	}
	return SIDE_VALUE ( this->player, value[0] - value[1] );
}

// 给局面打分
int PositionStruct::Evaluate ( const int alpha, const int beta ) const {
	int value = 0;
	// 1. 子力平衡评估
	value += Material ();
	// 2. 车的灵活性评估
	value += RookMobility ();
	// 3. 马的阻碍评估
	value += KnightTrap ();
	return value;
}