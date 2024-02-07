#include "game.h"
#include "dbl_linked_list.h"
#include "raylib.h"
#include "tetromino.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Tetromino Data
const Tetromino POSSIBLE_TETROMINOS[7] = {
	// I - Colour =
	{.data =
		 {
			 {false, false, false, false},
			 {true, true, true, true},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 0,
	 .dimension = 4},
	// O
	{.data =
		 {
			 {true, true, false, false},
			 {true, true, false, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 1,
	 .dimension = 2},
	// T
	{.data =
		 {
			 {false, true, false, false},
			 {true, true, true, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 2,
	 .dimension = 3},
	// J
	{.data =
		 {
			 {true, false, false, false},
			 {true, true, true, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 3,
	 .dimension = 3},
	// L
	{.data =
		 {
			 {false, false, true, false},
			 {true, true, true, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 4,
	 .dimension = 3},
	// S
	{.data =
		 {
			 {false, true, true, false},
			 {true, true, false, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 5,
	 .dimension = 3},
	// Z
	{.data =
		 {
			 {true, true, false, false},
			 {false, true, true, false},
			 {false, false, false, false},
			 {false, false, false, false},
		 },
	 .x = COL_SIZE / 2 - 1,
	 .y = 0,
	 .id = 6,
	 .dimension = 3},
};

// Map Functions
void print_map(DblLinkedList *list) {
	DblNode *cur = list->head;

	int i = 0;
	printf("NULL");
	while (cur != NULL) {
		printf(" <- [#%d=%hhu] -> ", i, ((Block *)cur->val)->active);
		cur = cur->next;
		i++;
	}
	printf("NULL\n");
}

void draw_map(DblLinkedList *map, Texture2D *texture) {
	DblNode *cur = map->head;
	for (int i = 0; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			if (((Block *)cur->val)[j].active == true) {
				DrawTextureRec(
					*texture,
					(Rectangle){.x = ((Block *)cur->val)[j].texture_pos *
									 TILE_SIZE,
								.y = 0,
								.width = TILE_SIZE,
								.height = TILE_SIZE},
					(Vector2){(j * TILE_SIZE) + SIDE_BUFFER + TILE_SIZE,
							  i * TILE_SIZE},
					WHITE);
			}
		}
		cur = cur->next;
	}
}

// Game Functions
bool check_tetromino_collision(DblLinkedList *board, Tetromino *tet, int offs_x,
							   int offs_y) {
	DblNode *node = (tet->y + offs_y < ROW_SIZE - 1)
						? get_dbl_node(board, tet->y + offs_y)
						: get_dbl_node(board, tet->y);
	for (int i = 0; i < tet->dimension; i++) {
		Block *row = (Block *)node->val;
		for (int j = 0; j < tet->dimension; j++) {
			if (tet->data[i][j] && row[tet->x + offs_x + j].active) {
				return true;
			}
		}

		if (node->next == NULL) {
			break;
		}
		node = node->next;
	}

	return false;
}

void push_tetromino(Game *game) {
	DblNode *row = get_dbl_node(&game->board, game->tetromino.y);
	for (int i = 0; i < game->tetromino.dimension; i++) {
		for (int j = 0; j < game->tetromino.dimension; j++) {
			if (game->tetromino.data[i][j]) {
				((Block *)row->val)[game->tetromino.x + j].active = true;
				((Block *)row->val)[game->tetromino.x + j].texture_pos =
					game->tetromino.id;
			}
		}
		if (row->next == NULL) {
			break;
		}
		row = row->next;
	}
	next_tetromino(game);
}

void rotate_tetromino(DblLinkedList *board, Tetromino *tet, Direction dir,
					  bool nested) {
	if (dir == ClockWise) {
		tet_swap_diag(tet);
		tet_rev_row(tet);
	} else {
		tet_rev_row(tet);
		tet_swap_diag(tet);
	}
	if (!nested) {
		if (check_tetromino_collision(board, tet, 0, 0) ||
			!in_bounds(tet, 0, 0)) {
			rotate_tetromino(board, tet, !dir, true);
		}
	}
}

void drop_tetromino(DblLinkedList *board, Tetromino *tet) {
	while (!check_tetromino_collision(board, tet, 0, 1) &&
		   in_bounds(tet, 0, 1)) {
		tet->y++;
	}
}

void hold_tetromino(Game *game) {
	if (game->hold_tetromino == 100) {
		game->hold_tetromino = game->tetromino.id;
		next_tetromino(game);
		game->held_tetromino = false;
		return;
	}
	uint_fast8_t swap = game->tetromino.id;
	game->tetromino = POSSIBLE_TETROMINOS[game->hold_tetromino];
	game->hold_tetromino = swap;
	game->held_tetromino = true;
}

int update_game(Game *game, Camera2D *cam) {
	// Update Game Sub Frame
	if (game->frame != FPS) {
		game->frame++;
	} else {
		game->frame = 1;
	}

	// Movement Checks
	if (game->frame % MOVE_SPEED == 0) {
		// Horizontal Movement
		if (IsKeyDown(KEY_RIGHT) ^ IsKeyDown(KEY_LEFT)) {
			int_fast8_t vel_x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
			if (in_bounds(&game->tetromino, vel_x, 0) &&
				!check_tetromino_collision(&game->board, &game->tetromino,
										   vel_x, 0)) {
				game->tetromino.x += vel_x;
			}
		}

		// Vertical Movement
		if (IsKeyDown(KEY_DOWN)) {
			if (in_bounds(&game->tetromino, 0, 1) &&
				!check_tetromino_collision(&game->board, &game->tetromino, 0,
										   1)) {
				game->tetromino.y += 1;
			}
		}
	}

	// Tetromino Rotation
	if (IsKeyPressed(KEY_UP)) {
		rotate_tetromino(&game->board, &game->tetromino, ClockWise, false);
	} else if (IsKeyPressed(KEY_Z)) {
		rotate_tetromino(&game->board, &game->tetromino, CounterClockWise,
						 false);
	}

	// Tetromino Dropping & Holding
	if (IsKeyPressed(KEY_SPACE)) {
		drop_tetromino(&game->board, &game->tetromino);
		push_tetromino(game);
		game->held_tetromino = false;
		game->screen_shake = SCREEN_SHAKE_LEN;
	}
	if (IsKeyPressed(KEY_C) && !game->held_tetromino) {
		hold_tetromino(game);
	}

	// Move Tetromino Down
	if (game->frame % (int)round(game->drop_speed) == 0) {
		if (!check_tetromino_collision(&game->board, &game->tetromino, 0, 1) &&
			in_bounds(&game->tetromino, 0, 1)) {
			game->tetromino.y += 1;
		} else {
			push_tetromino(game);
			game->held_tetromino = true;
			if (check_tetromino_collision(&game->board, &game->tetromino, 0,
										  0)) {
				return 1;
			}
		}
	}

	DblNode *node = game->board.head;
	for (int i = 0; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			if (!((Block *)node->val)[j].active) {
				goto OUTER_LOOP;
			}
		}
		game->lines_cleared++;
		snprintf(game->lines_txt, MAX_LINES_DIGITS, "%d", game->lines_cleared);
		if (game->drop_speed > MAX_DROP_SPEED) {
			game->drop_speed -= 0.3;
		}
		free(node->val);
		node = node->next;
		del_dbl_node_i(&game->board, i);
		insert_dbl_node_at_head(&game->board, calloc(COL_SIZE, sizeof(Block)));
		continue;
	OUTER_LOOP:;
		node = node->next;
	}

	if (game->screen_shake > 0) {
    cam->offset.x = (rand() % SCREEN_SHAKE_AMP) - (SCREEN_SHAKE_AMP/2);
    cam->offset.y = (rand() % SCREEN_SHAKE_AMP) - (SCREEN_SHAKE_AMP/2);
    // cam->offset.x = rand();
    // cam->offset.y = rand();
		game->screen_shake--;
	 } //else if (game->screen_shake == 0) {
 //    cam->offset.x = 0;
 //    cam->offset.y = 0;
	// 	game->screen_shake--;
 //  }

	return 0;
}

void draw_game(Game *game) {
	// Drawing Border
	DrawRectangle(SIDE_BUFFER, 0, TILE_SIZE, H, GRAY);
	for (int i = 0; i < ROW_SIZE + 1; i++) {
		DrawTextureRec(game->texture_map, game->border_block,
					   (Vector2){SIDE_BUFFER, i * TILE_SIZE}, WHITE);
	}
	for (int i = 0; i < ROW_SIZE + 1; i++) {
		DrawTextureRec(game->texture_map, game->border_block,
					   (Vector2){W - SIDE_BUFFER - TILE_SIZE, i * TILE_SIZE},
					   WHITE);
	}
	for (int i = 0; i < COL_SIZE; i++) {
		DrawTextureRec(
			game->texture_map, game->border_block,
			(Vector2){SIDE_BUFFER + TILE_SIZE + i * TILE_SIZE, H - TILE_SIZE},
			WHITE);
	}

	// Drawing Map and Current Tetromino
	draw_map(&game->board, &game->texture_map);
	draw_tetromino(&game->tetromino, game->tetromino.x * TILE_SIZE,
				   game->tetromino.y * TILE_SIZE, &game->texture_map);

	// Drawing Stats
	DrawText("HOLD", SIDE_BUFFER / 2 - MeasureText("HOLD", FONT_SIZE) / 2,
			 TILE_SIZE, FONT_SIZE, FG_COLOUR);
	DrawText("NEXT", W - SIDE_BUFFER / 2 - MeasureText("NEXT", FONT_SIZE) / 2,
			 TILE_SIZE, FONT_SIZE, FG_COLOUR);
	DrawText("LINES", W - SIDE_BUFFER / 2 - MeasureText("LINES", FONT_SIZE) / 2,
			 TILE_SIZE * 6.5, FONT_SIZE, FG_COLOUR);
	DrawText(game->lines_txt,
			 W - SIDE_BUFFER / 2 -
				 MeasureText(game->lines_txt, FONT_SIZE - 2) / 2,
			 TILE_SIZE * 7.75, FONT_SIZE - 2, MAROON);

	// Drawing Hold & Next Tetrominos
	if (game->hold_tetromino != 100) {
		draw_tetromino(&POSSIBLE_TETROMINOS[game->hold_tetromino],
					   -(TILE_SIZE + SIDE_BUFFER / 2 +
						 (POSSIBLE_TETROMINOS[game->hold_tetromino].dimension *
						  TILE_SIZE) /
							 2) +
						   (tetromino_offset_left(
								&POSSIBLE_TETROMINOS[game->hold_tetromino]) *
							TILE_SIZE) /
							   2,
					   TILE_SIZE * 2.5, &game->texture_map);
	}
	draw_tetromino(&POSSIBLE_TETROMINOS[game->next_tetromino],
				   (W - SIDE_BUFFER * 2 - TILE_SIZE) + (SIDE_BUFFER / 2) -
					   (TILE_SIZE *
						(POSSIBLE_TETROMINOS[game->next_tetromino].dimension -
						 tetromino_offset_left(
							 &POSSIBLE_TETROMINOS[game->next_tetromino])) /
						2),
				   TILE_SIZE * 2.5, &game->texture_map);
}

void init_game(Game *game) {
	game->texture_map = LoadTexture("./assets/tetris_map.png");
	game->border_block = (Rectangle){
		.x = TILE_SIZE * 7, .y = 0, .width = TILE_SIZE, .height = TILE_SIZE};
	game->board = new_dbl_linked_list();
	game->board.node_val_size = sizeof(bool) * COL_SIZE;
	for (int i = 0; i < ROW_SIZE; i++) {
		insert_dbl_node_at_head(&game->board, malloc(COL_SIZE * sizeof(Block)));
	}
	game->screen_shake = 0;
}

void next_tetromino(Game *game) {
	game->tetromino = POSSIBLE_TETROMINOS[game->next_tetromino];
	rand();
	game->next_tetromino = rand() % 7;
}

void reset_game(Game *game) {
	srand(time(NULL));
	game->frame = 0;
	game->drop_speed = (FPS / 3) * 2;
	game->hold_tetromino = 100; // INVALID
	game->held_tetromino = false;
	game->lines_cleared = 0;
	snprintf(game->lines_txt, MAX_LINES_DIGITS, "%d", game->lines_cleared);

	// Reset RNG and Get Starting Tetromino
	rand();
	game->tetromino = POSSIBLE_TETROMINOS[rand() % 7];
	game->next_tetromino = rand() % 7;

	// Reset Board
	DblNode *node = game->board.head;
	for (int i = 0; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			((Block *)node->val)[j].active = false;
		}
		node = node->next;
	}
}

void end_game(Game *game) {
	DblNode *node = game->board.head;
	for (int i = 0; i < game->board.len; i++) {
		free(node->val);
		node = node->next;
	}
	UnloadTexture(game->texture_map);
	delete_dbl_linked_list(&game->board);
}
