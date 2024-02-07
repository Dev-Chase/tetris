#ifndef __TETROMINO_H__
#define __TETROMINO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Tetromino {
    bool data[4][4]; // Buffer to Store Block Data
    int dimension;   // Specifies Used Amount of Rows and Cols
    int_fast8_t x;
    int_fast8_t y;
    uint_fast8_t id;
} Tetromino;

// Tetromino Functions
void draw_tetromino(const Tetromino *tet, int_fast32_t offs_x,
                    int_fast32_t offs_y, Texture2D *texture);
void tet_rev_row(Tetromino *tet);
void tet_swap_diag(Tetromino *tet);
// void rotate_tetromino(Tetromino *tet, Direction dir);
uint_fast8_t tetromino_offset_right(const Tetromino *tet);
uint_fast8_t tetromino_offset_left(const Tetromino *tet);
uint_fast8_t tetromino_offset_down(const Tetromino *tet);
bool in_bounds(Tetromino *tet, int offs_x, int offs_y);

#ifdef __cplusplus
}
#endif
#endif
