#include "Position.h"
#include "Util.h"
#include "magicmoves.h"
Position pos;

Move::Move() {
	P = NONE;
	C = NONE;
	From = 0;
	To = 0;
	Score = 0;
	Castling = false;
	Promotion = false;
	EP = false;
	PromotionType = NONE;
}
Move::Move(Piece piece, Piece captured, Bitboard from, Bitboard to, bool castling, bool promotion, bool ep) {
	P = piece;
	C = captured;
	From = from;
	To = to;
	Castling = castling;
	Promotion = promotion;
	EP = ep;
	PromotionType = NONE;
	Score = 0;
}
void Move::Output() {
	for (int i = 0; i < 64; i++) {
		if (GeneralBoard[i] & From)
			cout << PlayerMoves[i];
	}
	for (int i = 0; i < 64; i++) {
		if (GeneralBoard[i] & To)
			cout << PlayerMoves[i];
	}
}

void Position::Print() {
	cout << "White Pieces: " << White_Pieces << endl;
	cout << "Black Pieces: " << Black_Pieces << endl;
	cout << "White King: " << White_King << endl;
	cout << "Black King: " << Black_King << endl;
	cout << "White Queens: " << White_Queens << endl;
	cout << "White Rooks: " << White_Rooks << endl;
	cout << "White Bishops: " << White_Bishops << endl;
	cout << "White Knights: " << White_Knights << endl;
	cout << "White Pawns: " << White_Pawns << endl;
	cout << "Black Queens: " << Black_Queens << endl;
	cout << "Black Rooks: " << Black_Rooks << endl;
	cout << "Black Bishops: " << Black_Bishops << endl;
	cout << "Black Knights: " << Black_Knights << endl;
	cout << "Black Pawns: " << Black_Pawns << endl;
	cout << "En Passant Square: " << EP_Square << endl;
	cout << "Previous En Passant Square: " << Prev_EP_Square << endl;
}

Position::Position() {
	White_Pieces = 0;
	Black_Pieces = 0;
	White_King = 0;
	Black_King = 0;
	White_Queens = 0;
	White_Rooks = 0;
	White_Bishops = 0;
	White_Knights = 0;
	White_Pawns = 0;
	Black_Queens = 0;
	Black_Rooks = 0;
	Black_Bishops = 0;
	Black_Knights = 0;
	Black_Pawns = 0;
	EP_Square = 0;
	Prev_EP_Square = 0;
	numlegalmoves = 0;
	WhiteCanCastleK = false;
	WhiteCanCastleQ = false;
	BlackCanCastleK = false;
	BlackCanCastleQ = false;
	PrevWhiteCanCastleK = false;
	PrevWhiteCanCastleQ = false;
	PrevBlackCanCastleK = false;
	PrevBlackCanCastleQ = false;
	Current_Turn = true;
	hashkey = 0;
	numlegalmoves = 0;
	WhiteAttacks = 0;
	BlackAttacks = 0;
}
Position::~Position() {}
Position::Position(Position* position) {
	Current_Turn = position->Current_Turn;
	White_Pieces = position->White_Pieces;
	Black_Pieces = position->Black_Pieces;
	White_King = position->White_King;
	Black_King = position->Black_King;
	White_Queens = position->White_Queens;
	White_Rooks = position->White_Rooks;
	White_Bishops = position->White_Bishops;
	White_Knights = position->White_Knights;
	White_Pawns = position->White_Pawns;
	Black_Queens = position->Black_Queens;
	Black_Rooks = position->Black_Rooks;
	Black_Bishops = position->Black_Bishops;
	Black_Knights = position->Black_Knights;
	Black_Pawns = position->Black_Pawns;
	EP_Square = position->EP_Square;
	Prev_EP_Square = position->Prev_EP_Square;
	WhiteCanCastleK = position->WhiteCanCastleK;
	WhiteCanCastleQ = position->WhiteCanCastleQ;
	BlackCanCastleK = position->BlackCanCastleK;
	BlackCanCastleQ = position->BlackCanCastleQ;
	PrevWhiteCanCastleK = position->PrevWhiteCanCastleK;
	PrevWhiteCanCastleQ = position->PrevWhiteCanCastleQ;
	PrevBlackCanCastleK = position->PrevBlackCanCastleK;
	PrevBlackCanCastleQ = position->PrevBlackCanCastleQ;
	hashkey = position->hashkey;
	numlegalmoves = 0;
	WhiteAttacks = position->WhiteAttacks;
	BlackAttacks = position->BlackAttacks;
}
void Position::Reset() {
	White_Pieces = 65535;
	Black_Pieces = 18446462598732840960ULL;
	White_King = 16;
	Black_King = 1152921504606846976;
	White_Queens = 8;
	White_Rooks = 129;
	White_Bishops = 36;
	White_Knights = 66;
	White_Pawns = 65280;
	Black_Queens = 576460752303423488;
	Black_Rooks = 9295429630892703744ULL;
	Black_Bishops = 2594073385365405696;
	Black_Knights = 4755801206503243776;
	Black_Pawns = 71776119061217280;
	EP_Square = 0;
	Prev_EP_Square = 0;
	WhiteCanCastleK = true;
	WhiteCanCastleQ = true;
	BlackCanCastleK = true;
	BlackCanCastleQ = true;
	PrevWhiteCanCastleK = false;
	PrevWhiteCanCastleQ = false;
	PrevBlackCanCastleK = false;
	PrevBlackCanCastleQ = false;
	Current_Turn = true;
	numlegalmoves = 0;
	hashkey = 18446462598732906494ULL;
	Get_White_Attacks();
	Get_Black_Attacks();
}
Bitboard* Position::Get_Bitboard_From_Piece(Piece p) {
	switch (p) {
	case WP:
		return &White_Pawns;
		break;
	case WN:
		return &White_Knights;
		break;
	case WB:
		return &White_Bishops;
		break;
	case WR:
		return &White_Rooks;
		break;
	case WQ:
		return &White_Queens;
		break;
	case WK:
		return &White_King;
		break;
	case BP:
		return &Black_Pawns;
		break;
	case BN:
		return &Black_Knights;
		break;
	case BB:
		return &Black_Bishops;
		break;
	case BR:
		return &Black_Rooks;
		break;
	case BQ:
		return &Black_Queens;
		break;
	case BK:
		return &Black_King;
		break;
	default:
		return NULL;
	}
}
Piece Position::Get_Piece_From_Bitboard(Bitboard b) {
	if (b & White_Pawns)
		return WP;
	if (b & White_Knights)
		return WN;
	if (b & White_Bishops)
		return WB;
	if (b & White_Rooks)
		return WR;
	if (b & White_Queens)
		return WQ;
	if (b & White_King)
		return WK;
	if (b & Black_Pawns)
		return BP;
	if (b & Black_Knights)
		return BN;
	if (b & Black_Bishops)
		return BB;
	if (b & Black_Rooks)
		return BR;
	if (b & Black_Queens)
		return BQ;
	if (b & Black_King)
		return BK;
	return NONE;
}
void Position::Make_Move(Move m) {
	// any time we are executing a move, the en passant square is empty
	Prev_EP_Square = EP_Square;
	EP_Square = 0;

	// copy the castling rights before making a move
	PrevWhiteCanCastleK = WhiteCanCastleK;
	PrevWhiteCanCastleQ = WhiteCanCastleQ;
	PrevBlackCanCastleK = BlackCanCastleK;
	PrevBlackCanCastleQ = BlackCanCastleQ;

	if (m.Castling) {
		if (m.To & 64) {
			WhiteCanCastleK = false;
			White_Rooks |= 32;
			White_Rooks ^= 128;
			White_King |= 64;
			White_King ^= 16;
			White_Pieces ^= 144;
			White_Pieces |= 96;
		}
		else if (m.To & 4611686018427387904) {
			BlackCanCastleK = false;
			Black_Rooks |= 2305843009213693952;
			Black_Rooks ^= 9223372036854775808ULL;
			Black_King |= 4611686018427387904;
			Black_King ^= 1152921504606846976;
			Black_Pieces ^= 10376293541461622784ULL;
			Black_Pieces |= 6917529027641081856;
		}
		else if (m.To & 4) {
			WhiteCanCastleQ = false;
			White_Rooks |= 8;
			White_Rooks ^= 1;
			White_King |= 4;
			White_King ^= 16;
			White_Pieces ^= 17;
			White_Pieces |= 12;
		}
		else if (m.To & 288230376151711744) {
			BlackCanCastleQ = false;
			Black_Rooks |= 576460752303423488;
			Black_Rooks ^= 72057594037927936;
			Black_King |= 288230376151711744;
			Black_King ^= 1152921504606846976;
			Black_Pieces ^= 1224979098644774912;
			Black_Pieces |= 864691128455135232;
		}
		Current_Turn ^= 1;
		return;
	}

	// here we have to handle when to remove castling rights
	// else if, since when we castle we remove the appropriate accesses already
	// these are for non castling king or rook moves
	else if (m.From & White_King) {
		WhiteCanCastleK = false;
		WhiteCanCastleQ = false;
	}

	else if (m.From & Black_King) {
		BlackCanCastleK = false;
		BlackCanCastleQ = false;
	}

	// if we're moving anything from a1 it means the rook is gone. if we're capturing to a1 the rook is also gone
	if ((m.From & 1) || (m.To & 1))
		WhiteCanCastleQ = false;

	if ((m.From & 128) || (m.To & 128))
		WhiteCanCastleK = false;

	if ((m.From & 72057594037927936) || (m.To & 72057594037927936))
		BlackCanCastleQ = false;

	if ((m.From & 9223372036854775808) || (m.To & 9223372036854775808))
		BlackCanCastleK = false;

	Bitboard* bb = Get_Bitboard_From_Piece(m.P);
	*bb ^= m.From;
	*bb |= m.To;
	if (m.P > WK) {
		Black_Pieces |= m.To;
		Black_Pieces ^= m.From;
	}
	else {
		White_Pieces |= m.To;
		White_Pieces ^= m.From;
	}

	// if this is a capture but not EP
	if ((m.C != NONE) && !m.EP) {
		bb = Get_Bitboard_From_Piece(m.C);
		*bb ^= m.To;
		if (m.C > WK)
			Black_Pieces ^= m.To;
		else
			White_Pieces ^= m.To;
	}
	if (m.Promotion) {
		bb = Get_Bitboard_From_Piece(m.PromotionType);
		*bb |= m.To;
		bb = Get_Bitboard_From_Piece(m.P);
		*bb ^= m.To;
	}

	// if the move is an en passant move
	if (m.EP) {
		// if we're capturing a black pawn
		if (m.C > WK) {
			Black_Pieces ^= m.To >> 8;
			Black_Pawns ^= m.To >> 8;
		}
		// if we're capturing a white pawn
		else {
			White_Pieces ^= m.To << 8;
			White_Pawns ^= m.To << 8;
		}
	}

	// if the move is a two square pawn move, we need to set the EP square to behind it
	// if a white pawn is moving from the second rank to the fourth...
	if ((m.P == WP) && (m.From & 65280) && (m.To & 4278190080)) {
		EP_Square = m.From << 8;
	}

	// if a black pawn is moving from the seventh rank to the fifth...
	else if ((m.P == BP) && (m.From & 71776119061217280) && (m.To & 1095216660480)) {
		EP_Square = m.From >> 8;
	}

	Current_Turn ^= 1;
}
void Position::Undo_Move(Move m) {
	// any time we are undoing a move, the en passant square is empty
	EP_Square = Prev_EP_Square;
	Prev_EP_Square = 0;

	// restore castling rights from previous position
	WhiteCanCastleK = PrevWhiteCanCastleK;
	WhiteCanCastleQ = PrevWhiteCanCastleQ;
	BlackCanCastleK = PrevBlackCanCastleK;
	BlackCanCastleQ = PrevBlackCanCastleQ;

	if (m.Castling) {
		if (m.To & 64) {
			WhiteCanCastleK = true;
			White_Rooks ^= 32;
			White_Rooks |= 128;
			White_King ^= 64;
			White_King |= 16;
			White_Pieces |= 144;
			White_Pieces ^= 96;
		}
		else if (m.To & 4611686018427387904) {
			BlackCanCastleK = true;
			Black_Rooks ^= 2305843009213693952;
			Black_Rooks |= 9223372036854775808ULL;
			Black_King ^= 4611686018427387904;
			Black_King |= 1152921504606846976;
			Black_Pieces |= 10376293541461622784ULL;
			Black_Pieces ^= 6917529027641081856;
		}
		else if (m.To & 4) {
			WhiteCanCastleQ = true;
			White_Rooks ^= 8;
			White_Rooks |= 1;
			White_King ^= 4;
			White_King |= 16;
			White_Pieces |= 17;
			White_Pieces ^= 12;
		}
		else if (m.To & 288230376151711744) {
			BlackCanCastleQ = true;
			Black_Rooks ^= 576460752303423488;
			Black_Rooks |= 72057594037927936;
			Black_King ^= 288230376151711744;
			Black_King |= 1152921504606846976;
			Black_Pieces |= 1224979098644774912;
			Black_Pieces ^= 864691128455135232;
		}
		Current_Turn ^= 1;
		return;
	}
	Bitboard* bb = Get_Bitboard_From_Piece(m.P);
	*bb |= m.From;
	*bb ^= m.To;
	if (m.P > WK) {
		Black_Pieces ^= m.To;
		Black_Pieces |= m.From;
	}
	else {
		White_Pieces ^= m.To;
		White_Pieces |= m.From;
	}

	// if this is a capture but not EP
	if ((m.C != NONE) && !m.EP) {
		bb = Get_Bitboard_From_Piece(m.C);
		*bb |= m.To;
		if (m.C > WK)
			Black_Pieces |= m.To;
		else
			White_Pieces |= m.To;
	}
	if (m.Promotion) {
		bb = Get_Bitboard_From_Piece(m.PromotionType);
		*bb ^= m.To;
		bb = Get_Bitboard_From_Piece(m.P);
		*bb ^= m.To;
	}

	// if the move is an en passant move
	if (m.EP) {
		// if we captured a black pawn
		if (m.C > WK) {
			Black_Pieces |= m.To >> 8;
			Black_Pawns |= m.To >> 8;
		}
		// if we captured a white pawn
		else {
			White_Pieces |= m.To << 8;
			White_Pawns |= m.To << 8;
		}

		// if we're undoing an EP move, the square we moved to will still be an EP square
		EP_Square = m.To;
	}

	Current_Turn ^= 1;
}
int Get_Cp_Value(Piece piece) {
	if (piece == WP || piece == BP)
		return 100;
	if (piece == WN || piece == BN)
		return 300;
	if (piece == WB || piece == BB)
		return 300;
	if (piece == WR || piece == BR)
		return 500;
	if (piece == WQ || piece == BQ)
		return 800;
	if (piece == WK || piece == BK)
		return MATE;
	return 0;
}
Bitboard Position::Get_White_Attacks() {
	WhiteAttacks = 0;
	Bitboard b = White_Pawns;
	b |= A_Pawn_Mask;
	b ^= A_Pawn_Mask;
	WhiteAttacks |= b << 7;
	b = Black_Pawns;
	b |= H_Pawn_Mask;
	b ^= H_Pawn_Mask;
	WhiteAttacks |= b << 9;
	b = White_Knights;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Knight_Lookup_Table[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_Bishops;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Bmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = White_Rooks;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Rmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = White_Queens;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Qmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = White_King;
	int lookup = lsb(b);
	WhiteAttacks |= King_Lookup_Table[lookup];
	return WhiteAttacks;
}
Bitboard Position::GetLeastWhiteAttacker(Bitboard board) {
	WhiteAttacks = 0;
	Bitboard b = White_Pawns;
	b |= A_Pawn_Mask;
	b ^= A_Pawn_Mask;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= b << 7;
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_Pawns;
	b |= H_Pawn_Mask;
	b ^= H_Pawn_Mask;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= b << 9;
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	if (WhiteAttacks & board) return WhiteAttacks & board;
	b = White_Knights;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Knight_Lookup_Table[lookup];
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_Bishops;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Bmagic(lookup, White_Pieces | Black_Pieces);
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_Rooks;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Rmagic(lookup, White_Pieces | Black_Pieces);
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_Queens;
	while (b) {
		int lookup = lsb(b);
		WhiteAttacks |= Qmagic(lookup, White_Pieces | Black_Pieces);
		if (WhiteAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = White_King;
	int lookup = lsb(b);
	WhiteAttacks |= King_Lookup_Table[lookup];
	if (WhiteAttacks & board) return GeneralBoard[lookup];
	return 0;
}
Bitboard Position::Get_Black_Attacks() {
	BlackAttacks = 0;
	Bitboard b = Black_Pawns;
	b |= A_Pawn_Mask;
	b ^= A_Pawn_Mask;
	BlackAttacks |= b >> 9;
	b = Black_Pawns;
	b |= H_Pawn_Mask;
	b ^= H_Pawn_Mask;
	BlackAttacks |= b >> 7;
	b = Black_Knights;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Knight_Lookup_Table[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Bishops;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Bmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = Black_Rooks;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Rmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = Black_Queens;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Qmagic(lookup, White_Pieces | Black_Pieces);
		b ^= GeneralBoard[lookup];
	}
	b = Black_King;
	int lookup = lsb(b);
	BlackAttacks |= King_Lookup_Table[lookup];
	return BlackAttacks;
}
int Position::Non_Pawn_Material() {
	return __popcnt64((Current_Turn ? White_Knights | White_Bishops | White_Rooks | White_Queens :
		Black_Knights | Black_Bishops | Black_Rooks | Black_Queens));
}
Bitboard Position::GetLeastBlackAttacker(Bitboard board) {
	BlackAttacks = 0;
	Bitboard b = Black_Pawns;
	b |= A_Pawn_Mask;
	b ^= A_Pawn_Mask;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= b >> 9;
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Pawns;
	b |= H_Pawn_Mask;
	b ^= H_Pawn_Mask;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= b >> 7;
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Knights;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Knight_Lookup_Table[lookup];
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Bishops;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Bmagic(lookup, White_Pieces | Black_Pieces);
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Rooks;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Rmagic(lookup, White_Pieces | Black_Pieces);
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_Queens;
	while (b) {
		int lookup = lsb(b);
		BlackAttacks |= Qmagic(lookup, White_Pieces | Black_Pieces);
		if (BlackAttacks & board) return GeneralBoard[lookup];
		b ^= GeneralBoard[lookup];
	}
	b = Black_King;
	int lookup = lsb(b);
	BlackAttacks |= King_Lookup_Table[lookup];
	if (BlackAttacks & board) return GeneralBoard[lookup];
	return 0;
}

string Position::GetTextBoard() const {
	string result = "";

	std::vector<string> ranks;
	string temp = "";

	for (int i = 0; i < 64; i++) {
		if (i == 0 || i % 8 == 0)
			temp += "|";

		// white pieces
		if (White_Pawns & GeneralBoard[i])
			temp += "P";
		else if (White_Rooks & GeneralBoard[i])
			temp += "R";
		else if (White_Knights & GeneralBoard[i])
			temp += "N";
		else if (White_Bishops & GeneralBoard[i])
			temp += "B";
		else if (White_Queens & GeneralBoard[i])
			temp += "Q";
		else if (White_King & GeneralBoard[i])
			temp += "K";

		// black pieces
		else if (Black_Pawns & GeneralBoard[i])
			temp += "p";
		else if (Black_Rooks & GeneralBoard[i])
			temp += "r";
		else if (Black_Knights & GeneralBoard[i])
			temp += "n";
		else if (Black_Bishops & GeneralBoard[i])
			temp += "b";
		else if (Black_Queens & GeneralBoard[i])
			temp += "q";
		else if (Black_King & GeneralBoard[i])
			temp += "k";
		else
			temp += " ";

		temp += "|";

		if ((i + 1) % 8 == 0) {
			temp += "\n";
			ranks.push_back(temp);
			temp = "";
		}
	}

	ranks.push_back(" _ _ _ _ _ _ _ _\n");

	reverse(ranks.begin(), ranks.end());

	for (string s : ranks)
		result += s;

	return result;
}
