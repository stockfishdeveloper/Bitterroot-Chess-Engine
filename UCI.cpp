#include "Bitboard.h"
#include "UCI.h"
#include <limits>
#include <thread> //For multithreading--must be using C++11 compiler
#include "Thread.h"//Threading header file
#include "Search.h"
#include "Perft.h"
#include "Zobrist.h"
#include "MoveGen.h"
#include "Util.h"
#include "TransTable.h"
#include "Position.h"
#include "Eval.h"
#include "CounterMove.h"
#include "Pawns.h"
#include "Tablebase.h"

using namespace std;
int CheckUci();
string UciCommand;
string NalimovPath = "";
string SyzygyPath = "";
Bitboard Current_Rank = 72057594037927936;
ofstream Log("Log.txt");//For writing to a text file
int wtime = 0;
int btime = 0;
int winc = 0;
int binc = 0;
int Time_Usage = 0;
int MultiPV = 1;
LINE PVline;

int CheckUci() {
	bool Is_Fen = false;

	while (cin >> UciCommand) {
		Log << ">> " << UciCommand << endl;
		if (UciCommand == "uci") {
			string Uci_Out = "id name " + Engine_Info() + "\n" + "id author David Cimbalista\n" + "option name TimePerMove type spin default 3 min 1 max 5\n" + "option name Nalimov type string\n" + "option name Syzygy type string\n" + "option name MultiPV type spin default 1 min 1 max 500\n" + "uciok\n";
			cout << Uci_Out;
			Log << Uci_Out;
		}
		else if (UciCommand == "isready") {
			cout << "readyok" << endl;
			Log << "readyok" << endl;
		}

		else if (UciCommand == "quit")
			exit(0);//Exit the program if called to quit

		else if (UciCommand == "stop")
			Search::STOP_SEARCHING_NOW = true;

		else if (UciCommand == "ucinewgame") {
			Search::Searching = false;
			Search::STOP_SEARCHING_NOW = false;
			Search::Nodes = 0;
		}

		else if (UciCommand == "startpos") {
			Startpos();
		}
		else if (UciCommand == "print") {
			cout << pos.GetTextBoard();
		}
		else if (UciCommand == "mate") {
			int depth = 0;
			cin >> depth;
			int Mate_Search = Search::MateSearch(&pos, -1, 1, depth);
			cout << Mate_Search << endl;
			Log << Mate_Search << endl;
		}
		else if (UciCommand == "see") {
			pos.Current_Turn ? Generate_White_Moves(false, pos) : Generate_Black_Moves(false, pos);
			for (int i = 0; i < pos.numlegalmoves; i++) {
				if (pos.LegalMoves[i].C != NONE) {
					pos.LegalMoves[i].Output();
					cout << " : ";
					cout << Search::SEE(&pos, pos.LegalMoves[i].To) << endl;
				}
			}
		}
		else if (UciCommand == "bench") {
			Benchmark();
		}
		else if (UciCommand == "eval") {
			cout << Eval::Evaluate_Position(&pos) << endl;
		}
		else if (UciCommand == "perft") {
			Timer timer;
			timer.Start_Clock();
			int depth = 0;
			cin >> depth;
			Bitboard nodes = Root_Perft(depth, true);
			cout << "\n===========================\n";
			cout << "Total time (ms) : " << timer.Get_Time() << "\n";
			cout << "Nodes searched  : " << nodes << "\n";
			cout << "Nodes/second    : " << (nodes / (timer.Get_Time() + 1) * 1000) << "\n";
		}

		else if (UciCommand == "perftsuite") {
			PerftSuite();
		}

		else if (UciCommand == "perftspeed") {
			int Total_MS = 0;

			for (int i = 0; i < 100; i++) {
				Startpos();

				cout << "\nRunning perft iteration #" << i + 1 << " of 100\n\n";

				Timer timer;
				timer.Start_Clock();
				int depth = 6;
				Bitboard nodes = Root_Perft(depth, true);
				cout << "\n===========================\n";
				cout << "Total time (ms) : " << timer.Get_Time() << "\n";
				cout << "Nodes searched  : " << nodes << "\n";
				cout << "Nodes/second    : " << (nodes / (timer.Get_Time() + 1) * 1000) << "\n";

				Total_MS += timer.Get_Time();
			}

			cout << "\n===========================\n";
			cout << "\n===========================\n";
			cout << "Average perft 6 execution time over 100 runs: " << Total_MS / 100 << endl;
		}

		else if (Is_Fen) {
			string temp = UciCommand + " ";
			string a = " ";
			for (int i = 0; i < 5; i++) {
				cin >> a;
				Log << ">> " << a << endl;
				if (i != 4)
					a += " ";
				temp += a;
			}
			Parse_Fen(temp);
			Is_Fen = false;
		}

		else if (UciCommand == "fen")
			Is_Fen = true;

		else if (UciCommand == "setoption") {
			string name = "";
			cin >> name;
			Log << ">> " << name << endl;
			string optionname = "";
			cin >> optionname;
			Log << ">> " << optionname << endl;
			string valuestring = "";
			cin >> valuestring;
			Log << ">> " << valuestring << endl;
			string paramvalue = "";
			cin >> paramvalue;
			Log << ">> " << paramvalue << endl;
			if (optionname == "TimePerMove") {
				Time_Usage = stoi(paramvalue);
			}
			else if (optionname == "MultiPV") {
				MultiPV = stoi(paramvalue);
			}
			else if (optionname == "Nalimov") {
				NalimovPath = paramvalue;

				int Num_Man_Tablebases_Found = IInitializeTb(NalimovPath.c_str());
				cout  << Num_Man_Tablebases_Found << " man tablebases found" << endl;
				Log << Num_Man_Tablebases_Found << " man tablebases found" << endl;
			}
			else if (optionname == "Syzygy") {
				SyzygyPath = paramvalue;

				int found = InitSyzygy(SyzygyPath.c_str());
				cout << "info string Syzygy: " << found << " WDL tablebases found (up to "
					<< SyzygyMaxPieces() << " pieces)" << endl;
				Log << "Syzygy: " << found << " WDL tablebases found" << endl;
			}
		}

		else if (UciCommand == "proben") {
			int score = ProbePositionNalimov(pos);
			cout << "Probe result score: " << score << endl;
			Log << "Probe result score: " << score << endl;
		}

		else if (UciCommand == "probes") {
			bool ok = false;
			int wdl = ProbeSyzygyWDL(pos, &ok);
			if (!ok) {
				cout << "Syzygy WDL probe failed (not loaded / too many pieces / castling rights / no table)" << endl;
			}
			else {
				const char* names[] = { "loss", "blessed loss (draw under 50-move)", "draw",
										"cursed win (draw under 50-move)", "win" };
				cout << "WDL = " << wdl << " (" << names[wdl + 2] << ")" << endl;
				bool okz = false;
				int dtz = ProbeSyzygyDTZ(pos, &okz);
				if (okz)
					cout << "DTZ = " << dtz << " ply" << endl;
				else
					cout << "DTZ probe failed (no DTZ table for this material)" << endl;
			}
		}

		else if (UciCommand == "winc") {
			string white_inc = "";
			cin >> white_inc;

			winc = stoi(white_inc);
		}

		else if (UciCommand == "binc") {
			string black_inc = "";
			cin >> black_inc;

			binc = stoi(black_inc);
		}

		else if (UciCommand == "go") {
			// we need to take these inputs here so they don't get consumed and then GoCommand() is left waiting for input
			string time_left_white = "";
			string time_left_black = "";
			cin >> time_left_white;

			if (time_left_white == "infinite") {
				wtime = std::numeric_limits<int>::max();
				btime = std::numeric_limits<int>::max();
			}
			else {
				cin >> wtime >> time_left_black >> btime;
			}
			Log << ">> " << time_left_white << " " << wtime << " " << time_left_black << " " << btime << endl;
			if ((time_left_white == "btime") || (time_left_black == "wtime")) {
				int w = wtime;
				wtime = btime;
				btime = w;
			}

			thread t(GoCommand);
			t.detach();
		}
		else if (UciCommand == "moves") {
			Moves_Command();
		}
	}

	return 0;
}


int Parse_Fen(string Fen) {
	pos.Reset();

	string str = Fen;
	string temp = str;
	string hold[6];
	int processed = 0;
	for (int j = 0; j < 6; j++) {
		int a = temp.find(" ") + 1;
		for (int i = 0; i < a; i++)
			hold[j] += temp[i];
		processed += a;
		temp = "";
		for (unsigned int p = processed; p < str.length(); p++)
			temp += str[p];
	}
	hold[5] = "";
	for (unsigned int i = str.find_last_of(" ") + 1; i <= str.length(); i++)
		hold[5] += str[i];

	Search::Clear();

	// here we're checking for any EP squares called out in the FEN string
	if (hold[3].find('-') == string::npos) {
		for (int i = 0; i < 64; i++) {
			if (hold[3].find(PlayerMoves[i]) != string::npos)
				pos.EP_Square = GeneralBoard[i];
		}
	}

	pos.White_Pieces = 0;
	pos.Black_Pieces = 0;
	pos.White_King = 0;
	pos.Black_King = 0;
	pos.White_Queens = 0;
	pos.White_Rooks = 0;
	pos.White_Bishops = 0;
	pos.White_Knights = 0;
	pos.White_Pawns = 0;
	pos.Black_Queens = 0;
	pos.Black_Rooks = 0;
	pos.Black_Bishops = 0;
	pos.Black_Knights = 0;
	pos.Black_Pawns = 0;
	char Current_Square;
	for (unsigned int h = 0; h < (hold[0].length()); h++) {
		Current_Square = hold[0][h];
		Read_Fen(Current_Square);
	}
	Current_Rank = 72057594037927936;
	string Curr_Turn = hold[1];

	if (Curr_Turn.find("w") != string::npos) {
		pos.Current_Turn = true;
	}
	else {
		pos.Current_Turn = false;
	}
	string Legal_Castling = hold[2];
	pos.WhiteCanCastleK = false;
	pos.BlackCanCastleK = false;
	pos.WhiteCanCastleQ = false;
	pos.BlackCanCastleQ = false;
	for (unsigned int i = 0; i < Legal_Castling.length(); i++) {
		if (Legal_Castling.find("K") != string::npos)
			pos.WhiteCanCastleK = true;
		if (Legal_Castling.find("k") != string::npos)
			pos.BlackCanCastleK = true;
		if (Legal_Castling.find("Q") != string::npos)
			pos.WhiteCanCastleQ = true;
		if (Legal_Castling.find("q") != string::npos)
			pos.BlackCanCastleQ = true;
	}

	return 0;
}

int Read_Fen(char Current_Square) {
	switch (Current_Square) {
	case '1':
		if (!(Current_Rank & H_Pawn_Mask))
			Current_Rank *= 2;
		else
			Current_Rank *= 1;
		break;
	case '2':
		if (!(Current_Rank & G_Pawn_Mask))
			Current_Rank *= 4;
		else
			Current_Rank *= 2;
		break;
	case '3':
		if (!(Current_Rank & F_Pawn_Mask))
			Current_Rank *= 8;
		else
			Current_Rank *= 4;
		break;
	case '4':
		if (!(Current_Rank & E_Pawn_Mask))
			Current_Rank *= 16;
		else
			Current_Rank *= 8;
		break;
	case '5':
		if (!(Current_Rank & D_Pawn_Mask))
			Current_Rank *= 32;
		else
			Current_Rank *= 16;
		break;
	case '6':
		if (!(Current_Rank & C_Pawn_Mask))
			Current_Rank *= 64;
		else
			Current_Rank *= 32;
		break;
	case '7':
		if (!(Current_Rank & B_Pawn_Mask))
			Current_Rank *= 128;
		else
			Current_Rank *= 64;
		break;
	case '8':
		Current_Rank *= 128;
		break;
	case 'K':
		pos.White_King = Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if (!(Current_Rank & H_Pawn_Mask))
			Current_Rank *= 2;
		break;
	case 'k':
		pos.Black_King = Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if (!(Current_Rank & H_Pawn_Mask))
			Current_Rank *= 2;
		break;
	case 'Q':
		pos.White_Queens |= Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if (!(Current_Rank & H_Pawn_Mask))
			Current_Rank *= 2;
		break;
	case 'q':
		pos.Black_Queens |= Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if (!(Current_Rank & H_Pawn_Mask))
			Current_Rank *= 2;
		break;
	case 'R':
		pos.White_Rooks |= Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'r':
		pos.Black_Rooks |= Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'B':
		pos.White_Bishops |= Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'b':
		pos.Black_Bishops |= Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'N':
		pos.White_Knights |= Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'n':
		pos.Black_Knights |= Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'P':
		pos.White_Pawns |= Current_Rank;
		pos.White_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case 'p':
		pos.Black_Pawns |= Current_Rank;
		pos.Black_Pieces |= Current_Rank;
		if ((!(Current_Rank & H_Pawn_Mask)) && ((Current_Rank != 9223372036854775808ULL)))
			Current_Rank *= 2;
		break;
	case '/':
		Current_Rank /= 32768;
		break;
	}
	return 0;
}

int Moves_Command() {
	char First_Part[5];
	char Second_Part[5];
	string Promotion_Type;
	Log << ">> ";
	while (true) {
		Move m;
		cin.get();
		cin.get(First_Part, 3);

		string F = "go";
		if (First_Part == F) {
			cin.putback('o');
			cin.putback('g');
			break;
		}
		string f = "pe";
		if (First_Part == f) {
			cin.putback('e');
			cin.putback('p');
			return 0;
		}
		string z = "pr";
		if (First_Part == z) {
			cin.putback('r');
			cin.putback('p');
			return 0;
		}
		string i = "is";
		if (First_Part == i) {
			cin.putback('s');
			cin.putback('i');
			return 0;
		}
		string g = "be";
		if (First_Part == g) {
			cin.putback('e');
			cin.putback('b');
			return 0;
		}
		string e = "ev";
		if (First_Part == e) {
			cin.putback('v');
			cin.putback('e');
			return 0;
		}
		string h = "se";
		if (First_Part == h) {
			cin.putback('e');
			cin.putback('s');
			return 0;
		}
		Log << First_Part;
		cin.get(Second_Part, 3);
		Log << Second_Part;
		string a8 = "a8";
		string b8 = "b8";
		string c8 = "c8";
		string d8 = "d8";
		string e8 = "e8";
		string f8 = "f8";
		string g8 = "g8";
		string h8 = "h8";
		string a1 = "a1";
		string b1 = "b1";
		string c1 = "c1";
		string d1 = "d1";
		string e1 = "e1";
		string f1 = "f1";
		string g1 = "g1";
		string h1 = "h1";
		string a7 = "a7";
		string b7 = "b7";
		string c7 = "c7";
		string d7 = "d7";
		string e7 = "e7";
		string f7 = "f7";
		string g7 = "g7";
		string h7 = "h7";
		string a2 = "a2";
		string b2 = "b2";
		string c2 = "c2";
		string d2 = "d2";
		string e2 = "e2";
		string f2 = "f2";
		string g2 = "g2";
		string h2 = "h2";
		Bitboard Fr = 0;
		Bitboard T_o = 0;
		for (int i = 0; i < 64; i++) {
			if (First_Part == PlayerMoves[i]) {
				Fr = GeneralBoard[i];
			}
		}
		for (int i = 0; i < 64; i++) {
			if (Second_Part == PlayerMoves[i]) {
				T_o = GeneralBoard[i];
			}
		}
		if (((T_o & Eigth_Rank_White) && (Fr & Seventh_Rank_White) && (Fr & pos.White_Pawns)) || ((T_o & Eigth_Rank_Black) && (Fr & Seventh_Rank_Black) && (Fr & pos.Black_Pawns))) {
			cin >> Promotion_Type;
			Log << Promotion_Type << " ";
			Parse_Moves(First_Part, Second_Part, Promotion_Type);
		}
		else {
			Log << " ";
			Parse_Moves(First_Part, Second_Part);
		}
	}

	Log << endl;

	return 0;
}

int Parse_Moves(string First, string Second, string PromotionType) {
	Bitboard From = 0;
	Bitboard To = 0;
	for (int i = 0; i < 64; i++) {
		if (PlayerMoves[i] == First)
			From = GeneralBoard[i];
	}
	for (int i = 0; i < 64; i++) {
		if (PlayerMoves[i] == Second)
			To = GeneralBoard[i];
	}
	Piece p = pos.Get_Piece_From_Bitboard(From);
	assert(p != NONE);
	Piece Captured = NONE;
	bool Castling = (From & (pos.White_King | pos.Black_King)) && (From & 1152921504606846992) && (To & 4899916394579099716);
	bool Promotion = ((From & pos.White_Pawns) && From & Seventh_Rank_White) || ((From & pos.Black_Pawns) && (From & Seventh_Rank_Black));

	// here we check if the entered move is EP
	bool ep = false;

	if ((To & pos.EP_Square) && (From & (pos.White_Pawns | pos.Black_Pawns)))
		ep = true;

	if (From)

		if (pos.Current_Turn == true) {
			if (To & pos.Black_Pieces) {
				Captured = pos.Get_Piece_From_Bitboard(To);
				assert(Captured != NONE);
			}
		}
		else {
			if (To & pos.White_Pieces) {
				Captured = pos.Get_Piece_From_Bitboard(To);
				assert(Captured != NONE);
			}
		}
	if (p == WK) {
		pos.WhiteCanCastleK = false;
		pos.WhiteCanCastleQ = false;
	}
	else if (p == BK) {
		pos.BlackCanCastleK = false;
		pos.BlackCanCastleQ = false;
	}
	if (p == WR && From == 1) pos.WhiteCanCastleQ = false;
	if (p == WR && From == 128) pos.WhiteCanCastleK = false;
	if (p == BR && From == 72057594037927936) pos.BlackCanCastleQ = false;
	if (p == BR && From == 9223372036854775808ULL) pos.BlackCanCastleK = false;

	if(ep)
		if (pos.Current_Turn == true)
			Captured = BP;
		else
			Captured = WP;

	Move m(p, Captured, From, To, Castling, Promotion, ep);
	if (PromotionType != "") {
		if (PromotionType.find("q") != string::npos)
			m.PromotionType = pos.Current_Turn ? WQ : BQ;
		else if (PromotionType.find("r") != string::npos)
			m.PromotionType = pos.Current_Turn ? WR : BR;
		else if (PromotionType.find("b") != string::npos)
			m.PromotionType = pos.Current_Turn ? WB : BB;
		else if (PromotionType.find("n") != string::npos)
			m.PromotionType = pos.Current_Turn ? WN : BN;
	}
	pos.Make_Move(m);
	return 0;
}




string Engine_Info() {
	string result = "";
	string Version = "";
	const string months("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec");
	string month, day, year;
	stringstream ss, date(__DATE__); // From compiler, format is "Sep 21 2008"
	result += "Bitterroot ";
	date >> month >> day >> year;
	result += "20" + year.substr(2) + (to_string((1 + months.find(month) / 4)).length() < 2 ? "0" + to_string((1 + months.find(month) / 4)) : to_string((1 + months.find(month) / 4))) + (day.length() < 2 ? "0" + day : day);

	return result;

}
void Uci_Pv(int depth, int seldepth, Move best, int* matemoves, int time, int nodes, int tbhits, vector<LINE> PvLines) {
	output.lock();
	for (int i = 0; i < (PvLines.size() < MultiPV ? PvLines.size() : MultiPV); i++) {
		cout << "info multipv " << i + 1 << " depth " << depth << " seldepth " << depth + seldepth << " score ";
		Log << "info multipv " << i + 1 << " depth " << depth << " seldepth " << depth + seldepth << " score ";

		// If the tablebases returned a hit
		if (PvLines[i].score < -MATE) {
			cout << "mate " << -(127 + PvLines[i].score + MATE);
			Log << "mate " << -(127 + PvLines[i].score + MATE);
		}
		else if(PvLines[i].score > MATE) {
			cout << "mate " << 127 - (PvLines[i].score - MATE);
			Log << "mate " << 127 - (PvLines[i].score - MATE);
		}
		else if (PvLines[i].score == MATE) {
			if (depth + 1 < *matemoves) {
				cout << "mate " << ((depth + 1) / 2) - 1;
				Log << "mate " << ((depth + 1) / 2) - 1;
				*matemoves = depth + 1;
			}
			else {
				cout << "mate " << (*matemoves / 2) - 1;
				Log << "mate " << (*matemoves / 2) - 1;
			}
		}
		else if (PvLines[i].score == -MATE) {
			if (depth + 1 < *matemoves) {
				cout << "mate " << -(((depth + 1) / 2) - 1);
				Log << "mate " << -(((depth + 1) / 2) - 1);
				*matemoves = depth + 1;
			}
			else {
				cout << "mate " << -((*matemoves / 2) - 1);
				Log << "mate " << -((*matemoves / 2) - 1);
			}
		}
		else {
			cout << "cp " << PvLines[i].score;
			Log << "cp " << PvLines[i].score;
		}
		cout << " hashfull " << int((TT.count / 8388608.0) * 1000);

		if(tbhits > 0)
			cout << " tbhits " << tbhits;

		cout << " pv " << PvLines[i].Output();
		Log << " pv " << PvLines[i].Output();
		cout << "time " << time << " nodes " << nodes << " nps " << int(nodes / (float(time + 1) / 1000)) << endl;
		Log << "time " << time << " nodes " << nodes << " nps " << int(nodes / (float(time + 1) / 1000)) << endl;
	}
	output.unlock();
}

void Startpos() {
	//Set up the board internally
	pos.Reset();
	Search::Searching = false;
	wtime = 0;
	btime = 0;
	Search::STOP_SEARCHING_NOW = false;
	Search::Nodes = 0;
	Search::Clear();
	TT.clear();
	InitCounterMove();
}

void GoCommand() {
	Timer timer;
	Search::Searching = true;
	Search::Time_Allocation = 10000;
	timer.Start_Clock();
	Move blank;
	blank = Search::Think(wtime, btime, winc, binc);
	Search::Searching = false;

	for (int h = 0; h < 64; h++) {
		if (GeneralBoard[h] & blank.From) {
			cout << "bestmove " << PlayerMoves[h];
			Log << "bestmove " << PlayerMoves[h];
		}
	}
	for (int h = 0; h < 64; h++) {
		if (GeneralBoard[h] & blank.To) {
			cout << PlayerMoves[h];
			Log << PlayerMoves[h];
			if ((blank.To & Eigth_Rank_White) && (blank.From & Seventh_Rank_White) && ((blank.P == WP))) {
				cout << "q" << endl;
				Log << "q" << endl;
			}
			else if ((blank.To & Eigth_Rank_Black) && (blank.From & Seventh_Rank_Black) && ((blank.P == BP))) {
				cout << "q" << endl;
				Log << "q" << endl;
			}
			else {
				cout << endl;
				Log << endl;
			}
		}
	}

	Search::Nodes = 0;
}
