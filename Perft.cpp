#include "Perft.h"
#include "Search.h"
#include "UCI.h"

Bitboard Root_Perft(int depth, bool PrintMoveList) {
	if (depth == 0) return 0;

	if (PrintMoveList)
		cout << "\n";

	Bitboard nodes = 0;
	pos.Current_Turn ? Generate_White_Moves(false, pos) : Generate_Black_Moves(false, pos);
	for (int i = 0; i < pos.numlegalmoves; i++) {
		pos.Make_Move(pos.LegalMoves[i]);

		if (PrintMoveList)
			Print_Move_List(i);

		int f = nodes;
		nodes += Perft(pos, depth - 1);

		if (PrintMoveList)
			cout << nodes - f << endl;

		pos.Undo_Move(pos.LegalMoves[i]);
	}
	return nodes;

}
int Perft(Position& posit, int depth) {
	if (depth == 1) {
		Position position(posit);
		position.Current_Turn ? Generate_White_Moves(false, position) : Generate_Black_Moves(false, position);
		return position.numlegalmoves;
	}

	if (depth == 0) return 1;

	Position position(posit);
	Bitboard nodes = 0;
	position.Current_Turn ? Generate_White_Moves(false, position) : Generate_Black_Moves(false, position);
	for (int i = 0; i < position.numlegalmoves; i++) {
		position.Make_Move(position.LegalMoves[i]);
		nodes += Perft(position, depth - 1);
		position.Undo_Move(position.LegalMoves[i]);
	}
	return nodes;
}

void Print_Move_List(int i) {
	for (int h = 0; h < 64; h++) {
		if (GeneralBoard[h] & pos.LegalMoves[i].From) {
			cout << PlayerMoves[h];
		}
	}

	for (int h = 0; h < 64; h++) {
		if (GeneralBoard[h] & pos.LegalMoves[i].To) {
			cout << PlayerMoves[h];
			if (pos.LegalMoves[i].Promotion) {
				switch (pos.LegalMoves[i].PromotionType) {
				case WN:
					cout << "n";
					break;
				case WB:
					cout << "b";
					break;
				case WR:
					cout << "r";
					break;
				case WQ:
					cout << "q";
					break;
				case BN:
					cout << "n";
					break;
				case BB:
					cout << "b";
					break;
				case BR:
					cout << "r";
					break;
				case BQ:
					cout << "q";
					break;
				default:
					break;
				}
			}
			cout << ": ";
		}
	}
}

void PerftSuite() {
	vector<string> TestPositions;
	vector<int> TestPositionDepths;
	vector<Bitboard> TestPositionNodes;

	/////////////////////////////////////////////////////////////////////////////////
	// Test positions - Original suite
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1134888);
	TestPositions.push_back("8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1015133);
	TestPositions.push_back("8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1440467);
	TestPositions.push_back("5k2/8/8/8/8/8/8/4K2R w K - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(661072);
	TestPositions.push_back("3k4/8/8/8/8/8/8/R3K3 w Q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(803711);
	TestPositions.push_back("r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1");
	TestPositionDepths.push_back(4);
	TestPositionNodes.push_back(1274206);
	TestPositions.push_back("r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1");
	TestPositionDepths.push_back(4);
	TestPositionNodes.push_back(1720476);
	TestPositions.push_back("2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3821001);
	TestPositions.push_back("8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1");
	TestPositionDepths.push_back(5);
	TestPositionNodes.push_back(1004658);
	TestPositions.push_back("4k3/1P6/8/8/8/8/K7/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(217342);
	TestPositions.push_back("8/P1k5/K7/8/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(92683);
	TestPositions.push_back("K1k5/8/P7/8/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2217);
	TestPositions.push_back("8/k1P5/8/1K6/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(7);
	TestPositionNodes.push_back(567584);
	TestPositions.push_back("8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1");
	TestPositionDepths.push_back(4);
	TestPositionNodes.push_back(23527);

	/////////////////////////////////////////////////////////////////////////////////
	// Extended test positions - Standard positions
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	TestPositionDepths.push_back(5);
	TestPositionNodes.push_back(4865609);
	TestPositions.push_back("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
	TestPositionDepths.push_back(4);
	TestPositionNodes.push_back(4085603);

	/////////////////////////////////////////////////////////////////////////////////
	// Castling rights tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("4k3/8/8/8/8/8/8/4K2R w K - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(764643);
	TestPositions.push_back("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(846648);
	TestPositions.push_back("4k2r/8/8/8/8/8/8/4K3 w k - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(899442);
	TestPositions.push_back("r3k3/8/8/8/8/8/8/4K3 w q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1001523);
	TestPositions.push_back("4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2788982);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/4K3 w kq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3517770);
	TestPositions.push_back("8/8/8/8/8/8/6k1/4K2R w K - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(185867);
	TestPositions.push_back("8/8/8/8/8/8/1k6/R3K3 w Q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(413018);
	TestPositions.push_back("4k2r/6K1/8/8/8/8/8/8 w k - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(179869);
	TestPositions.push_back("r3k3/1K6/8/8/8/8/8/8 w q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(367724);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(179862938);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/1R2K2R w Kkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(195629489);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/2R1K2R w Kkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(184411439);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/R3K1R1 w Qkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(189224276);
	TestPositions.push_back("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(198328929);
	TestPositions.push_back("2r1k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(185959088);
	TestPositions.push_back("r3k1r1/8/8/8/8/8/8/R3K2R w KQq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(190755813);
	TestPositions.push_back("4k3/8/8/8/8/8/8/4K2R b K - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(899442);
	TestPositions.push_back("4k3/8/8/8/8/8/8/R3K3 b Q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1001523);
	TestPositions.push_back("4k2r/8/8/8/8/8/8/4K3 b k - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(764643);
	TestPositions.push_back("r3k3/8/8/8/8/8/8/4K3 b q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(846648);
	TestPositions.push_back("4k3/8/8/8/8/8/8/R3K2R b KQ - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3517770);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/4K3 b kq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2788982);
	TestPositions.push_back("8/8/8/8/8/8/6k1/4K2R b K - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(179869);
	TestPositions.push_back("8/8/8/8/8/8/1k6/R3K3 b Q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(367724);
	TestPositions.push_back("4k2r/6K1/8/8/8/8/8/8 b k - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(185867);
	TestPositions.push_back("r3k3/1K6/8/8/8/8/8/8 b q - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(413018);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(179862938);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(198328929);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/2R1K2R b Kkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(185959088);
	TestPositions.push_back("r3k2r/8/8/8/8/8/8/R3K1R1 b Qkq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(190755813);
	TestPositions.push_back("1r2k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(195629489);
	TestPositions.push_back("2r1k2r/8/8/8/8/8/8/R3K2R b KQk - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(184411439);
	TestPositions.push_back("r3k1r1/8/8/8/8/8/8/R3K2R b KQq - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(189224276);

	/////////////////////////////////////////////////////////////////////////////////
	// Knight move tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("8/1n4N1/2k5/8/8/5K2/1N4n1/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(8107539);
	TestPositions.push_back("8/1k6/8/5N2/8/4n3/8/2K5 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2594412);
	TestPositions.push_back("8/8/4k3/3Nn3/3nN3/4K3/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(19870403);
	TestPositions.push_back("K7/8/2n5/1n6/8/8/8/k6N w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(588695);
	TestPositions.push_back("k7/8/2N5/1N6/8/8/8/K6n w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(688780);
	TestPositions.push_back("8/1n4N1/2k5/8/8/5K2/1N4n1/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(8503277);
	TestPositions.push_back("8/1k6/8/5N2/8/4n3/8/2K5 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3147566);
	TestPositions.push_back("8/8/3K4/3Nn3/3nN3/4k3/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(4405103);
	TestPositions.push_back("K7/8/2n5/1n6/8/8/8/k6N b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(688780);
	TestPositions.push_back("k7/8/2N5/1N6/8/8/8/K6n b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(588695);

	/////////////////////////////////////////////////////////////////////////////////
	// Bishop move tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("B6b/8/8/8/2K5/4k3/8/b6B w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(22823890);
	TestPositions.push_back("8/8/1B6/7b/7k/8/2B1b3/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(28861171);
	TestPositions.push_back("k7/B7/1B6/1B6/8/8/8/K6b w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(7881673);
	TestPositions.push_back("K7/b7/1b6/1b6/8/8/8/k6B w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(7382896);
	TestPositions.push_back("B6b/8/8/8/2K5/5k2/8/b6B b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(9250746);
	TestPositions.push_back("8/8/1B6/7b/7k/8/2B1b3/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29027891);
	TestPositions.push_back("k7/B7/1B6/1B6/8/8/8/K6b b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(7382896);
	TestPositions.push_back("K7/b7/1b6/1b6/8/8/8/k6B b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(7881673);

	/////////////////////////////////////////////////////////////////////////////////
	// Rook move tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("7k/RR6/8/8/8/8/rr6/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(44956585);
	TestPositions.push_back("R6r/8/8/2K5/5k2/8/8/r6R w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(525169084);
	TestPositions.push_back("7k/RR6/8/8/8/8/rr6/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(44956585);
	TestPositions.push_back("R6r/8/8/2K5/5k2/8/8/r6R b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(524966748);

	/////////////////////////////////////////////////////////////////////////////////
	// Queen move tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("6kq/8/8/8/8/8/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(391507);
	TestPositions.push_back("6KQ/8/8/8/8/8/8/7k b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(391507);
	TestPositions.push_back("K7/8/8/3Q4/4q3/8/8/7k w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3370175);
	TestPositions.push_back("6qk/8/8/8/8/8/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(419369);
	TestPositions.push_back("K7/8/8/3Q4/4q3/8/8/7k b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(3370175);

	/////////////////////////////////////////////////////////////////////////////////
	// Pawn move tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("8/8/8/8/8/K7/P7/k7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(6249);
	TestPositions.push_back("8/8/8/8/8/7K/7P/7k w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(6249);
	TestPositions.push_back("K7/p7/k7/8/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2343);
	TestPositions.push_back("7K/7p/7k/8/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2343);
	TestPositions.push_back("8/2k1p3/3pP3/3P2K1/8/8/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(34834);
	TestPositions.push_back("8/8/8/8/8/K7/P7/k7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2343);
	TestPositions.push_back("8/8/8/8/8/7K/7P/7k b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(2343);
	TestPositions.push_back("K7/p7/k7/8/8/8/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(6249);
	TestPositions.push_back("7K/7p/7k/8/8/8/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(6249);
	TestPositions.push_back("8/2k1p3/3pP3/3P2K1/8/8/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(34822);
	TestPositions.push_back("8/8/8/8/8/4k3/4P3/4K3 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(11848);
	TestPositions.push_back("4k3/4p3/4K3/8/8/8/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(11848);
	TestPositions.push_back("8/8/7k/7p/7P/7K/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(10724);
	TestPositions.push_back("8/8/k7/p7/P7/K7/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(10724);
	TestPositions.push_back("8/8/3k4/3p4/3P4/3K4/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(53138);
	TestPositions.push_back("8/3k4/3p4/8/3P4/3K4/8/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(157093);
	TestPositions.push_back("8/8/3k4/3p4/8/3P4/3K4/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(158065);
	TestPositions.push_back("k7/8/3p4/8/3P4/8/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(20960);
	TestPositions.push_back("8/8/7k/7p/7P/7K/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(10724);
	TestPositions.push_back("8/8/k7/p7/P7/K7/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(10724);
	TestPositions.push_back("8/8/3k4/3p4/3P4/3K4/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(53138);
	TestPositions.push_back("8/3k4/3p4/8/3P4/3K4/8/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(158065);
	TestPositions.push_back("8/8/3k4/3p4/8/3P4/3K4/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(157093);
	TestPositions.push_back("k7/8/3p4/8/3P4/8/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(21104);
	TestPositions.push_back("7k/3p4/8/8/3P4/8/8/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(32191);
	TestPositions.push_back("7k/8/8/3p4/8/8/3P4/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(30980);
	TestPositions.push_back("k7/8/8/7p/6P1/8/8/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("k7/8/7p/8/8/6P1/8/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/8/8/6p1/7P/8/8/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("k7/8/6p1/8/8/7P/8/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/8/8/3p4/4p3/8/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(22886);
	TestPositions.push_back("k7/8/3p4/8/8/4P3/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(28662);
	TestPositions.push_back("7k/3p4/8/8/3P4/8/8/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(32167);
	TestPositions.push_back("7k/8/8/3p4/8/8/3P4/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(30749);
	TestPositions.push_back("k7/8/8/7p/6P1/8/8/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("k7/8/7p/8/8/6P1/8/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/8/8/6p1/7P/8/8/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("k7/8/6p1/8/8/7P/8/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/8/8/3p4/4p3/8/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(22579);
	TestPositions.push_back("k7/8/3p4/8/8/4P3/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(28662);
	TestPositions.push_back("7k/8/8/p7/1P6/8/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("7k/8/p7/8/8/1P6/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("7k/8/8/1p6/P7/8/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("7k/8/1p6/8/8/P7/8/7K w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/7p/8/8/8/8/6P1/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(55338);
	TestPositions.push_back("k7/6p1/8/8/8/8/7P/K7 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(55338);
	TestPositions.push_back("3k4/3pp3/8/8/8/8/3PP3/3K4 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(199002);
	TestPositions.push_back("7k/8/8/p7/1P6/8/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("7k/8/p7/8/8/1P6/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("7k/8/8/1p6/P7/8/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(41874);
	TestPositions.push_back("7k/8/1p6/8/8/P7/8/7K b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(29679);
	TestPositions.push_back("k7/7p/8/8/8/8/6P1/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(55338);
	TestPositions.push_back("k7/6p1/8/8/8/8/7P/K7 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(55338);
	TestPositions.push_back("3k4/3pp3/8/8/8/8/3PP3/3K4 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(199002);

	/////////////////////////////////////////////////////////////////////////////////
	// Promotion tests
	/////////////////////////////////////////////////////////////////////////////////
	TestPositions.push_back("8/Pk6/8/8/8/8/6Kp/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1030499);
	TestPositions.push_back("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(37665329);
	TestPositions.push_back("8/PPPk4/8/8/8/8/4Kppp/8 w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(28859283);
	TestPositions.push_back("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(71179139);
	TestPositions.push_back("8/Pk6/8/8/8/8/6Kp/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(1030499);
	TestPositions.push_back("n1n5/1Pk5/8/8/8/8/5Kp1/5N1N b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(37665329);
	TestPositions.push_back("8/PPPk4/8/8/8/8/4Kppp/8 b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(28859283);
	TestPositions.push_back("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");
	TestPositionDepths.push_back(6);
	TestPositionNodes.push_back(71179139);


	/////////////////////////////////////////////////////////////////////////////////
	//
	/////////////////////////////////////////////////////////////////////////////////

	int FailCount = 0;

	cout << "\n\nStarting the perft test suite:\n" << endl;

	for (int i = 0; i < TestPositions.size(); i++) {
		cout << "Position " << i + 1 << "/" << TestPositions.size() << ": " << TestPositions[i] << endl;

		Parse_Fen(TestPositions[i]);

		Bitboard nodes = Root_Perft(TestPositionDepths[i], false);

		cout << "Expected: " << TestPositionNodes[i] << ", got: " << nodes;

		// print the right amount of dashes so the pass/fails all line up
		for (int j = 0; j < 50 - (to_string(nodes).length() * 2); j++) {
			cout << "-";
		}

		cout << ">   ";

		if (TestPositionNodes[i] == nodes)
			cout << "PASS\n\n" << endl;
		else {
			cout << "FAIL\n\n" << endl;
			FailCount++;
		}
	}

	cout << "============================================\n" << endl;
	cout << "Perft suite results: " << TestPositions.size() - FailCount << " passed, " << FailCount << " failed\n" << endl;
}
