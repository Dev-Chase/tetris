#include "tetromino.h"
#include "raylib.h"
#include "settings.h"
#include <stdio.h>
#include <stdlib.h>

void draw_tetromino(const Tetromino *tet, int_fast32_t offs_x,
                    int_fast32_t offs_y, Texture2D *texture) {
    for (int i = 0; i < tet->dimension; i++) {
        for (int j = 0; j < tet->dimension; j++) {
            if (tet->data[i][j]) {
                DrawTextureRec(*texture,
                               (Rectangle){.x = tet->id * TILE_SIZE,
                                           .y = 0,
                                           .width = TILE_SIZE,
                                           .height = TILE_SIZE},
                               (Vector2){(j * TILE_SIZE) + offs_x +
                                             (SIDE_BUFFER + TILE_SIZE),
                                         i * TILE_SIZE + offs_y},
                               WHITE);

                continue;
            }
        }
    }
}

void tet_swap_diag(Tetromino *tet) {
    // Swap Items Along Diagonal
    for (int i = 0; i < tet->dimension; i++) {
        for (int j = 0; j < i; j++) {
            bool swp = tet->data[i][j];
            tet->data[i][j] = tet->data[j][i];
            tet->data[j][i] = swp;
        }
    };
}

void tet_rev_row(Tetromino *tet) {
    // Reverse Each Row
    for (int i = 0; i < tet->dimension; i++) {
        for (int j = 0; j < tet->dimension / 2; j++) {
            bool swp = tet->data[i][j];
            tet->data[i][j] = tet->data[i][tet->dimension - 1 - j];
            tet->data[i][tet->dimension - 1 - j] = swp;
        }
    };
}

uint_fast8_t tetromino_offset_left(const Tetromino *tet) {
    for (int i = 0; i < tet->dimension; i++) {
        for (int j = 0; j < tet->dimension; j++) {
            if (tet->data[j][i]) {
                return i;
            }
        }
    }
    fprintf(stderr, "error: invalid tetromino data\n");
    return 0;
}

uint_fast8_t tetromino_offset_right(const Tetromino *tet) {
    for (int i = tet->dimension - 1; i > -1; i--) {
        for (int j = 0; j < tet->dimension; j++) {
            if (tet->data[j][i]) {
                return i;
            }
        }
    }
    fprintf(stderr, "error: invalid tetromino data\n");
    return 0;
}

uint_fast8_t tetromino_offset_down(const Tetromino *tet) {
    for (int i = tet->dimension - 1; i > -1; i--) {
        for (int j = 0; j < tet->dimension; j++) {
            if (tet->data[i][j]) {
                return i;
            }
        }
    }
    fprintf(stderr, "error: invalid tetromino data\n");
    return 0;
}

bool in_bounds(Tetromino *tet, int offs_x, int offs_y) {
    if (tet->x + offs_x + tetromino_offset_left(tet) > -1 &&
        tet->x + offs_x + tetromino_offset_right(tet) < COL_SIZE &&
        tet->y + offs_y + tetromino_offset_down(tet) < ROW_SIZE) {
        return true;
    }
    return false;
}
