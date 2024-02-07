#define FPS 60
#define MOVE_SPEED 5
#define MAX_DROP_SPEED (FPS / 14)
#define BG_COLOUR BLACK
#define FG_COLOUR WHITE
#define FONT_SIZE 32
#define ROW_SIZE 20
#define COL_SIZE 10
#define TILE_SIZE 33
#define SIDE_BUFFER (TILE_SIZE * 6)
#define W ((SIDE_BUFFER * 2) + (TILE_SIZE * COL_SIZE) + (TILE_SIZE * 2))
#define H ((TILE_SIZE * ROW_SIZE) + (TILE_SIZE * 1))
#define GAME_OVER 1
#define MAX_LINES_DIGITS 1000
#define SCREEN_SHAKE_AMP 3
#define SCREEN_SHAKE_LEN (FPS / 5)
