#ifndef TBINTERFACE_H
#define TBINTERFACE_H

#include "tbprobe.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize data structures
void TBitf_init(void);

// Allocate a TB_Position instance
TB_Position *TBitf_alloc_position(void);

// Free a TB_Position instance
void TBitf_free_position(TB_Position *pos);

// Initialize a TB_Position instance from a fen
void TBitf_set_from_fen(TB_Position *pos, const char *fen, int *cnt50);

// Initialize a TB_Position instance directly from engine piece lists (see the
// implementation in tbinterface_bb.c for the parameter conventions). Returns
// false if the position has too many pieces to probe.
bool TBitf_setup_from_pieces(TB_Position *pos, int num, const uint8_t *pt,
    const uint8_t *sq, int stm, int epSquare);

// Convert a move to algebraic notation
void TBitf_move_to_string(TB_Position *pos, int m, char *str);

// Convert a move to uci notation
void TBitf_move_to_string_uci(TB_Position *pos, int m, char *str);

// Print an ASCII representation of the position to stdout
void TBitf_print_pos(TB_Position *pos);

#ifdef __cplusplus
}
#endif

#endif
