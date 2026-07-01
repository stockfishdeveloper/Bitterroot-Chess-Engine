using namespace std;
#include "Bitboard.h"
#include "MoveGen.h"
#include "Search.h"
#include "magicmoves.h"
#include "Util.h"

bool Is_Legal;
void Generate_White_Moves(const bool caps, Position& position) {
	bool inCheck = Search::Is_Mate(&position) < 0;
	Bitboard blockers = Qmagic(lsb(position.White_King), position.White_Pieces | position.Black_Pieces) & position.White_Pieces;
	position.numlegalmoves = 0;

	Bitboard Pawn_Save = position.White_Pawns;

	while (Pawn_Save) {
		short Pawn_LSB = lsb(Pawn_Save);
		
		if (!caps) {
			if (GeneralBoard[Pawn_LSB] & 65280) {
				if (!((GeneralBoard[Pawn_LSB] << 8) & (position.White_Pieces | position.Black_Pieces))) {
					Move m(WP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 8, false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
					if (!((GeneralBoard[Pawn_LSB] << 16) & (position.White_Pieces | position.Black_Pieces))) {
						Move m(WP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 16, false, false, false);
						if (inCheck || (m.From & blockers)) {
							if (White_Is_Legal(position, m))
								position.LegalMoves[position.numlegalmoves++] = m;
						}
						else
							position.LegalMoves[position.numlegalmoves++] = m;
					}
				}
			}
			else {
				if ((!((GeneralBoard[Pawn_LSB] << 8) & (position.White_Pieces | position.Black_Pieces))) && (!(GeneralBoard[Pawn_LSB] & 71776119061217280))) {
					Move m(WP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 8, false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}
		if ((GeneralBoard[Pawn_LSB] & 71776119061217280) && (!(GeneralBoard[Pawn_LSB] << 8 & (position.White_Pieces | position.Black_Pieces)))) {
			Move m(WP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 8, false, true, false);
			for (int p = 1; p < 5; p++) {
				m.PromotionType = Piece(int(WP) + p);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}
		if (!(GeneralBoard[Pawn_LSB] & A_Pawn_Mask)) {
			if ((GeneralBoard[Pawn_LSB] << 7) & position.Black_Pieces && (GeneralBoard[Pawn_LSB] & 71776119061217280)) {
				Move m(WP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] << 7), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 7, false, true, false);
				{
					for (int p = 1; p < 5; p++) {
						m.PromotionType = Piece(int(WP) + p);
						if (inCheck || (m.From & blockers)) {
							if (White_Is_Legal(position, m))
								position.LegalMoves[position.numlegalmoves++] = m;
						}
						else
							position.LegalMoves[position.numlegalmoves++] = m;
					}
				}
			}
			else if ((GeneralBoard[Pawn_LSB] << 7) & position.Black_Pieces) {
				Move m(WP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] << 7), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 7, false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}

			// if EP is possible capturing up to the left
			if ((GeneralBoard[Pawn_LSB] << 7) & position.EP_Square) {
				Move m(WP, BP, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 7, false, false, true);

				if (White_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}
		if (!(GeneralBoard[Pawn_LSB] & H_Pawn_Mask)) {
			if ((GeneralBoard[Pawn_LSB] << 9) & position.Black_Pieces && (GeneralBoard[Pawn_LSB] & 71776119061217280)) {
				Move m(WP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] << 9), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 9, false, true, false);
				for (int p = 1; p < 5; p++) {
					m.PromotionType = Piece(int(WP) + p);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
			else if ((GeneralBoard[Pawn_LSB] << 9) & position.Black_Pieces) {
				Move m(WP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] << 9), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 9, false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}

			// if EP is possible capturing up to the right
			if ((GeneralBoard[Pawn_LSB] << 9) & position.EP_Square) {
				Move m(WP, BP, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] << 9, false, false, true);

				if (White_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}

		Pawn_Save ^= GeneralBoard[Pawn_LSB];
	}

	Bitboard King_Save = position.White_King;

	while (King_Save) {
		short King_LSB = lsb(King_Save);
		Bitboard possible_locations = ((King_Lookup_Table[King_LSB] | position.White_Pieces) ^ position.White_Pieces);

		int p = __popcnt64(possible_locations);

		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.Black_Pieces) {
				Move m(WK, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[King_LSB], GeneralBoard[j], false, false, false);
				if (White_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(WK, NONE, GeneralBoard[King_LSB], GeneralBoard[j], false, false, false);
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		if (!caps && position.WhiteCanCastleK) {
			Bitboard first = (GeneralBoard[King_LSB] & 16);
			Bitboard second = ((position.White_Pieces | position.Black_Pieces) & 96);
			Bitboard e1 = 16, f1 = 32, g1 = 64;
			bool Rookonh1 = position.White_Rooks & 128;
			if (first && !second && Rookonh1) {
				Move m(WK, NONE, GeneralBoard[King_LSB], f1, false, false, false);
				bool safetomovetof1 = White_Is_Legal(position, m);
				Move m1(WK, NONE, GeneralBoard[King_LSB], g1, false, false, false);
				bool safetomovetog1 = White_Is_Legal(position, m1);
				Move m2(WK, NONE, GeneralBoard[King_LSB], e1, false, false, false);
				bool safetomovetoe1 = Search::Is_Mate(&position) >= 0;
				if (safetomovetof1 && safetomovetog1 && safetomovetoe1) {
					m1.Castling = true;
					position.LegalMoves[position.numlegalmoves++] = m1;
				}
			}
		}
		if (!caps && position.WhiteCanCastleQ) {
			Bitboard first = (GeneralBoard[King_LSB] & 16);
			Bitboard second = ((position.White_Pieces | position.Black_Pieces) & 14);
			Bitboard e1 = 16, d1 = 8, c1 = 4;
			bool Rookona1 = position.White_Rooks & 1;
			if (first && !second && Rookona1) {
				Move m(WK, NONE, GeneralBoard[King_LSB], d1, false, false, false);
				bool safetomovetod1 = White_Is_Legal(position, m);
				Move m1(WK, NONE, GeneralBoard[King_LSB], c1, false, false, false);
				bool safetomovetoc1 = White_Is_Legal(position, m1);
				Move m2(WK, NONE, GeneralBoard[King_LSB], e1, false, false, false);
				bool safetomovetoe1 = Search::Is_Mate(&position) >= 0;
				if (safetomovetoc1 && safetomovetod1 && safetomovetoe1) {
					m1.Castling = true;
					position.LegalMoves[position.numlegalmoves++] = m1;
				}
			}
		}

		King_Save ^= GeneralBoard[King_LSB];
	}

	Bitboard Knight_Save = position.White_Knights;

	while(Knight_Save) {
		short Knight_LSB = lsb(Knight_Save);
		Bitboard possible_locations = ((Knight_Lookup_Table[Knight_LSB] | position.White_Pieces) ^ position.White_Pieces);

		int p = __popcnt64(possible_locations);

		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.Black_Pieces) {
				Move m(WN, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Knight_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(WN, NONE, GeneralBoard[Knight_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Knight_Save ^= GeneralBoard[Knight_LSB];
	}

	Bitboard Bishop_Save = position.White_Bishops;

	while (Bishop_Save) {
		short Bishop_LSB = lsb(Bishop_Save);
		Bitboard possible_locations = ((Bmagic(Bishop_LSB, position.White_Pieces | position.Black_Pieces) | position.White_Pieces) ^ position.White_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations  ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.Black_Pieces) {
				Move m(WB, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Bishop_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(WB, NONE, GeneralBoard[Bishop_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Bishop_Save ^= GeneralBoard[Bishop_LSB];
	}

	Bitboard Rook_Save = position.White_Rooks;

	while (Rook_Save) {
		short Rook_LSB = lsb(Rook_Save);
		Bitboard possible_locations = ((Rmagic(Rook_LSB, position.White_Pieces | position.Black_Pieces) | position.White_Pieces) ^ position.White_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.Black_Pieces) {
				Move m(WR, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Rook_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(WR, NONE, GeneralBoard[Rook_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Rook_Save ^= GeneralBoard[Rook_LSB];
	}

	Bitboard Queen_Save = position.White_Queens;

	while (Queen_Save) {
		short Queen_LSB = lsb(Queen_Save);
		Bitboard possible_locations = ((Qmagic(Queen_LSB, position.White_Pieces | position.Black_Pieces) | position.White_Pieces) ^ position.White_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.Black_Pieces) {
				Move m(WQ, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Queen_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (White_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(WQ, NONE, GeneralBoard[Queen_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (White_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Queen_Save ^= GeneralBoard[Queen_LSB];
	}
}

void Generate_Black_Moves(const bool caps, Position& position) {
	bool inCheck = Search::Is_Mate(&position) < 0;
	Bitboard blockers = Qmagic(lsb(position.Black_King), position.White_Pieces | position.Black_Pieces) & position.Black_Pieces;
	position.numlegalmoves = 0;

	Bitboard Pawn_Save = position.Black_Pawns;

	while (Pawn_Save) {
		short Pawn_LSB = lsb(Pawn_Save);

		if (!caps) {
			if (GeneralBoard[Pawn_LSB] & 71776119061217280) {
				if (!((GeneralBoard[Pawn_LSB] >> 8) & (position.White_Pieces | position.Black_Pieces))) {
					Move m(BP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 8, false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
					if (!((GeneralBoard[Pawn_LSB] >> 16) & (position.White_Pieces | position.Black_Pieces))) {
						Move m(BP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 16, false, false, false);
						if (inCheck || (m.From & blockers)) {
							if (Black_Is_Legal(position, m))
								position.LegalMoves[position.numlegalmoves++] = m;
						}
						else
							position.LegalMoves[position.numlegalmoves++] = m;
					}
				}
			}
			else {
				if ((!((GeneralBoard[Pawn_LSB] >> 8) & (position.White_Pieces | position.Black_Pieces))) && (!(GeneralBoard[Pawn_LSB] & 65280))) {
					Move m(BP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 8, false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}
		if (GeneralBoard[Pawn_LSB] & 65280 && (!(GeneralBoard[Pawn_LSB] >> 8 & (position.White_Pieces | position.Black_Pieces)))) {
			Move m(BP, NONE, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 8, false, true, false);
			for (int p = 1; p < 5; p++) {
				m.PromotionType = Piece(int(BP) + p);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}
		if (!(GeneralBoard[Pawn_LSB] & H_Pawn_Mask)) {
			if (((GeneralBoard[Pawn_LSB] >> 7) & position.White_Pieces) && (GeneralBoard[Pawn_LSB] & 65280)) {
				Move m(BP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] >> 7), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 7, false, true, false);
				for (int p = 1; p < 5; p++) {
					m.PromotionType = Piece(int(BP) + p);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
			else if ((GeneralBoard[Pawn_LSB] >> 7) & position.White_Pieces) {
				Move m(BP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] >> 7), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 7, false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}

			// if EP is possible capturing down to the right
			if ((GeneralBoard[Pawn_LSB] >> 7) & position.EP_Square) {
				Move m(BP, WP, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 7, false, false, true);

				if (Black_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}
		if (!(GeneralBoard[Pawn_LSB] & A_Pawn_Mask)) {
			if (((GeneralBoard[Pawn_LSB] >> 9) & position.White_Pieces) && (GeneralBoard[Pawn_LSB] & 65280)) {
				Move m(BP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] >> 9), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 9, false, true, false);
				for (int p = 1; p < 5; p++) {
					m.PromotionType = Piece(int(BP) + p);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
			else if ((GeneralBoard[Pawn_LSB] >> 9) & position.White_Pieces) {
				Move m(BP, position.Get_Piece_From_Bitboard(GeneralBoard[Pawn_LSB] >> 9), GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 9, false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}

			// if EP is possible capturing down to the left
			if ((GeneralBoard[Pawn_LSB] >> 9) & position.EP_Square) {
				Move m(BP, WP, GeneralBoard[Pawn_LSB], GeneralBoard[Pawn_LSB] >> 9, false, false, true);

				if (Black_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
		}

		Pawn_Save ^= GeneralBoard[Pawn_LSB];
	}

	Bitboard King_Save = position.Black_King;

	while (King_Save) {
		short King_LSB = lsb(King_Save);
		Bitboard possible_locations = ((King_Lookup_Table[King_LSB] | position.Black_Pieces) ^ position.Black_Pieces);

		int p = __popcnt64(possible_locations);

		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.White_Pieces) {
				Move m(BK, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[King_LSB], GeneralBoard[j], false, false, false);
				if (Black_Is_Legal(position, m))
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(BK, NONE, GeneralBoard[King_LSB], GeneralBoard[j], false, false, false);
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		if (!caps && position.BlackCanCastleK) {
			Bitboard first = (GeneralBoard[King_LSB] & 1152921504606846976);
			Bitboard second = ((position.White_Pieces | position.Black_Pieces) & 6917529027641081856);
			Bitboard e8 = 1152921504606846976, f8 = 2305843009213693952, g8 = 4611686018427387904;
			bool Rookonh8 = (position.Black_Rooks & 9223372036854775808ULL);
			if (first && !second && Rookonh8) {
				Move m(BK, NONE, GeneralBoard[King_LSB], f8, false, false, false);
				bool safetomovetof8 = Black_Is_Legal(position, m);
				Move m1(BK, NONE, GeneralBoard[King_LSB], g8, false, false, false);
				bool safetomovetog8 = Black_Is_Legal(position, m1);
				Move m2(BK, NONE, GeneralBoard[King_LSB], e8, false, false, false);
				bool safetomovetoe8 = Search::Is_Mate(&position) >= 0;
				if (safetomovetof8 && safetomovetog8 && safetomovetoe8) {
					m1.Castling = true;
					position.LegalMoves[position.numlegalmoves++] = m1;
				}
			}
		}
		if (!caps && position.BlackCanCastleQ) {
			Bitboard first = (GeneralBoard[King_LSB] & 1152921504606846976);
			Bitboard second = ((position.White_Pieces | position.Black_Pieces) & 1008806316530991104);
			Bitboard e8 = 1152921504606846976, d8 = 576460752303423488, c8 = 288230376151711744;
			bool Rookona8 = (position.Black_Rooks & 72057594037927936);
			if (first && !second && Rookona8) {
				Move m(BK, NONE, GeneralBoard[King_LSB], d8, false, false, false);
				bool safetomovetod8 = Black_Is_Legal(position, m);
				Move m1(BK, NONE, GeneralBoard[King_LSB], c8, false, false, false);
				bool safetomovetoc8 = Black_Is_Legal(position, m1);
				Move m2(BK, NONE, GeneralBoard[King_LSB], e8, false, false, false);
				bool safetomovetoe8 = Search::Is_Mate(&position) >= 0;
				if (safetomovetod8 && safetomovetoc8 && safetomovetoe8) {
					m1.Castling = true;
					position.LegalMoves[position.numlegalmoves++] = m1;
				}
			}
		}

		King_Save ^= GeneralBoard[King_LSB];
	}

	Bitboard Knight_Save = position.Black_Knights;
	
	while (Knight_Save) {
		short Knight_LSB = lsb(Knight_Save);
		Bitboard possible_locations = ((Knight_Lookup_Table[Knight_LSB] | position.Black_Pieces) ^ position.Black_Pieces);

		int p = __popcnt64(possible_locations);

		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.White_Pieces) {
				Move m(BN, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Knight_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(BN, NONE, GeneralBoard[Knight_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Knight_Save ^= GeneralBoard[Knight_LSB];
	}

	Bitboard Bishop_Save = position.Black_Bishops;
	
	while (Bishop_Save) {
		short Bishop_LSB = lsb(Bishop_Save);
		Bitboard possible_locations = ((Bmagic(Bishop_LSB, position.White_Pieces | position.Black_Pieces) | position.Black_Pieces) ^ position.Black_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.White_Pieces) {
				Move m(BB, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Bishop_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(BB, NONE, GeneralBoard[Bishop_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Bishop_Save ^= GeneralBoard[Bishop_LSB];
	}

	Bitboard Rook_Save = position.Black_Rooks;

	while (Rook_Save) {
		short Rook_LSB = lsb(Rook_Save);
		Bitboard possible_locations = ((Rmagic(Rook_LSB, position.Black_Pieces | position.White_Pieces) | position.Black_Pieces) ^ position.Black_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.White_Pieces) {
				Move m(BR, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Rook_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(BR, NONE, GeneralBoard[Rook_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Rook_Save ^= GeneralBoard[Rook_LSB];
	}

	Bitboard Queen_Save = position.Black_Queens;

	while (Queen_Save) {
		short Queen_LSB = lsb(Queen_Save);
		Bitboard possible_locations = ((Qmagic(Queen_LSB, position.Black_Pieces | position.White_Pieces) | position.Black_Pieces) ^ position.Black_Pieces);

		int p = __popcnt64(possible_locations);
		for (int m = 0; m < p; m++) {
			short j = lsb(possible_locations);
			possible_locations ^= GeneralBoard[j];
			if (GeneralBoard[j] & position.White_Pieces) {
				Move m(BQ, position.Get_Piece_From_Bitboard(GeneralBoard[j]), GeneralBoard[Queen_LSB], GeneralBoard[j], false, false, false);
				if (inCheck || (m.From & blockers)) {
					if (Black_Is_Legal(position, m))
						position.LegalMoves[position.numlegalmoves++] = m;
				}
				else
					position.LegalMoves[position.numlegalmoves++] = m;
			}
			else {
				if (!caps) {
					Move m(BQ, NONE, GeneralBoard[Queen_LSB], GeneralBoard[j], false, false, false);
					if (inCheck || (m.From & blockers)) {
						if (Black_Is_Legal(position, m))
							position.LegalMoves[position.numlegalmoves++] = m;
					}
					else
						position.LegalMoves[position.numlegalmoves++] = m;
				}
			}
		}

		Queen_Save ^= GeneralBoard[Queen_LSB];
	}
}
