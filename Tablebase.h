#ifndef TABLEBASE_H
#define TABLEBASE_H

#include <intrin.h>
#include "Nalimov\TBINDEX.h"
#include "Bitboard.h"
#include "Util.h"

using namespace std;

int ProbePositionNalimov(Position& position);

// --- Syzygy tablebases ------------------------------------------------------
// Initialise the Syzygy probing code with a (semicolon-separated) path list.
// Returns the number of WDL tables found. Safe to call repeatedly to reload.
int InitSyzygy(const char* path);

// Largest piece count for which WDL tables were found (0 if none loaded).
int SyzygyMaxPieces();

// Raw probes from the side-to-move's perspective. *success is set to false if
// the position could not be probed (not loaded / too many pieces / castling
// rights / no matching table).
//   WDL: -2 loss, -1 blessed loss, 0 draw, 1 cursed win, 2 win.
//   DTZ: distance-to-zeroing in plies (see Syzygy/tbprobe.h for the full spec).
int ProbeSyzygyWDL(Position& position, bool* success);
int ProbeSyzygyDTZ(Position& position, bool* success);

// Search helpers. Both return true on a TB hit and write an around-MATE score
// (win = MATE + k, loss = -MATE - k, draw = 0) from the side-to-move's
// perspective, so it drops straight into the engine's existing score handling.
//   - ProbeSyzygySearchWDL: interior nodes, WDL only, distance approximated by
//     ply-from-root so shallower wins score higher.
//   - ProbeSyzygyRootDTZ: root moves, DTZ-based so conversion distance is real.
bool ProbeSyzygySearchWDL(Position& position, int plyFromRoot, int& score);
bool ProbeSyzygyRootDTZ(Position& position, int& score);

#endif