#include "MoveLegalityTesting.h"
#include "Position.h"
#include "magicmoves.h"

/* This function takes a bitboard called To_Square that has only the bit set that the move moves to; it takes a From_Square that is the same,
and it has the bit set that the piece in the move moves to. It also takes a Move_Type, which tells it which way to update its bitboards
*/
#include "Util.h"
bool White_Is_Legal(Position& position, Move move) {
	position.Make_Move(move);
	int h = lsb(position.White_King);

	if (Knight_Lookup_Table[h] & position.Black_Knights) {
		position.Undo_Move(move);
		return false;
	}
	Bitboard Spare = position.Black_Pawns;
	Spare |= H_Pawn_Mask;
	Spare ^= H_Pawn_Mask;
	if ((Spare >> 7) & position.White_King) {
		position.Undo_Move(move);
		return false;
	}
	Spare = position.Black_Pawns;
	Spare |= A_Pawn_Mask;
	Spare ^= A_Pawn_Mask;
	if ((Spare >> 9) & position.White_King) {
		position.Undo_Move(move);
		return false;
	}

	if (King_Lookup_Table[h] & position.Black_King) {
		position.Undo_Move(move);
		return false;
	}

	Bitboard BAttacks = Bmagic(h, (position.White_Pieces | position.Black_Pieces));

	if (BAttacks & (position.Black_Bishops)) {
		position.Undo_Move(move);
		return false;
	}

	Bitboard RAttacks = Rmagic(h, (position.White_Pieces | position.Black_Pieces));

	if (RAttacks & (position.Black_Rooks)) {
		position.Undo_Move(move);
		return false;
	}

	Bitboard QAttacks = BAttacks | RAttacks;
	if (QAttacks & (position.Black_Queens)) {
		position.Undo_Move(move);
		return false;
	}

	position.Undo_Move(move);
	return true;
}


bool Black_Is_Legal(Position& position, Move move) {
	position.Make_Move(move);
	int h = lsb(position.Black_King);

	if (Knight_Lookup_Table[h] & position.White_Knights) {
		position.Undo_Move(move);
		return false;
	}
	Bitboard Spare = position.White_Pawns;
	Spare |= A_Pawn_Mask;
	Spare ^= A_Pawn_Mask;
	if ((Spare << 7) & position.Black_King) {
		position.Undo_Move(move);
		return false;
	}
	Spare = position.White_Pawns;
	Spare |= H_Pawn_Mask;
	Spare ^= H_Pawn_Mask;
	if ((Spare << 9) & position.Black_King) {
		position.Undo_Move(move);
		return false;
	}

	if (King_Lookup_Table[h] & position.White_King) {
		position.Undo_Move(move);
		return false;
	}

	Bitboard BAttacks = Bmagic(h, (position.White_Pieces | position.Black_Pieces));
	if (BAttacks & (position.White_Bishops)) {
		position.Undo_Move(move);
		return false;
	}

	Bitboard RAttacks = Rmagic(h, (position.White_Pieces | position.Black_Pieces));
	if (RAttacks & (position.White_Rooks)) {
		position.Undo_Move(move);
		return false;
	}

	if ((BAttacks | RAttacks) & (position.White_Queens)) {
		position.Undo_Move(move);
		return false;
	}

	position.Undo_Move(move);
	return true;
}

