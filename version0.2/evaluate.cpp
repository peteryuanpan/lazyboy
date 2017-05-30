#include "evaluate.h"
#include "search.h"
#include "position.h"
#include "move.h"

EvaluateStruct Evl;
int vlPiece[2][7][256];

// 子力平衡评估
int PositionStruct::Material ( void ) const {
	return SIDE_VALUE ( player, vlRed - vlBlk ) + Evl.vlAdvanced;
}

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

	// 计算vlAdvanced、vlHollowThreat、vlCentralThreat
	Evl.vlAdvanced = ( TOTAL_ADVANCED_VALUE * MIDGAME_VALUE + TOTAL_ADVANCED_VALUE / 2 ) / TOTAL_GAME_VALUE;
	//printf("vlAdvanced = %d\n", Evl.vlAdvanced);
	for ( int i = 0; i < 16; i ++ ) {
		Evl.vlHollowThreat[i] = cvlHollowThreat[i] * ( MIDGAME_VALUE + TOTAL_GAME_VALUE ) / ( TOTAL_GAME_VALUE * 2 );
		Evl.vlCentralThreat[i] = cvlCentralThreat[i];
		//printf("i = %d vlHollowThreat = %d CentralThreat = %d\n", i, Evl.vlHollowThreat[i], Evl.vlCentralThreat[i]);
	}

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
				const int w = ( sd == 0 ) ? p : REVERSE_POS ( p );
				// 将
				vlPiece[sd][KING_TYPE][w] = ( MIDGAME_VALUE * MIDGAME_ATTACKING_KING_PAWN[p] + ENDGAME_VALUE * ENDGAME_ATTACKING_KING_PAWN[p] ) / TOTAL_GAME_VALUE;
				// 兵
				const int t1 = vlPiece[sd][KING_TYPE][w];
				const int t2 = ( MIDGAME_VALUE * MIDGAME_ATTACKLESS_KING_PAWN[p] + ENDGAME_VALUE * ENDGAME_ATTACKLESS_KING_PAWN[p] ) / TOTAL_GAME_VALUE;
				vlPiece[sd][PAWN_TYPE][w] = ( t1 * ATTACKING_VALUE + t2 * ATTACKLESS_VALUE ) / TOTAL_ATTACK_VALUE;
				// 士、象
				vlPiece[sd][ADVISOR_TYPE][w] = ( THREATENED_VALUE * THREATENED_ADVISOR_BISHOP[p] + THREATLESS_VALUE * THREATLESS_ADVISOR_BISHOP[p]) / TOTAL_ATTACK_VALUE;
				vlPiece[sd][BISHOP_TYPE][w] = vlPiece[sd][ADVISOR_TYPE][w];
				// 马
				vlPiece[sd][KNIGHT_TYPE][w] = ( MIDGAME_VALUE * MIDGAME_KNIGHT[p] + ENDGAME_VALUE * ENDGAME_KNIGHT[p]) / TOTAL_GAME_VALUE;
				// 车
				vlPiece[sd][ROOK_TYPE][w] = ( MIDGAME_VALUE * MIDGAME_ROOK[p] + ENDGAME_VALUE * ENDGAME_ROOK[p]) / TOTAL_GAME_VALUE;
				// 炮
				vlPiece[sd][CANNON_TYPE][w] = ( MIDGAME_VALUE * MIDGAME_CANNON[p] + ENDGAME_VALUE * ENDGAME_CANNON[p]) / TOTAL_GAME_VALUE;
			}
		}
	}

	// 计算vlRedAdvisorLeakage、vlBlkAdvisorLeakage
	Evl.vlRedAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * blkAttack / TOTAL_ATTACK_VALUE;
	Evl.vlBlkAdvisorLeakage = TOTAL_ADVISOR_LEAKAGE * redAttack / TOTAL_ATTACK_VALUE;
	//printf("vlRedAdvisorLeakage = %d vlBlkAdvisorLeakage = %d\n", Evl.vlRedAdvisorLeakage, Evl.vlBlkAdvisorLeakage);

	// 计算vlRedBottomThreat、vlBlkBottomThreat
	for ( int i = 0; i < 16; i ++ ) {
		Evl.vlRedBottomThreat[i] = cvlBottomThreat[i] * blkAttack / TOTAL_ATTACK_VALUE;
		Evl.vlBlkBottomThreat[i] = cvlBottomThreat[i] * redAttack / TOTAL_ATTACK_VALUE;
		//printf("i = %d vlRedBottomThreat = %d vlBlkBottomThreat = %d\n", i, Evl.vlRedBottomThreat[i], Evl.vlBlkBottomThreat[i]);
	}

	// 4. 计算红黑的子力值
	// 调整不受威胁方少掉的士、象分值
	this->vlRed = ADVISOR_BISHOP_ATTACKLESS_VALUE * ( TOTAL_ATTACK_VALUE - blkAttack ) / TOTAL_ATTACK_VALUE;
	this->vlBlk = ADVISOR_BISHOP_ATTACKLESS_VALUE * ( TOTAL_ATTACK_VALUE - redAttack ) / TOTAL_ATTACK_VALUE;

	// 再计算
	for ( int i = 16; i < 32; i ++ ) {
		if ( piece[i] ) {
			this->vlRed += vlPiece[0][PIECE_TYPE[i]][piece[i]];
		}
	}
	for ( int i = 32; i < 48; i ++ ) {
		if ( piece[i] ) {
			this->vlBlk += vlPiece[1][PIECE_TYPE[i]][piece[i]];
		}
	}
}

const int SHAPE_NONE = 0;
const int SHAPE_CENTER = 1;
const int SHAPE_LEFT = 2;
const int SHAPE_RIGHT = 3;

// 特殊棋形
int PositionStruct::AdvisorShape ( void ) const {
	const int rST = SIDE_TAG (0);
	const int bST = SIDE_TAG (1);
	int redPenalty = 0;
	int blkPenalty = 0;
	int kingPos;
	int sqAdv1, sqAdv2;
	int nShape;
	int p, r, c, t;

	if ( piece[rST+ADVISOR_FROM] && piece[rST+ADVISOR_TO] ) { // 红方双士都在
		kingPos = piece[rST+KING_FROM];
		if ( kingPos == 199 ) { // 红方帅在初始位置
			sqAdv1 = piece[rST+ADVISOR_FROM]; // 红方士1的位置
			sqAdv2 = piece[rST+ADVISOR_TO]; // 红方士2的位置
			// 判断士的形状类型
			// 分为：初始类型、左防类型、右防类型、其它类型
			if ( sqAdv1 == 198 ) {
				if ( sqAdv2 == 200 ) {
					nShape = SHAPE_CENTER;
				}
				else if ( sqAdv2 == 183 ) {
					nShape = SHAPE_LEFT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else if ( sqAdv1 == 200 ) {
				if ( sqAdv2 == 198 ) {
					nShape = SHAPE_CENTER;
				}
				else if ( sqAdv2 == 183 ) {
					nShape = SHAPE_RIGHT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else if ( sqAdv1 == 183 ) {
				if ( sqAdv2 == 198 ) {
					nShape = SHAPE_LEFT;
				}
				else if ( sqAdv2 == 200 ) {
					nShape = SHAPE_RIGHT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else {
				nShape = SHAPE_NONE;
			}
			// 判断士的类型结束

			// 根据士的棋形评分
			if ( nShape == SHAPE_NONE ) { // 其它类型
				// 不计分
			}
			else if ( nShape == SHAPE_CENTER ) { // 初始类型
				for ( int pcCannon = bST + CANNON_FROM; pcCannon <= bST + CANNON_TO; pcCannon ++ ) { // 枚举黑方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						c = COL (p);
						if ( c == 7 ) { // 黑炮位于中列
							r = ROW (p);
							t = HIGHER_P[bitCol[c]][r][0];
							if ( p + (t<<4) == kingPos ) { // 黑炮与红将之间有0个子
								redPenalty -= Evl.vlHollowThreat[15-r];
							}
							t = HIGHER_P[bitCol[c]][r][2];
							if ( p + (t<<4) == kingPos ) { // 黑炮与红将之间有2个子
								if ( square[183] == 21 || square[183] == 22 ) { // 有个红马是窝心马
									redPenalty -= Evl.vlCentralThreat[15-r];
								}
							}
						}
					}
				}
			}
			else if ( nShape == SHAPE_LEFT ) { // 左防类型
				for ( int pcCannon = bST + CANNON_FROM; pcCannon <= bST + CANNON_TO; pcCannon ++ ) { // 枚举黑方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						r = ROW (p);
						c = COL (p);
						if ( c == 7 ) { // 黑炮位于中列
							t = HIGHER_P[bitCol[c]][r][2];
							if ( p + (t<<4) == kingPos ) { // 黑炮与红将之间有2个子
								redPenalty -= Evl.vlCentralThreat[15-r] / 4;
								if ( Protected(1, 200) ) { // 若红将出门地被守，还要有罚分
									redPenalty -= 20;
								}

								// 如果红车在底线保护红帅，则给予更大的罚分
								for ( int pcRook = rST + ROOK_FROM; pcRook <= rST + ROOK_TO; pcRook ++ ) {
									p = piece[pcRook];
									if ( p != 0 ) {
										r = ROW (p);
										if ( r == 12 ) { // 红车在沉底位
											c = COL (p);
											t = LOWER_P[bitRow[r]][c][0];
											if ( p - t == kingPos ) { // 红车可保护到红帅
												redPenalty -= 80;
											}
 										}
									}
								}
							}
						}
						if ( r == 12 ) { // 黑炮是沉底炮
							t = LOWER_P[bitRow[r]][c][0];
							if ( p - t == kingPos ) {
								redPenalty -= Evl.vlRedBottomThreat[c];
							}
						}
					}
				}
			}
			else if ( nShape == SHAPE_RIGHT ) { // 右防类型
				for ( int pcCannon = bST + CANNON_FROM; pcCannon <= bST + CANNON_TO; pcCannon ++ ) { // 枚举黑方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						r = ROW (p);
						c = COL (p);
						if ( c == 7 ) { // 黑炮位于中列
							t = HIGHER_P[bitCol[c]][r][2];
							if ( p + (t<<4) == kingPos ) { // 黑炮与红将之间有2个子
								redPenalty -= Evl.vlCentralThreat[15-r] / 4;
								if ( Protected(1, 198) ) { // 若红将出门地被守，还要有罚分
									redPenalty -= 20;
								}

								// 如果红车在底线保护红帅，则给予更大的罚分
								for ( int pcRook = rST + ROOK_FROM; pcRook <= rST + ROOK_TO; pcRook ++ ) {
									p = piece[pcRook];
									if ( p != 0 ) {
										r = ROW (p);
										if ( r == 12 ) { // 红车在沉底位
											c = COL (p);
											t = HIGHER_P[bitRow[r]][c][0];
											if ( p + t == kingPos ) { // 红车可保护到红帅
												redPenalty -= 80;
											}
 										}
									}
								}
							}
						}
						if ( r == 12 ) { // 黑炮是沉底炮
							t = HIGHER_P[bitRow[r]][c][0];
							if ( p + t == kingPos ) {
								redPenalty -= Evl.vlRedBottomThreat[c];
							}
						}
					}
				}
			}
		}
		else if ( kingPos == 183 ) { // 红方帅在初始位置上方的位置
			redPenalty -= 20;
		}
	}
	else { // 红方至少缺一士
		if ( piece[bST+ROOK_FROM] && piece[bST+ROOK_TO] ) { // 黑方双车在
			redPenalty -= Evl.vlRedAdvisorLeakage;
		}
	}

	if ( piece[bST+ADVISOR_FROM] && piece[bST+ADVISOR_TO] ) { // 黑方双士都在
		kingPos = piece[bST+KING_FROM];
		if ( kingPos == 55 ) { // 黑方帅在初始位置
			sqAdv1 = piece[bST+ADVISOR_FROM]; // 黑方士1的位置
			sqAdv2 = piece[bST+ADVISOR_TO]; // 黑方士2的位置
			// 判断士的形状类型
			// 分为：初始类型、左防类型、右防类型、其它类型
			if ( sqAdv1 == 54 ) {
				if ( sqAdv2 == 56 ) {
					nShape = SHAPE_CENTER;
				}
				else if ( sqAdv2 == 71 ) {
					nShape = SHAPE_LEFT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else if ( sqAdv1 == 56 ) {
				if ( sqAdv2 == 54 ) {
					nShape = SHAPE_CENTER;
				}
				else if ( sqAdv2 == 71 ) {
					nShape = SHAPE_RIGHT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else if ( sqAdv1 == 71 ) {
				if ( sqAdv2 == 54 ) {
					nShape = SHAPE_LEFT;
				}
				else if ( sqAdv2 == 56 ) {
					nShape = SHAPE_RIGHT;
				}
				else {
					nShape = SHAPE_NONE;
				}
			}
			else {
				nShape = SHAPE_NONE;
			}
			// 判断士的类型结束

			// 根据士的棋形评分
			if ( nShape == SHAPE_NONE ) { // 其它类型
				// 不计分
			}
			else if ( nShape == SHAPE_CENTER ) { // 初始类型
				for ( int pcCannon = rST + CANNON_FROM; pcCannon <= rST + CANNON_TO; pcCannon ++ ) { // 枚举红方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						c = COL (p);
						if ( c == 7 ) { // 红炮位于中列
							r = ROW (p);
							t = LOWER_P[bitCol[c]][r][0];
							if ( p - (t<<4) == kingPos ) { // 红炮与黑将之间有0个子
								blkPenalty -= Evl.vlHollowThreat[r];
							}
							t = LOWER_P[bitCol[c]][r][2];
							if ( p - (t<<4) == kingPos ) { // 红炮与黑将之间有2个子
								if ( square[71] == 37 || square[71] == 38 ) { // 有个黑马是窝心马
									blkPenalty -= Evl.vlCentralThreat[r];
								}
							}
						}
					}
				}
			}
			else if ( nShape == SHAPE_LEFT ) { // 左防类型
				for ( int pcCannon = rST + CANNON_FROM; pcCannon <= rST + CANNON_TO; pcCannon ++ ) { // 枚举红方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						r = ROW (p);
						c = COL (p);
						if ( c == 7 ) { // 黑炮位于中列
							t = LOWER_P[bitCol[c]][r][2];
							if ( p - (t<<4) == kingPos ) { //红炮与黑将之间有2个子
								blkPenalty -= Evl.vlCentralThreat[r] / 4;
								if ( Protected(0, 56) ) { // 若黑将出门地被守，还要有罚分
									blkPenalty -= 20;
								}

								// 如果黑车在底线保护黑帅，则给予更大的罚分
								for ( int pcRook = bST + ROOK_FROM; pcRook <= bST + ROOK_TO; pcRook ++ ) {
									p = piece[pcRook];
									if ( p != 0 ) {
										r = ROW (p);
										if ( r == 3 ) { // 黑车在沉底位
											c = COL (p);
											t = LOWER_P[bitRow[r]][c][0];
											if ( p - t == kingPos ) { // 黑车可保护到黑帅
												blkPenalty -= 80;
											}
 										}
									}
								}
							}
						}
						if ( r == 3 ) { // 红炮是沉底炮
							t = LOWER_P[bitRow[r]][c][0];
							if ( p - t == kingPos ) {
								blkPenalty -= Evl.vlBlkBottomThreat[c];
							}
						}
					}
				}
			}
			else if ( nShape == SHAPE_RIGHT ) { // 右防类型
				for ( int pcCannon = rST + CANNON_FROM; pcCannon <= rST + CANNON_TO; pcCannon ++ ) { // 枚举红方的炮
					p = piece[pcCannon];
					if ( p != 0 ) {
						r = ROW (p);
						c = COL (p);
						if ( c == 7 ) { // 红炮位于中列
							t = LOWER_P[bitCol[c]][r][2];
							if ( p - (t<<4) == kingPos ) { // 红炮与黑将之间有2个子
								blkPenalty -= Evl.vlCentralThreat[r] / 4;
								if ( Protected(0, 54) ) { // 若红将出门地被守，还要有罚分
									blkPenalty -= 20;
								}

								// 如果黑车在底线保护黑帅，则给予更大的罚分
								for ( int pcRook = bST + ROOK_FROM; pcRook <= bST + ROOK_TO; pcRook ++ ) {
									p = piece[pcRook];
									if ( p != 0 ) {
										r = ROW (p);
										if ( r == 3 ) { // 黑车在沉底位
											c = COL (p);
											t = HIGHER_P[bitRow[r]][c][0];
											if ( p + t == kingPos ) { // 黑车可保护到黑帅
												blkPenalty -= 80;
											}
 										}
									}
								}
							}
						}
						if ( r == 3 ) { // 红炮是沉底炮
							t = HIGHER_P[bitRow[r]][c][0];
							if ( p + t == kingPos ) {
								blkPenalty -= Evl.vlBlkBottomThreat[c];
							}
						}
					}
				}
			}
		}
		else if ( kingPos == 71 ) { // 黑方帅在初始位置下方的位置
			blkPenalty -= 20;
		}
	}
	else { // 黑方至少缺一士
		if ( piece[rST+ROOK_FROM] && piece[rST+ROOK_TO] ) { // 红方双车在
			blkPenalty -= Evl.vlBlkAdvisorLeakage;
		}
	}

	return SIDE_VALUE ( this->player, redPenalty - blkPenalty );
}

// 常数表"cnValuableStringPieces"用判断牵制是否有价值
// 大于0的项是对于车来说的，牵制马和炮(被牵制)都有价值，大于1的项是对于炮来说只有牵制马才有价值
const int cnValuableStringPieces[48] = {
		  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 2, 2, 0, 0, 1, 1, 0, 0, 0, 0, 0
};

// "ccvlStringValueTab"是类似"KNIGHT_PIN_TAB"的常数表(参阅"pregen.h")，决定牵制价值
// 中间子和被牵制子的距离越近，牵制的价值就越大
const char ccvlStringValueTab[512] = {
                               0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
  12, 16, 20, 24, 28, 32, 36,  0, 36, 32, 28, 24, 20, 16, 12,  0,
   0,  0,  0,  0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 36,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 28,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 20,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0, 12,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0
};

// 牵制
int PositionStruct::StringHold ( void ) const {
	int ST, OPP_ST;
	int src, mid, dst;
	int r, c, t;
	int vlString[2];

	for ( int sd = 0; sd < 2; sd ++ ) {
		vlString[sd] = 0;
		ST = SIDE_TAG (sd);
		OPP_ST = OPP_SIDE_TAG (sd);

		// 1. 考虑用车来牵制的情况
		for ( int i = ROOK_FROM; i <= ROOK_TO; i ++ ) {
			src = piece[i+ST];
			r = ROW (src);
			c = COL (src);
			if ( src != 0 ) {
				// 1.1 考虑牵制目标是帅
				dst = piece[KING_FROM+OPP_ST];
				if ( dst != 0 ) {
					for ( int k = 0; k < 4; k ++ ) {
						mid = 0;
						if ( k == 0 ) {
							t = LOWER_P[bitCol[c]][r][1];
							if ( src - (t<<4) == dst ) {
								t = LOWER_P[bitCol[c]][r][0];
								mid = src - (t<<4);
							}
						}
						else if ( k == 1 ) {
							t = HIGHER_P[bitCol[c]][r][1];
							if ( src + (t<<4) == dst ) {
								t = HIGHER_P[bitCol[c]][r][0];
								mid = src + (t<<4);
							}
						}
						else if ( k == 2 ) {
							t = LOWER_P[bitRow[r]][c][1];
							if ( src - t == dst ) {
								t = LOWER_P[bitRow[r]][c][0];
								mid = src - t;
							}
						}
						else if ( k == 3 ) {
							t = HIGHER_P[bitRow[r]][c][1];
							if ( src + t == dst ) {
								t = HIGHER_P[bitRow[r]][c][0];
								mid = src + t;
							}
						}

						if ( mid != 0 ) {
							if ( COLOR_TYPE(square[src]) != COLOR_TYPE(square[mid]) ) { // 颜色不同
								if ( cnValuableStringPieces[square[mid]] > 0 ) { // 有价值，注意是0
									if ( !Protected(1-sd, mid, dst) ) { // mid有条件的未被保护
										vlString[sd] += ccvlStringValueTab[dst-mid+256];
									}
								}
							}
						}
					}
				}
				// 1.2 考虑牵制目标是车的情况
				for ( int j = ROOK_FROM; j <= ROOK_TO; j ++ ) {
					dst = piece[j+OPP_ST];
					if ( dst != 0 ) {
						for ( int k = 0; k < 4; k ++ ) {
							mid = 0;
							if ( k == 0 ) {
								t = LOWER_P[bitCol[c]][r][1];
								if ( src - (t<<4) == dst ) {
									t = LOWER_P[bitCol[c]][r][0];
									mid = src - (t<<4);
								}
							}
							else if ( k == 1 ) {
								t = HIGHER_P[bitCol[c]][r][1];
								if ( src + (t<<4) == dst ) {
									t = HIGHER_P[bitCol[c]][r][0];
									mid = src + (t<<4);
								}
							}
							else if ( k == 2 ) {
								t = LOWER_P[bitRow[r]][c][1];
								if ( src - t == dst ) {
									t = LOWER_P[bitRow[r]][c][0];
									mid = src - t;
								}
							}
							else if ( k == 3 ) {
								t = HIGHER_P[bitRow[r]][c][1];
								if ( src + t == dst ) {
									t = HIGHER_P[bitRow[r]][c][0];
									mid = src + t;
								}
							}

							if ( mid != 0 ) {
								if ( COLOR_TYPE(square[src]) != COLOR_TYPE(square[mid]) ) { // 颜色不同
									if ( cnValuableStringPieces[square[mid]] > 0 ) { // 有价值，注意是0
										if ( !Protected(1-sd, mid, dst) ) { // mid有条件的未被保护
											if ( !Protected(1-sd, dst) ) { // dst也要未被保护，因为是车牵制车
												vlString[sd] += ccvlStringValueTab[dst-mid+256];
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		// 2. 考虑用炮来牵制的情况
		for ( int i = CANNON_FROM; i <= CANNON_TO; i ++ ) {
			src = piece[i+ST];
			r = ROW (src);
			c = COL (src);
			if ( src != 0 ) {
				// 2.1 考虑牵制目标是帅
				dst = piece[KING_FROM+OPP_ST];
				if ( dst != 0 ) {
					for ( int k = 0; k < 4; k ++ ) {
						mid = 0;
						if ( k == 0 ) {
							t = LOWER_P[bitCol[c]][r][2];
							if ( src - (t<<4) == dst ) {
								t = LOWER_P[bitCol[c]][r][1];
								mid = src - (t<<4);
							}
						}
						else if ( k == 1 ) {
							t = HIGHER_P[bitCol[c]][r][2];
							if ( src + (t<<4) == dst ) {
								t = HIGHER_P[bitCol[c]][r][1];
								mid = src + (t<<4);
							}
						}
						else if ( k == 2 ) {
							t = LOWER_P[bitRow[r]][c][2];
							if ( src - t == dst ) {
								t = LOWER_P[bitRow[r]][c][1];
								mid = src - t;
							}
						}
						else if ( k == 3 ) {
							t = HIGHER_P[bitRow[r]][c][2];
							if ( src + t == dst ) {
								t = HIGHER_P[bitRow[r]][c][1];
								mid = src + t;
							}
						}

						if ( mid != 0 ) {
							if ( COLOR_TYPE(square[src]) != COLOR_TYPE(square[mid]) ) { // 颜色不同
								if ( cnValuableStringPieces[square[mid]] > 1 ) { // 有价值，注意是1
									if ( !Protected(1-sd, mid, dst) ) { // mid有条件的未被保护
										vlString[sd] += ccvlStringValueTab[dst-mid+256];
									}
								}
							}
						}
					}
				}
				// 2.2 考虑牵制目标是车的情况
				for ( int j = ROOK_FROM; j <= ROOK_TO; j ++ ) {
					dst = piece[j+OPP_ST];
					if ( dst != 0 ) {
						for ( int k = 0; k < 4; k ++ ) {
							mid = 0;
							if ( k == 0 ) {
								t = LOWER_P[bitCol[c]][r][2];
								if ( src - (t<<4) == dst ) {
									t = LOWER_P[bitCol[c]][r][1];
									mid = src - (t<<4);
								}
							}
							else if ( k == 1 ) {
								t = HIGHER_P[bitCol[c]][r][2];
								if ( src + (t<<4) == dst ) {
									t = HIGHER_P[bitCol[c]][r][1];
									mid = src + (t<<4);
								}
							}
							else if ( k == 2 ) {
								t = LOWER_P[bitRow[r]][c][2];
								if ( src - t == dst ) {
									t = LOWER_P[bitRow[r]][c][1];
									mid = src - t;
								}
							}
							else if ( k == 3 ) {
								t = HIGHER_P[bitRow[r]][c][2];
								if ( src + t == dst ) {
									t = HIGHER_P[bitRow[r]][c][1];
									mid = src + t;
								}
							}

							if ( mid != 0 ) {
								if ( COLOR_TYPE(square[src]) != COLOR_TYPE(square[mid]) ) { // 颜色不同
									if ( cnValuableStringPieces[square[mid]] > 1 ) { // 有价值，注意是1
										if ( !Protected(1-sd, mid, dst) ) { // mid有条件的未被保护
											if ( true ) { // dst不需要被保护，因为是炮牵制炮
												vlString[sd] += ccvlStringValueTab[dst-mid+256];
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return SIDE_VALUE ( this->player, vlString[0] - vlString[1] );
}

// 打分，车的灵活性
int PositionStruct::RookMobility ( void ) const {
	int value[2];
	for ( int sd = 0; sd < 2; sd ++ ) {
		value[sd] = 0;
		int ST = SIDE_TAG (sd);
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
		int ST = SIDE_TAG (sd);
		for ( int i = KNIGHT_FROM; i <= KNIGHT_TO; i ++ ) {
			if ( piece[i+ST] ) {
				int nMovable = 0;
				int k = 0;
				while ( KNIGHT_HIT[ piece[i+ST] ][k] != 0 ) {
					int hit = KNIGHT_HIT[ piece[i+ST] ][k];
					int pin = KNIGHT_PIN[ piece[i+ST] ][k];
					if ( square[hit] == 0 && square[pin] == 0 && !Protected2(1-sd, piece[i+ST], hit) ) {
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
int PositionStruct::Evaluate ( void ) const {
	int vl = 0;

	// 1. 子力平衡评价
	vl += Material ();

	// 2. 特殊棋形
	vl += AdvisorShape ();

	// 3. 牵制
	vl += StringHold ();

	// 4. 车的灵活性
	vl += RookMobility ();

	// 5. 马的阻碍
	vl += KnightTrap ();

	return vl;
}
