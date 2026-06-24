#ifndef Position_H_
#define Position_H_
#include <vector>
#include <algorithm>
#include "Bitboard.h"

enum Piece {
	WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK, NONE
};

class Move {
public:
	int Score;
	bool Castling;
	bool Promotion;
	bool EP;
	Piece P;
	Piece C;
	Bitboard From;
	Bitboard To;
	Piece PromotionType;
	Move();
	Move(Piece piece, Piece captured, Bitboard from, Bitboard to, bool castling, bool promotion, bool ep);
	void Output();
};

class Position {
public:
	bool WhiteCanCastleK;
	bool WhiteCanCastleQ;
	bool BlackCanCastleK;
	bool BlackCanCastleQ;
	bool PrevWhiteCanCastleK;
	bool PrevWhiteCanCastleQ;
	bool PrevBlackCanCastleK;
	bool PrevBlackCanCastleQ;
	bool Current_Turn;
	int numlegalmoves;
	Bitboard White_Pieces;
	Bitboard Black_Pieces;
	Bitboard White_King;
	Bitboard Black_King;
	Bitboard White_Queens;
	Bitboard White_Rooks;
	Bitboard White_Bishops;
	Bitboard White_Knights;
	Bitboard White_Pawns;
	Bitboard Black_Queens;
	Bitboard Black_Rooks;
	Bitboard Black_Bishops;
	Bitboard Black_Knights;
	Bitboard Black_Pawns;
	Bitboard hashkey;
	Bitboard WhiteAttacks;
	Bitboard BlackAttacks;
	Bitboard EP_Square;
	Bitboard Prev_EP_Square;
	Move LegalMoves[100];
	void Print();
	Position();
	~Position();
	Position(Position* position);
	void Reset();
	void Make_Move(Move m);
	void Undo_Move(Move m);
	int Non_Pawn_Material();
	string GetTextBoard() const;
	Bitboard Get_White_Attacks();
	Bitboard GetLeastWhiteAttacker(Bitboard b);
	Bitboard Get_Black_Attacks();
	Bitboard GetLeastBlackAttacker(Bitboard board);
	Bitboard* Get_Bitboard_From_Piece(Piece p);
	Piece Get_Piece_From_Bitboard(Bitboard b);
};

extern Position pos;

int Get_Cp_Value(Piece piece);
#endif
