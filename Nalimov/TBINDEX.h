#ifndef TBINDEX_H_
#define TBINDEX_H_
#define NEW
#define T41_INCLUDE
#define T33_INCLUDE

// Forward declaration
typedef int square;
typedef unsigned long INDEX;
typedef INDEX(__fastcall* PfnCalcIndex)(square* psqW, square* psqB, square sqEnP, int fInverse);

int IInitializeTb(const char* pszPath);
int IDescFindFromCounters(int*);
PfnCalcIndex PfnIndCalcFun(int iTb, int side);
int TbtProbeTable(int iTb, int side, unsigned long indOffset);
#endif
