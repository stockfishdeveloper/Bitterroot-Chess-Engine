using namespace std;
#include "Zobrist.h"
#include "magicmoves.c"//Pradu's Magic Bitboards
#include "UCI.h"
#include "CounterMove.h"

int main() {
	initmagicmoves();//Thanks, Pradu
	InitCounterMove();
	cout << Engine_Info();
	cout << "\nA chess engine by David Cimbalista\nCopyright (c) 2026" << endl;
	CheckUci(); //Main command loop
	return 0;
}