#ifndef __GAME_H__
#define __GAME_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "dbl_linked_list.h"
#include "settings.h"
#include "tetromino.h"
#include <stdbool.h>

typedef struct Block {
    bool active;
    uint_fast8_t texture_pos;
} Block;

typedef struct Game {
    // Board & Tetrominos
    DblLinkedList board;
    Tetromino tetromino;
    bool held_tetromino;
    uint_fast8_t hold_tetromino;
    uint_fast8_t next_tetromino;

    // Gameplay & Stats
    uint_fast16_t frame;
    double drop_speed;
    char lines_txt[MAX_LINES_DIGITS];
    uint_fast32_t lines_cleared;
    int screen_shake;

    // Textures
    Texture2D texture_map;
    Rectangle border_block;
} Game;

typedef enum RotationDirection { ClockWise, CounterClockWise } Direction;

// Map Functions
void draw_map(DblLinkedList *map, Texture2D *texture);

// Game Functions
bool check_tetromino_collision(DblLinkedList *board, Tetromino *tet, int offs_x,
                               int offs_y);
void push_tetromino(Game *game);
void next_tetromino(Game *game);
void rotate_tetromino(DblLinkedList *board, Tetromino *tet, Direction dir,
                      bool nested);
void hold_tetromino(Game *game);
int update_game(Game *game, Camera2D *cam);
void draw_game(Game *game);
void init_game(Game *game);  // Initialize Game Map and Load Texture
void reset_game(Game *game); // Reinitialize Game Map
void end_game(Game *game);   // Free Game Memory

#ifdef __cplusplus
}
#endif
#endif
