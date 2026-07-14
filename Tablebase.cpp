#include "Tablebase.h"


// Probe the currently-loaded position against the Nalimov tablebases.
// Assumes IInitializeTb() has already been called (via the "tb" command).

// Returns the score of the current position from the player to move's perspective
// Return value of 127 means position is not found in the tablebases (or tablebases not loaded)
// Return value of 0 means it's a draw
// Score of -100 means opponent mates in 27 moves
// Score of 103 meamns we mate the opponent in 24 moves
// In general, 127 - score = number of moves till mate (from the player to move's perspective)

int ProbePositionNalimov(Position& position) {

	// Count pieces for tablebase lookup (indices: 0-4 white P,N,B,R,Q; 5-9 black P,N,B,R,Q)
	int pieces[10];
	pieces[0] = (int)__popcnt64(position.White_Pawns);
	pieces[1] = (int)__popcnt64(position.White_Knights);
	pieces[2] = (int)__popcnt64(position.White_Bishops);
	pieces[3] = (int)__popcnt64(position.White_Rooks);
	pieces[4] = (int)__popcnt64(position.White_Queens);
	pieces[5] = (int)__popcnt64(position.Black_Pawns);
	pieces[6] = (int)__popcnt64(position.Black_Knights);
	pieces[7] = (int)__popcnt64(position.Black_Bishops);
	pieces[8] = (int)__popcnt64(position.Black_Rooks);
	pieces[9] = (int)__popcnt64(position.Black_Queens);

	// Look up the (signed) tablebase descriptor for the ACTUAL material.
	// A positive id means the position already matches the stored orientation
	// (the "white" side of the table is our white side). A negative id means the
	// position is the colour-inverse of a stored table: the strong side is black,
	// so we must swap colours AND vertically flip the board so that black pawns
	// (which move down) become white pawns (which move up) for the lookup.
	int tbid_signed = IDescFindFromCounters(pieces);
	if (tbid_signed == 0) {
		cout << "No tablebase found for this position" << endl;
		return 0;
	}
	bool invert = (tbid_signed < 0);
	int tbid = invert ? -tbid_signed : tbid_signed;

	// Extract piece squares into the tablebase's white/black arrays.
	// Layout expected by the Nalimov index functions: [0] = king, then the
	// remaining pieces in DESCENDING value order (Q, R, B, N, P) to match the
	// tablebase naming convention. When inverting we read the opposite colour and
	// vertically flip each square (sq ^ 56).
	int wpieces[16] = { 0 };
	int bpieces[16] = { 0 };
	int wc = 0, bc = 0;

	Bitboard sk_King, sk_Queens, sk_Rooks, sk_Bishops, sk_Knights, sk_Pawns; // strong (TB white) side
	Bitboard wk_King, wk_Queens, wk_Rooks, wk_Bishops, wk_Knights, wk_Pawns; // weak   (TB black) side
	if (!invert) {
		sk_King = position.White_King;   sk_Queens = position.White_Queens; sk_Rooks = position.White_Rooks;
		sk_Bishops = position.White_Bishops; sk_Knights = position.White_Knights; sk_Pawns = position.White_Pawns;
		wk_King = position.Black_King;   wk_Queens = position.Black_Queens; wk_Rooks = position.Black_Rooks;
		wk_Bishops = position.Black_Bishops; wk_Knights = position.Black_Knights; wk_Pawns = position.Black_Pawns;
	}
	else {
		sk_King = position.Black_King;   sk_Queens = position.Black_Queens; sk_Rooks = position.Black_Rooks;
		sk_Bishops = position.Black_Bishops; sk_Knights = position.Black_Knights; sk_Pawns = position.Black_Pawns;
		wk_King = position.White_King;   wk_Queens = position.White_Queens; wk_Rooks = position.White_Rooks;
		wk_Bishops = position.White_Bishops; wk_Knights = position.White_Knights; wk_Pawns = position.White_Pawns;
	}

#define TB_SQ(s) (invert ? ((s) ^ 56) : (s))

	// TB white side (king first, then Q,R,B,N,P)
	wpieces[wc++] = TB_SQ(lsb(sk_King));
	for (Bitboard b = sk_Queens; b; b &= b - 1) wpieces[wc++] = TB_SQ(lsb(b));
	for (Bitboard b = sk_Rooks; b; b &= b - 1) wpieces[wc++] = TB_SQ(lsb(b));
	for (Bitboard b = sk_Bishops; b; b &= b - 1) wpieces[wc++] = TB_SQ(lsb(b));
	for (Bitboard b = sk_Knights; b; b &= b - 1) wpieces[wc++] = TB_SQ(lsb(b));
	for (Bitboard b = sk_Pawns; b; b &= b - 1) wpieces[wc++] = TB_SQ(lsb(b));

	// TB black side
	bpieces[bc++] = TB_SQ(lsb(wk_King));
	for (Bitboard b = wk_Queens; b; b &= b - 1) bpieces[bc++] = TB_SQ(lsb(b));
	for (Bitboard b = wk_Rooks; b; b &= b - 1) bpieces[bc++] = TB_SQ(lsb(b));
	for (Bitboard b = wk_Bishops; b; b &= b - 1) bpieces[bc++] = TB_SQ(lsb(b));
	for (Bitboard b = wk_Knights; b; b &= b - 1) bpieces[bc++] = TB_SQ(lsb(b));
	for (Bitboard b = wk_Pawns; b; b &= b - 1) bpieces[bc++] = TB_SQ(lsb(b));

#undef TB_SQ

	// Side to move, as seen by the tablebase (0 = TB white to move, 1 = TB black).
	// position.Current_Turn is true when white is to move. When inverting, the TB white
	// side is our black side, so the side-to-move flips.
	int tb_side;
	if (!invert)
		tb_side = position.Current_Turn ? 0 : 1;
	else
		tb_side = position.Current_Turn ? 1 : 0;

	PfnCalcIndex pfnCalc = PfnIndCalcFun(tbid, tb_side);
	if (pfnCalc == NULL) {
		cout << "No index function found for this tablebase" << endl;
		return 0;
	}

	// En passant. Nalimov's sqEnP is the en-passant target square (identical to
	// the FEN e.p. square) but must be given ONLY when the side to move can
	// actually capture there; otherwise it must be XX (-1). When inverting we
	// vertically flip it (^ 56) to match the flipped piece squares.
	int sqEnP = -1;  // XX
	if (position.EP_Square != 0) {
		int epsq = lsb(position.EP_Square);
		int epFile = epsq & 7, epRank = epsq >> 3;
		bool canCapture = false;
		if (position.Current_Turn) {
			// White to move: e.p. target on rank 6 (index 5), capturing white pawn on rank 5.
			if (epRank == 5) {
				for (int df = -1; df <= 1; df += 2) {
					int ff = epFile + df;
					if (ff >= 0 && ff < 8 && (position.White_Pawns & (1ULL << (32 + ff))))
						canCapture = true;
				}
			}
		}
		else {
			// Black to move: e.p. target on rank 3 (index 2), capturing black pawn on rank 4.
			if (epRank == 2) {
				for (int df = -1; df <= 1; df += 2) {
					int ff = epFile + df;
					if (ff >= 0 && ff < 8 && (position.Black_Pawns & (1ULL << (24 + ff))))
						canCapture = true;
				}
			}
		}
		if (canCapture)
			sqEnP = invert ? (epsq ^ 56) : epsq;
	}

	unsigned long tbindex = pfnCalc(wpieces, bpieces, sqEnP, 0);
	int score = TbtProbeTable(tbid, tb_side, tbindex);

	return score;
}