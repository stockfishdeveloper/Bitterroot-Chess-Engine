using namespace std;
#include <vector>
#include <algorithm>
#include "Bitboard.h"
#include "Search.h"
#include "Eval.h"
#include "UCI.h"
#include "magicmoves.h"
#include "Endgame.h"
#include "Zobrist.h"
#include "TransTable.h"
#include "Thread.h"
#include "Util.h"
#include "CounterMove.h"
#include "TimeUsage.h"
#include "Tablebase.h"

int Search::Time_Allocation = 0;
bool Search::Searching = false;
Bitboard Search::Nodes = 0;
int Search::Depth = 0;
int Search::Seldepth = 0;
bool Search::STOP_SEARCHING_NOW = false;
Bitboard Search::tbhits = 0;
Move Search::Think(int wtime, int btime, int winc, int binc, int Maxdepth) {
	Move Best;
	Best.Score = -INF;
	Timer timer;
	timer.Start_Clock();
	Time_Allocation = (pos.Current_Turn ? wtime : btime);
	Move move;
	int rootAlpha = -INF;
	int rootBeta = INF;
	const int MAXDEPTH = Maxdepth;
	LINE line;
	vector<Move>rootstack;
	int count = 0;
	int matemoves = 1000;
	Search::tbhits = 0;
	vector<LINE> pvlines;
	//InitCounterMove();

	for (int q = 1; q < MAXDEPTH; q++) {
		Search::Depth = q;
		output.lock();
		cout << "info depth " << Search::Depth << " nodes " << Search::Nodes << endl;
		output.unlock();

		if (q == 1) {
			pos.Current_Turn ? Generate_White_Moves(false, pos) : Generate_Black_Moves(false, pos);
			if (pos.numlegalmoves == 1) {
				return pos.LegalMoves[0];
			}
		}
		if (q == 1) {
			TTEntry* tt = TT.probe(Get_Current_Hash_Key(&pos));
			for (int h = 0; h < pos.numlegalmoves; h++) {
				rootstack.push_back(pos.LegalMoves[h]);
				count++;
			}
		}
		std::stable_sort(rootstack.begin(), rootstack.end(), [](const Move& lhs, const Move& rhs) { return (lhs.Score > rhs.Score); });

		bool inCheck = Search::Is_Mate(&pos) == -MATE;
		for (unsigned int i = 0; i < rootstack.size(); i++) {
			if (q >= 2 && timer.Get_Time() > 1000) {
				output.lock();
				cout << "info currmove " << PlayerMoves[(lsb(rootstack[i].From))] << PlayerMoves[(lsb(rootstack[i].To))];
				Log << "<< info currmove " << PlayerMoves[(lsb(rootstack[i].From))] << PlayerMoves[(lsb(rootstack[i].To))];
				cout << " currmovenumber " << (i + 1) << endl;
				Log << " currmovenumber " << (i + 1) << endl;
				output.unlock();
			}

			pos.Make_Move(rootstack[i]);

			int score = 0;

			// While we're at root, check the tablebases to see if the current position is a win/loss/draw
			if (NalimovPath != "" && ((int)__popcnt64(pos.White_Pieces | pos.Black_Pieces) < 6)) {
				int tb_score = -ProbePositionNalimov(pos);

				if (tb_score != 127) {
					int final_score = 0;

					// increment our tbhits counteer
					tbhits++;

					if (tb_score > 0) {
						final_score = MATE + tb_score;
					}
					else if (tb_score < 0) {
						final_score = -MATE + tb_score;
					}

					score = final_score;
				}
			}

			else {
				score = -AlphaBeta(&pos, -rootBeta, -rootAlpha, q - 1, &line, true, true);
				rootstack[i].Score = score;
			}

			pos.Undo_Move(rootstack[i]);

			if (score >= rootBeta) {
				score = -AlphaBeta(&pos, -INF, INF, (q - 1), &line, true, true);
				rootstack[i].Score = score;
			}

			if (score > rootAlpha) {
				LINE* f = new LINE;
				f->cmove = 0;
				::PVline = *f;
				delete f;
				::PVline.argmove[0] = rootstack[i];
				::PVline.score = score;
				::PVline.cmove = line.cmove + 1;
				memcpy(::PVline.argmove + 1, line.argmove, line.cmove * sizeof(Move));
				Best = rootstack[i];
				Best.Score = score;
				//Uci_Pv(q, Seldepth, Best, &matemoves, timer.Get_Time(), Nodes);
				rootAlpha = score;
			}

			LINE TempLine;
			TempLine.argmove[0] = rootstack[i];
			TempLine.score = score;
			TempLine.cmove = line.cmove + 1;
			memcpy(TempLine.argmove + 1, line.argmove, line.cmove * sizeof(Move));
			pvlines.push_back(TempLine);

			if (Should_Stop_Searching_Now(timer.Get_Time(), Search::Time_Allocation, (pos.Current_Turn ? winc : binc), q)) {
				Search::STOP_SEARCHING_NOW = true;
				return Best;
			}
		}

		// sort all pvlines by score
		std::stable_sort(pvlines.begin(), pvlines.end(), [](const LINE& lhs, const LINE& rhs) { return (lhs.score > rhs.score); });
		// output all our multipv lines
		Uci_Pv(q, Seldepth, Best, &matemoves, timer.Get_Time(), Nodes, Search::tbhits, pvlines);

		// here we need to clear out the pvlines since we don't care about pv lines from previous depths
		pvlines.clear();

		TT.save(q, rootAlpha, Best, Alpha, Get_Current_Hash_Key(&pos));
		rootAlpha = Best.Score - 50;
		//rootBeta = Best.Score + 50;
		//Uci_Pv(q, Seldepth, Best, &matemoves, timer.Get_Time(), Nodes);
		LINE* f = new LINE;
		f->cmove = 0;
		//::PVline = *f;
		line = *f;
		delete f;

		if (STOP_SEARCHING_NOW) {
			Time_Allocation = 0;
			return Best;
		}
	}
	return Best;
}

int Search::AlphaBeta(Position* posit, int alpha, int beta, int depth, LINE* pline, bool PvNode, bool donullmove) {
	if (depth <= 1) {
		++Nodes;
		pline->cmove = 0;
		return QuiescenceSearch(posit, alpha, beta, depth + 1);
	}
	if (STOP_SEARCHING_NOW) {
		Time_Allocation = 0;
		return alpha;
	}
	Position position(posit);
	bool inCheck = Search::Is_Mate(&position) == -MATE;
	LINE line;
	//TT Probe
	TTEntry* tt = TT.probe(Get_Current_Hash_Key(&position));
	if (!PvNode && tt != NULL) {
		if (tt->depth >= depth) {
			if (tt->nodetype == Alpha) /*alpha = tt->score;*/if (tt->score <= alpha) return alpha;
			if (tt->nodetype == Beta) /*beta = tt->score;*/if (tt->score >= beta) return beta;
			if (tt->nodetype == Exact) return tt->score;
		}
	}

	// If position has less than 5 pieces, check the tablebases to see if the current position is a win/loss/draw
	if (NalimovPath != "" && depth >= 3 && ((int)__popcnt64(position.White_Pieces | position.Black_Pieces) < 6)) {
		int tb_score = ProbePositionNalimov(position);

		if (tb_score != 127) {
			int final_score = 0;

			// increment our tbhits counteer
			tbhits++;

			// score will be mate in x moves plus however many full moves we are from root
			if (tb_score > 0) {
				final_score = MATE + tb_score - ((Search::Depth - depth) / 2);
			}
			else if (tb_score < 0) {
				final_score = -MATE + tb_score + ((Search::Depth - depth) / 2);
			}

			return final_score;
		}
	}

	//Null Move Pruning
	// very important, do not try null move pruning if we're already confirmed to be in a position where mate is already forced for either side
	// doing it here completelly wrecks the score result, leading to inaccurate mate scores
	if ((!inCheck) /*&& !PvNode*/ && (depth > 3) && (position.Non_Pawn_Material() > 0) && (beta > -MATE) && (beta < MATE)) {
		position.Current_Turn ^= 1;
		int score = (depth == 4 ?
			-QuiescenceSearch(&position, -beta, -beta + 1, depth - 3) :
			-AlphaBeta(&position, -beta, -beta + 1, depth - 3, &line, false, false));
		
		if (score >= beta) {
			position.Current_Turn ^= 1;
			return beta;
		}
		position.Current_Turn ^= 1;
	}
	position.Current_Turn ? Generate_White_Moves(false, position) : Generate_Black_Moves(false, position);
	if (position.numlegalmoves == 0) {
		alpha = inCheck * -MATE;
		pline->score = alpha;
		pline->cmove = 0;
		return alpha;
	}
	NodeType node = Alpha;
	vector<Move> moves;
	for (int i = 0; i < position.numlegalmoves; i++) {
		position.LegalMoves[i].Score = 0;
		//if(tt != NULL) if(tt->best.From == position.LegalMoves[i].From && tt->best.To == position.LegalMoves[i].To) position.LegalMoves[i].Score += 1000;
		if (position.LegalMoves[i].C != NONE)
			position.LegalMoves[i].Score += (Get_Move_Score(position.LegalMoves[i]));
		//if(position.LegalMoves[i].C != NONE)
			//position.LegalMoves[i].Score += SEE(&position, position.LegalMoves[i].To);
		//if(position.LegalMoves[i].Promotion)
			//position.LegalMoves[i].Score += 50;
		position.LegalMoves[i].Score += CounterMove[depth][lsb(position.LegalMoves[i].From)][lsb(position.LegalMoves[i].To)] * 200;
		//position.LegalMoves[i].Score += Eval::EvalPSQTResult(&position, position.LegalMoves[i]); //SEEMED TO BE WORTH A LOT OF ELO--Sucks a lot of NPS
		moves.push_back(position.LegalMoves[i]);
	}
	std::stable_sort(moves.begin(), moves.end(), [](const Move& lhs, const Move& rhs) { return lhs.Score > rhs.Score; });
	/*for(int i = 0; i < moves.size(); i++)
	{
		Log << i << " " << moves[i].Score << (moves[i].Score >= 1000 ? " tt move" : "") << (moves[i].Promotion  ? " promotion" : "");
		if(CounterMove[depth][lsb(position.LegalMoves[i].From)][lsb(position.LegalMoves[i].To)] != 0)
			Log << " CounterMove score was: " << CounterMove[depth][lsb(position.LegalMoves[i].From)][lsb(position.LegalMoves[i].To)] << endl;
		else
			Log << endl;
	}
	Log << "END" << endl;*/
	//int pos_score = Eval::Evaluate_Position(&position);
	bool pvfound = false;
	//if(inCheck) depth++; Check extension--seems to be worth ~50-100 ELO
	for (int i = 0; i < position.numlegalmoves; i++) {
		/*if(depth == 2 && !inCheck && !PvNode)
		{
			if((pos_score + 200) <= alpha)
			continue;
		}
		if(depth == 3 && !inCheck && !PvNode)
		{
			if((pos_score + 500) <= alpha)
			continue;
		}
		if(depth == 4 && !inCheck && !PvNode)
		{
			if((pos_score + 800) <= alpha)
			continue;
		}*/
		Nodes++;
		position.Make_Move(moves[i]);
		int score;
		//if(i < 3 && PvNode)
		score = -AlphaBeta(&position, -beta, -alpha, depth - 1, &line, true, false);

		/*else if(depth > 2
				&& i > 2
				&& !PvNode
				&& (moves[i].C == NONE)
				&& (!(moves[i].Promotion))
				&& !inCheck)
				{
					score = -AlphaBeta(&position, -(alpha + 1), -alpha, depth - 2, &line, false, true);
					if(score > alpha)
						score = -AlphaBeta(&position, -(alpha + 1), -alpha, depth - 1, &line, false, true);
					if(score > alpha)
						score = -AlphaBeta(&position, -beta, -alpha, depth - 1, &line, false, true);
				}
		else if(PvNode)
		{
			score = -AlphaBeta(&position, -(alpha+1), -alpha, depth - 1, &line, true, false);
			if(score > alpha)
				score = -AlphaBeta(&position, -beta, -alpha, depth - 1, &line, true, false);
		}
		else
			score = -AlphaBeta(&position, -beta, -alpha, depth - 1, &line, false, true);*/

		position.Undo_Move(moves[i]);

		if (score >= beta) {
			TT.save(depth, beta, moves[i], Beta, Get_Current_Hash_Key(&position));
			if (moves[i].C == NONE)
				CounterMove[depth][lsb(moves[i].From)][lsb(moves[i].To)]++;
			return beta;
		}

		if (score > alpha) {
			pline->argmove[0] = moves[i];
			pline->score = score;
			memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(Move));
			pline->cmove = line.cmove + 1;
			alpha = score;
			node = Exact;
			pvfound = true;
		}
	}
	TT.save(depth, alpha, pline->argmove[0], node, Get_Current_Hash_Key(&position));
	return alpha;
}

int Search::Is_Mate(Position* position) {
	int score = 0;
	int h = lsb(position->White_King);
	Bitboard BAttacks = Bmagic(h, (position->White_Pieces | position->Black_Pieces));
	Bitboard RAttacks = Rmagic(h, (position->White_Pieces | position->Black_Pieces));
	Bitboard QAttacks = Qmagic(h, (position->White_Pieces | position->Black_Pieces));

	if (BAttacks & (position->Black_Bishops))
		score = -1;
	if (RAttacks & (position->Black_Rooks))
		score = -1;
	if (QAttacks & (position->Black_Queens))
		score = -1;
	if (Knight_Lookup_Table[h] & position->Black_Knights)
		score = -1;
	if (King_Lookup_Table[h] & position->Black_King)
		score = -1;
	Bitboard Spare = position->Black_Pawns;
	Spare |= H_Pawn_Mask;
	Spare ^= H_Pawn_Mask;
	if ((Spare >> 7) & position->White_King)
		score = -1;
	Bitboard Spare2 = position->Black_Pawns;
	Spare2 |= A_Pawn_Mask;
	Spare2 ^= A_Pawn_Mask;
	if ((Spare2 >> 9) & position->White_King)
		score = -1;
	Bitboard Black_Pawns5 = position->Black_Pawns;
	Black_Pawns5 |= A_Pawn_Mask;
	Black_Pawns5 ^= A_Pawn_Mask;
	Black_Pawns5 |= H_Pawn_Mask;
	Black_Pawns5 ^= H_Pawn_Mask;
	if (((Black_Pawns5 >> 7) | (Black_Pawns5 >> 9)) & position->White_King)
		score = -1;
	if (score != 0) {
		if (score < 0) {
			if (position->Current_Turn)
				return -MATE;
			else
				return MATE;
		}
	}
	h = lsb(position->Black_King);
	BAttacks = Bmagic(h, (position->White_Pieces | position->Black_Pieces));
	RAttacks = Rmagic(h, (position->White_Pieces | position->Black_Pieces));
	QAttacks = Qmagic(h, (position->White_Pieces | position->Black_Pieces));
	if (BAttacks & (position->White_Bishops))
		score = 1;
	if (RAttacks & (position->White_Rooks))
		score = 1;
	if (QAttacks & (position->White_Queens))
		score = 1;
	if (Knight_Lookup_Table[h] & position->White_Knights)
		score = 1;
	if (King_Lookup_Table[h] & position->White_King)
		score = 1;
	Spare = position->White_Pawns;
	Spare |= A_Pawn_Mask;
	Spare ^= A_Pawn_Mask;
	if ((Spare << 7) & position->Black_King)
		score = 1;
	Bitboard Spare7 = position->White_Pawns;
	Spare7 |= H_Pawn_Mask;
	Spare7 ^= H_Pawn_Mask;
	if ((Spare7 << 9) & position->Black_King)
		score = 1;
	Bitboard White_Pawns2 = position->White_Pawns;
	White_Pawns2 |= A_Pawn_Mask;
	White_Pawns2 ^= A_Pawn_Mask;
	White_Pawns2 |= H_Pawn_Mask;
	White_Pawns2 ^= H_Pawn_Mask;
	if (((White_Pawns2 << 7) | (White_Pawns2 << 9)) & GeneralBoard[h])
		score = 1;
	if (score != 0) {
		if (score > 0) {
			if (position->Current_Turn)
				return MATE;
			else
				return -MATE;
		}
	}
	return 0;
}

void Search::Clear() {
	Search::Searching = false;
	Search::STOP_SEARCHING_NOW = false;
	Search::Nodes = 0;
	Search::Depth = 0;
}

int Search::QuiescenceSearch(Position* posit, int alpha, int beta, int depth) {
	int stand_pat = posit->Current_Turn ? Eval::Evaluate_Position(posit) : -Eval::Evaluate_Position(posit);
	Search::Seldepth = std::max(depth, Search::Seldepth);
	if (stand_pat >= beta)
		return beta;
	if (stand_pat > alpha)
		alpha = stand_pat;
	Nodes++;
	Position position(posit);
	bool inCheck = Search::Is_Mate(&position) == -MATE;
	if (inCheck)
		position.Current_Turn ? Generate_White_Moves(false, position) : Generate_Black_Moves(false, position);
	else
		position.Current_Turn ? Generate_White_Moves(true, position) : Generate_Black_Moves(true, position);
	if (position.numlegalmoves == 0) {
		if (inCheck)
			return -MATE;
		else
			return stand_pat;
	}
	TTEntry* tt = TT.probe(Get_Current_Hash_Key(&position));
	if (tt != NULL) {
		if (tt->depth >= depth) {
			if (tt->nodetype == Alpha) /*alpha = tt->score;*/if (tt->score <= alpha) return alpha;
			if (tt->nodetype == Beta) /*beta = tt->score;*/if (tt->score >= beta) return beta;
			if (tt->nodetype == Exact) return tt->score;
		}
	}
	Move Best;
	vector<Move>stack;
	for (int i = 0; i < position.numlegalmoves; i++) {
		Move m = position.LegalMoves[i];
		m.Score += Get_Move_Score(m);
		m.Score += SEE(&position, position.LegalMoves[i].To);
		if (tt != NULL) if (tt->best.From == m.From && tt->best.To == m.To) m.Score += 100000;
		stack.push_back(m);
	}
	std::stable_sort(stack.begin(), stack.end(), [](const Move& lhs, const Move& rhs) { return lhs.Score > rhs.Score; });
	if (stack.size() > 0) Best = stack[0];
	NodeType n = Alpha;
	for (unsigned int i = 0; i < (stack.size()); i++) {
		if (stand_pat + Get_Cp_Value(stack[i].C) <= alpha)
			continue;
		//if(SEE(&position, position.LegalMoves[i].To) < 0)
			//continue;
		Nodes++;
		position.Make_Move(stack[i]);
		int score;
		//if(i == 0)
		score = -QuiescenceSearch(&position, -beta, -alpha, depth + 1);
		/*else
		{
			score = -QuiescenceSearch(&position, -beta, -beta + 1, depth + 1);
			if(score > alpha)
				score = -QuiescenceSearch(&position, -beta, -alpha, depth + 1);
		}*/
		position.Undo_Move(stack[i]);
		if (score >= beta) {
			TT.save(depth, beta, stack[i], Beta, Get_Current_Hash_Key(&position));
			return beta;
		}
		if (score > alpha) {
			n = Exact;
			Best = stack[i];
			Best.Score = score;
			alpha = score;
		}
	}
	TT.save(depth, Best.Score, Best, n, Get_Current_Hash_Key(&position));
	return alpha;
}
int Search::MateSearch(Position* posit, int alpha, int beta, int depth) {
	if (depth <= 1) {
		Nodes++;
		return 0;
	}
	Position position(posit);
	position.Current_Turn ? Generate_White_Moves(false, position) : Generate_Black_Moves(false, position);
	if (position.numlegalmoves == 0) {
		return (Is_Mate(&position));
	}
	for (int i = 0; i < position.numlegalmoves; i++) {
		Nodes++;
		position.Make_Move(position.LegalMoves[i]);
		int score = -MateSearch(&position, -beta, -alpha, depth - 1);
		position.Undo_Move(position.LegalMoves[i]);
		if (score >= beta) {
			return beta;
		}
		if (score > alpha) {
			alpha = score;
		}
	}
	return alpha;
}

int Search::Get_Move_Score(Move& m) {
	return ((64 * Get_Cp_Value(m.C)) - Get_Cp_Value(m.P));
}
int Search::SEE(Position* position, Bitboard square) {
	int value = 0;
	Piece victim = position->Get_Piece_From_Bitboard(square);
	Bitboard b = position->Current_Turn == true ? position->GetLeastWhiteAttacker(square) : position->GetLeastBlackAttacker(square);
	Piece aggressor = position->Get_Piece_From_Bitboard(b);
	//if(aggressor == WP || aggressor == BP) return 100;
	if (aggressor != NONE) {
		//int prize = (Get_Cp_Value(victim) < Get_Cp_Value(aggressor));
		//if(prize < 0) return prize;

		// THIS NEEDS TO BE CHECKED
		// We have to check if this is an en passant move here
		Position posit(position);
		Move m(aggressor, victim, b, square, false,
			(((aggressor == WP && square & Eigth_Rank_White) || (aggressor == BP && square & Eigth_Rank_Black)) ? true : false), false);
		if ((aggressor == WP && square & Eigth_Rank_White) || (aggressor == BP && square & Eigth_Rank_Black))
			m.PromotionType = posit.Current_Turn == true ? WQ : BQ;
		posit.Make_Move(m);
		value = Get_Cp_Value(victim) - SEE(&posit, square);
		posit.Undo_Move(m);
	}
	return value;
}
