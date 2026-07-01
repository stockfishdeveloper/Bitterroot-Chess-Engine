#ifndef UTIL_H
#define UTIL_H
#include "Bitboard.h"
#include "Search.h"

const char LSB_64_table[154] =
{
#define __ 0
   0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
#undef __
};

const Bitboard debruijn64 = 0x03f79d71b4cb0a89;

#define lsb(bb) (LSB_64_table[((bb ^ (bb - 1)) * debruijn64) >> 58])

enum Movetype {
	Capture, Promotion, Normal
};

void Benchmark();
void Display_Move(Move m);
Movetype Get_Move_Type(Move& m);
Bitboard Unconvert_Int(const int& number);
#endif
