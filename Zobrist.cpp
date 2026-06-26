#include "Bitboard.h"
#include "Zobrist.h"
Bitboard zobrist[64][12][2];
Bitboard rand64() {
	static Bitboard next = 1;
	next = next * 1103515245 + 12345;
	return next;
}
void Init_Zobrist() {
	srand(time(0));
	for (int i = 0; i < 64; i++) {
		zobrist[i][0][0] = rand64();
		for (int j = 0; j < 6; j++) {
			zobrist[i][j][0] = rand64();
			for (int h = 0; h < 2; h++) {
				zobrist[i][j][h] = rand64();
			}
		}
	}
}
Bitboard Get_Current_Hash_Key(Position* position) {
	Bitboard key = 0;
	key ^= position->White_Pawns;
	key ^= position->White_Knights;
	key ^= position->White_Bishops;
	key ^= position->White_Rooks;
	key ^= position->White_Queens;
	key ^= position->White_King;
	key ^= position->Black_Pawns;
	key ^= position->Black_Knights;
	key ^= position->Black_Bishops;
	key ^= position->Black_Rooks;
	key ^= position->Black_Queens;
	key ^= position->Black_King;

	key ^= int(position->Current_Turn);

	return key;
}
