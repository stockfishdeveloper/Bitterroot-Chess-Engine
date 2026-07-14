#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <iostream>
#include <chrono>
#include "Search.h"

using namespace std;

extern int Time_Usage;
extern int MultiPV;
extern Bitboard Current_Rank;
extern string UciCommand;
extern string NalimovPath;
extern string SyzygyPath;
extern LINE PVline;
extern ofstream Log;

extern int CheckUci();
void Uci_Pv(int depth, int seldepth, Move best, int* matemoves, int time, int nodes, int tbhits, vector<LINE> PvLines);
void Startpos();
void GoCommand();
int Parse_Fen(string Fen);
int MakeMove(Move&);
int Moves_Command();
int Parse_Moves(string First_Part, string Second_Part, string Promotion_Type = "");
int Read_Fen(char Current_Square);
extern string Engine_Info();
#endif
